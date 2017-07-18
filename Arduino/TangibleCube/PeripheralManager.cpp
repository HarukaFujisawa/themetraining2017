
#include <Arduino.h>
#include "PeripheralManager.h"
#include <ArduinoJson.h>

PeripheralManager::PeripheralManager()
{
}

PeripheralManager::~PeripheralManager()
{
}

bool PeripheralManager::init(int port)
{
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

  // UDP蜿嶺ｿ｡繝舌ャ繝輔ぃ繧偵メ繧ｧ繝�繧ｯ.
  checkReceive();

  // 髱槭い繧ｯ繝�繧｣繝悶ョ繝舌う繧ｹ縺ｮ繝√ぉ繝�繧ｯ.
  if(abs(t - s_checkTime) > 1000) {
    s_checkTime = t;
    checkInactiveDevice();
  }

  // alive騾∝�ｺ
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
    if(readSize > 0) {  // 譁�蟄怜�励�ｮ邨らｫｯ繧定ｨｭ螳�
      buffer[readSize] = '\0';
    }

    IPAddress remoteIP = m_udp.remoteIP();
    int remotePort = m_udp.remotePort();

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
          sendAvailable(remoteIP, remotePort);  // 閭ｽ蜉帙ｒ騾夐＃.
        //} else if(0 == strcmp(pcmd, "available")) {
        } else if(0 == strcmp(pcmd, "request")) {
          PeripheralDevice device;
          device.address     = remoteIP;
          device.port        = remotePort;
          device.latestAlive = millis();

          PeripheralDevice *pDevice = getDevice(remoteIP, remotePort);
          if(NULL == pDevice) {
            m_devices.push_back(device);  // 繝�繝舌う繧ｹ繧堤匳骭ｲ.
          } else {
            *pDevice = device;
          }
        } else if(0 == strcmp(pcmd, "alive")) {
          PeripheralDevice *pDevice = getDevice(remoteIP, remotePort);
          if(NULL != pDevice) {
            pDevice->latestAlive = millis();  // 譎ょ綾繧呈峩譁ｰ.
          }
        } else if(0 == strcmp(pcmd, "bye")) {
        }
      }
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

void PeripheralManager::sendAvailable(IPAddress address, int port)
{
  StaticJsonBuffer<255> jsonBuffer;
  JsonObject &root = jsonBuffer.createObject();
  root["pcmd"] = "available";
  JsonArray &array = root.createNestedArray("ability");
  for(std::list<std::string>::iterator itr = m_ability.begin(); itr != m_ability.end(); itr++) {
    array.add((const char *)itr->c_str());
  }
  char buff[255];
  root.printTo(buff, 255);

  send(buff, address, port);

  Serial.print("send available: ");
  Serial.println(buff);
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




