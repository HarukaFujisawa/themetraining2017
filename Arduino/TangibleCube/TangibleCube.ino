#include "mpu6050.h"
#include "wifi_tool.h"
#include "nzGyroPosture.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUDP.h>
#include <ArduinoJson.h>
#include "PeripheralManager.h"
#include "pushRecognizer.h"


const static char *s_pHostName = "spatial_dev_01"; // set device hostname for mDNS
const char *s_pRemoteAddress = "192.168.11.5";
const int s_remotePort = 10386;
const int s_localPort = 10386;
static MDNSResponder s_dns; // mDNS Responder
static nzGyroPosture s_posture;
static PeripheralManager s_remote;

const static float GRAVITY = (9.80665f);
const static int LED_PIN = 13; //5
//const static int TOUCH_CHARGE_PIN = 12;
//const static int TOUCH_SENSOR_PIN = 13;

static bool checkRecvUDP();
static void sendUDP_motion(MotionSensorDataf &data);
static void sendUDP_push(Push &p_data);
//static bool blinkLED(int sw = -1); // sw= -1:blink, 0:off, 1:on

static pushRecognizer mPushRecog;

void setup()
{
  Serial.begin(115200);

  Wire.begin(4, 5); //SCLとSDAのピン番号
  delay(50);
  
  initITGMPU();
  mPushRecog.init();
  wifiConnect();  

  pinMode(LED_PIN, OUTPUT);

  delay(100);
  if(s_remote.init(s_localPort)) {
    // Registering device ability.
    s_remote.getAbility().push_back("gyroscope");
    s_remote.getAbility().push_back("accelerometer");
    s_remote.getAbility().push_back("motion2"); //ぱぺぞうの番号によってここの数字を変える
    s_remote.getAbility().push_back("push2");   //ぱぺぞうの番号によってここの数字を変える

    s_remote.createAbilityBuffer();
    s_remote.onLED(255, 255, 255);

    Serial.print("Start Listing UDP port : ");
    Serial.println(s_localPort);
    delay(100);

    if(s_dns.begin(s_pHostName, WiFi.localIP())) {  // Start mDNS.
      Serial.print("Start mDNS: ");
      Serial.println(s_pHostName);
    } else {
      Serial.println("Failed to start mDNS.");
    }
  } else {
    Serial.println("Failed to start UDP.");
  }
}

void loop()
{ 
#if true 
  updateITGMPU();

  //*******Gyro***********//
  MotionSensorDataf data;
  if(getITGMPUData(data)) {
    nzVector3f gyro(data.gx, data.gy, data.gz);
    nzVector3f acc(data.ax, data.ay, data.az);
    s_posture.inputGyroscope(data.timestamp, gyro);
    s_posture.inputAccelerometer(data.timestamp, acc);
  }
  //*******Gyro***********//
   
  //*******Push Recognizer***********//
  Push pushResult = mPushRecog.input(millis());
  //Serial.println(mPushRecog.getPress());
  if(pushResult.result)
  {
    sendUDP_push(pushResult);   
    Serial.print(pushResult.result);
    Serial.print(" , ");
    Serial.print(pushResult.time);
    Serial.print(" , ");
    Serial.println(pushResult.MaxDiff);
  }
  //*******Push Recognizer***********//

  #endif
  
  static long s_latestUpdate = 0;
  long curr = millis();
  if(curr - s_latestUpdate < 16) {
    return; // タイトループここまで
  }
  s_latestUpdate = curr;

  static int s_checkCount = 0;
  s_checkCount++;
//  if((s_checkCount % 3) == 0) { // == 0だお
//    s_remote.update();
//    s_dns.update();  
//
//    s_posture.update();
//    sendUDP_motion(data);        
//  } else {
//  }

  
  if((s_checkCount % 2) == 0) { // == 0だお
    s_remote.update();
    if((s_checkCount % 10) == 0) { // == 0だお
      s_dns.update();      
    }
  } else {
    s_posture.update();
    sendUDP_motion(data);
  }

  
#if false

    Serial.print("acc(");
    Serial.print(String(data.ax, 3));
    Serial.print(", ");
    Serial.print(String(data.ay, 3));
    Serial.print(", ");
    Serial.print(String(data.az, 3));
    Serial.print(") ");

    Serial.print("gyro(");
    Serial.print(String(data.gx, 3));
    Serial.print(", ");
    Serial.print(String(data.gy, 3));
    Serial.print(", ");
    Serial.print(String(data.gz, 3));
    Serial.print(") ");

    Serial.print("rot(");
    Serial.print(String(s_posture.quat.i.x, 3));
    Serial.print(", ");
    Serial.print(String(s_posture.quat.i.y, 3));
    Serial.print(", ");
    Serial.print(String(s_posture.quat.i.z, 3));
    Serial.print(", ");
    Serial.print(String(s_posture.quat.r, 3));
    Serial.println(")");
#endif

/*
  if(fabs(acc.length() - GRAVITY) > .5f || gyro.length() > 9.0f){
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
*/
}

static void sendUDP_motion(MotionSensorDataf &data)
{
  StaticJsonBuffer<512> jsonBuffer;
  
  unsigned long starttime = micros();

  JsonObject &root = jsonBuffer.createObject();
  root["type"] = 0;
  root["time"] = (int)micros();
  JsonObject &gyroscope     = root.createNestedObject("gyro");
  JsonObject &accelerometer = root.createNestedObject("acc");
  JsonObject &postureRot    = root.createNestedObject("rot");

  gyroscope["x"] = (float)data.gx;
  gyroscope["y"] = (float)data.gy;
  gyroscope["z"] = (float)data.gz;
  accelerometer["x"] = (float)data.ax;
  accelerometer["y"] = (float)data.ay;
  accelerometer["z"] = (float)data.az;
  postureRot["x"] = (float)s_posture.quat.i.x;
  postureRot["y"] = (float)s_posture.quat.i.y;
  postureRot["z"] = (float)s_posture.quat.i.z;
  postureRot["w"] = (float)s_posture.quat.r;
    
  char buff[512];
  root.printTo(buff, 512);

  s_remote.send(buff);
  //Serial.println(buff);
}

static void sendUDP_push(Push &p_data)
{
  StaticJsonBuffer<512> jsonBuffer;
  
  JsonObject &root = jsonBuffer.createObject();
  root["type"] = 1;
  JsonObject &push = root.createNestedObject("push");

  push["result"] = (float)p_data.result;
  push["time"] = (float)p_data.time;
  push["MaxDiff"] = (float)p_data.MaxDiff;
  
  char buff[512];
  root.printTo(buff, 512);

  s_remote.send(buff);
}


//static bool blinkLED(int sw)
//{
//  static bool s_on = false;
//  if(sw < 0) {
//    s_on = !s_on;
//  } else if(sw == 0) {
//    s_on = false;
//  } else {
//    s_on = true;
//  }
//  digitalWrite(LED_PIN, s_on ? HIGH : LOW);
//}



