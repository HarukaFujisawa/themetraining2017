#include <Arduino.h>
#include <Wire.h>
#include "i2c_tool.h"
#include "mpu6050.h"
#include "nzGyroPosture.h"

/**
 *	ITG_MPU motion sensor
 *	register map...
 *	http://www.i2cdevlib.com/devices/mpu6050#registers
 */
 
#define PWR_MGMT_1    0x6B
#define ACCEL_X_OUT_H 0x3B
#define GYRO_X_OUT_H  0x43
#define CONFIG        0x1A
#define GYRO_CONFIG   0x1B
#define ACCEL_CONFIG  0x1C

//#define SERIAL_DEC_GYRO_OUT
//#define SERIAL_DEC_ACC_OUT

const static float SENSITIVITY_SCALE_FACTOR = 1.0f / 32.8f;  // param for MPU-6050
const static float GRAVITY = (9.80665f);
const static float G_PER_LSB = (1.0f / 8192.0f); // param for MPU6050

const static byte deviceAddress = 0x68;

void gyroInit();
void accInit();
void initITGMPU();
void updateITGMPU();
void printMotionValues();
void gyro3Axis();
void acc3Axis();


// モーションセンサの値
MotionSensorData s_data;
static nzGyroPosture s_posture;

void initITGMPU()
{
  //Wire.begin(4, 14);
  //delay(50);

  writeI2C(deviceAddress, CONFIG, (0x03 << 3));

  gyroInit();
  accInit();
  delay(500);
}

void updateITGMPU()
{
  //noInterrupts();  // 割り込み禁止区間
  // 譎る俣繧定ｨ域ｸｬ
  unsigned long startTime, endTime, procTime;
  startTime = micros();
  gyro3Axis();
  acc3Axis();
  
  //interrupts();  // 割り込み許可
  printMotionValues();
  endTime = micros();
  procTime = endTime - startTime;
  delayMicroseconds(10000 - procTime);
  s_data.timestamp = startTime;
}

bool getITGMPUData(MotionSensorData &data)
{
  bool err = true;
  if(data.timestamp != s_data.timestamp) {
    data = s_data;
    err = true;
  }
  return err;
}

bool getITGMPUData(MotionSensorDataf &data)
{
  MotionSensorData d;
  bool err = getITGMPUData(d);
  if(err) {
    convertITGMPU2MKS(data, d);
  }
  return err;
}

void convertITGMPU2MKS(MotionSensorDataf &mksData, const MotionSensorData &data)
{
  mksData.timestamp = data.timestamp;
  mksData.ax = data.ax * G_PER_LSB * GRAVITY;
  mksData.ay = data.ay * G_PER_LSB * GRAVITY;
  mksData.az = data.az * G_PER_LSB * GRAVITY;
  mksData.gx = data.gx * SENSITIVITY_SCALE_FACTOR;
  mksData.gy = data.gy * SENSITIVITY_SCALE_FACTOR;
  mksData.gz = data.gz * SENSITIVITY_SCALE_FACTOR;
}

void printMotionValues()
{
#ifdef SERIAL_DEC_GYRO_OUT
  Serial.print("gx = ");
  Serial.print(s_data.gx);
  Serial.print(", gy = ");
  Serial.print(", ");
  Serial.print(s_data.gy);
  Serial.print(", gz = ");
  Serial.print(", ");
  Serial.println(s_data.gz);
#endif
#ifdef SERIAL_DEC_ACC_OUT
  Serial.print(" ax = ");
  Serial.print(", ");
  Serial.print(s_data.ax);
  Serial.print(", ay = ");
  Serial.print(", ");
  Serial.print(s_data.ay);
  Serial.print(", az = ");
  Serial.print(", ");
  Serial.println(s_data.az);
#endif
}

void gyroInit()
{
  writeI2C(deviceAddress, PWR_MGMT_1, 2);
  delay(100);
  writeI2C(deviceAddress, GYRO_CONFIG, (2 << 3));
  delay(100);
}

void accInit()
{
  writeI2C(deviceAddress, ACCEL_CONFIG, (1 << 3));
  delay(100);
}

void gyro3Axis()
{
  byte Value[6];
  readI2C(deviceAddress,GYRO_X_OUT_H,Value, 6);

  s_data.gx = (Value[0]<<8) | Value[1];
  s_data.gy = (Value[2]<<8) | Value[3];
  s_data.gz = (Value[4]<<8) | Value[5];  
}

void acc3Axis()
{
  byte Value[6];
  readI2C(deviceAddress,ACCEL_X_OUT_H, Value, 6);

  s_data.ax = (Value[0]<<8) | Value[1];
  s_data.ay = (Value[2]<<8) | Value[3];
  s_data.az = (Value[4]<<8) | Value[5];
}




