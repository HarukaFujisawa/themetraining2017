
#include <Arduino.h>
#include "PeripheralManager.h"

PeripheralManager::PeripheralManager()
{
}

PeripheralManager::~PeripheralManager()
{
  led.offLED();
}

bool PeripheralManager::init(int port)
{
  isCreatedMyAbilityBuff = false;
  led.init();
  //led.onLED();//(150, 150, 150);
    
  bool err = false;
  err = m_udp.begin(port);
  return err;
}

bool PeripheralManager::send(const char *pData)
{
  bool err = true;
  for(std::list<PeripheralDevice>::iterator itr = m_devices.begin(); itr != m_devices.end(); itr++) {
    bool e = send(pData, itr->address, itr->port);
    err = (e == true && err == true);
  }
  return err;
}

bool PeripheralManager::send(const char *pData, const char *pAddress, int port)
{
  bool err = false;
  if(m_udp.beginPacket(pAddress, port)) {
    m_udp.write(pData);
    m_udp.endPacket();
    m_udp.flush();
    err = true;
  }
  return err;
}

bool PeripheralManager::send(const char *pData, IPAddress address, int port)
{
  bool err = false;
  if(m_udp.beginPacket(address, port)) {
    m_udp.write(pData);
    m_udp.endPacket();
    m_udp.flush();
    err = true;
  }
  return err;
}

void PeripheralManager::update()
{
  static unsigned long s_checkTime = 0;
  static unsigned long s_alivePingTime = 0;
  unsigned long t = millis();

  // UDP受信バッファをチェック.
  checkReceive();

  // 非アクティブデバイスのチェック.
  if(abs(t - s_checkTime) > 1000) {
    s_checkTime = t;
    checkInactiveDevice();
  }

  // alive送出
  if(abs(t - s_alivePingTime) > 5000) {
    s_alivePingTime = t;
    sendAlive();
  }
}

void PeripheralManager::checkReceive()
{
  int dataReady = m_udp.parsePacket();
  if(dataReady) {
    char buffer[256];

    int readSize = m_udp.read(buffer, (dataReady < 255 ? dataReady : 255));
    if(readSize > 0) {  // 文字列の終端を設定
      buffer[readSize] = '\0';
    }

    IPAddress remoteIP = m_udp.remoteIP();
    int remotePort = m_udp.remotePort();

    Serial.println("*******");
    Serial.print("Recieve from ");
    Serial.print(remoteIP);
    Serial.print(" : ");
    Serial.println(buffer);

    StaticJsonBuffer<255> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buffer);
    if(root.success()) {
      if(root.containsKey("pcmd")) {
        const char *pcmd = (const char *)root["pcmd"];
        if(0 == strcmp(pcmd, "search")) {
          sendAvailable(remoteIP, remotePort);  // 能力を通達. //※
        //} else if(0 == strcmp(pcmd, "available")) {
        } else if(0 == strcmp(pcmd, "request")) {
          PeripheralDevice device;
          device.address     = remoteIP;
          device.port        = remotePort;
          device.latestAlive = millis();

          PeripheralDevice *pDevice = getDevice(remoteIP, remotePort);
          if(NULL == pDevice) {
            m_devices.push_back(device);  // デバイスを登録.
          } else {
            *pDevice = device;
          }
        } else if(0 == strcmp(pcmd, "alive")) {
          PeripheralDevice *pDevice = getDevice(remoteIP, remotePort);
          if(NULL != pDevice) {
            pDevice->latestAlive = millis();  // 時刻を更新.
          }
        } else if(0 == strcmp(pcmd, "bye")) {
        }
      }
      else if(root.containsKey("event")){ //イベントをキャッチ  
        const char *event = (const char *)root["event"];
        Serial.println(event);        
        if(containsNestedKey(root, "onLED")) {        
          //Lチカ
          Serial.println("LED ON");
          led.onLED(root["event"]["onLED"][0], root["event"]["onLED"][1], root["event"]["onLED"][2]);   
        }else if(0 == strcmp(event, "offLED")) {  
          Serial.println("LED OFF"); 
          led.offLED();
        }
      }
    }
    else
    {
      Serial.println("???????????????");
    }
  }
}

void PeripheralManager::checkInactiveDevice()
{
  unsigned long t = millis();
  for(std::list<PeripheralDevice>::iterator itr = m_devices.begin(); itr != m_devices.end(); ) {
    if(abs(t - itr->latestAlive) > 23000) {
      Serial.print(itr->address);
      Serial.print(" leave. Peripheral available rest ");
      Serial.println(m_devices.size() - 1);
      sendBye(itr->address, itr->port);
      itr = m_devices.erase(itr);
    } else {
      itr++;
    }
  }
}

void PeripheralManager::createAbilityBuffer()
{
  StaticJsonBuffer<255> jsonBuffer;
  JsonObject &root = jsonBuffer.createObject();
  root["pcmd"] = "available";
  JsonArray &array = root.createNestedArray("ability");

  //if(!array.success()) return;
  
  for(std::list<std::string>::iterator itr = m_ability.begin(); itr != m_ability.end(); itr++) {
    Serial.println("start");
    Serial.println((const char *)itr->c_str()); //※
    //array.add((const char *)itr->c_str()); 
    Serial.println(array.add((const char *)itr->c_str()));
    Serial.println("done");
  }

  root.printTo(myAbilityBuff, 255);

  Serial.print("create available buffer: ");
  Serial.println(myAbilityBuff);
  isCreatedMyAbilityBuff = true;
}

void PeripheralManager::sendAvailable(IPAddress address, int port)
{
  /*
  StaticJsonBuffer<255> jsonBuffer;
  JsonObject &root = jsonBuffer.createObject();
  root["pcmd"] = "available";
  JsonArray &array = root.createNestedArray("ability");

  if(!array.success()) return;
  
  for(std::list<std::string>::iterator itr = m_ability.begin(); itr != m_ability.end(); itr++) {
    Serial.println("start");
    Serial.println((const char *)itr->c_str()); //※
    //array.add((const char *)itr->c_str()); 
    Serial.println(array.add((const char *)itr->c_str()));
    Serial.println("done");
  }
  char buff[255];
  root.printTo(buff, 255);
*/
if(isCreatedMyAbilityBuff){
  send(myAbilityBuff, address, port);
}

  Serial.print("send available: ");
  Serial.println(myAbilityBuff);
}

void PeripheralManager::sendAlive()
{
  for(std::list<PeripheralDevice>::iterator itr = m_devices.begin(); itr != m_devices.end(); itr++) {
    send("{\"pcmd\":\"alive\"}", itr->address, itr->port);
  }
}

void PeripheralManager::sendBye(IPAddress address, int port)
{
  send("{\"pcmd\":\"bye\"}", address, port);
}

PeripheralManager::PeripheralDevice *PeripheralManager::getDevice(IPAddress &address, int port)
{
  PeripheralDevice *pRet = NULL;
  for(std::list<PeripheralDevice>::iterator itr = m_devices.begin(); itr != m_devices.end(); itr++) {
    if(itr->address == address && itr->port == port) {
      pRet = &(*itr);
      break;
    }
  }
  return pRet;
}

bool PeripheralManager::containsNestedKey(const JsonObject& obj, const char* key) {
    for (const JsonPair& pair : obj) {
        if (!strcmp(pair.key, key))
            return true;

        if (containsNestedKey(pair.value.as<JsonObject>(), key)) 
            return true;
    }

    return false;
}




