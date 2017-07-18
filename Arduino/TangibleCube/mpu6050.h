
#pragma once

struct MotionSensorData {
  int timestamp;
  signed short ax;
  signed short ay;
  signed short az;
  signed short gx;
  signed short gy;
  signed short gz;
};

struct MotionSensorDataf {
  int timestamp;
  float ax;
  float ay;
  float az;
  float gx;
  float gy;
  float gz;
};

void initITGMPU();
void updateITGMPU();
bool getITGMPUData(MotionSensorData &data);
bool getITGMPUData(MotionSensorDataf &data);
void convertITGMPU2MKS(MotionSensorDataf &mksData, const MotionSensorData &data);


