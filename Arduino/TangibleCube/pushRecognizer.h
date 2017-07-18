#pragma once

#include <stdlib.h>
#include <Wire.h>

#define BME280_ADDRESS 0x76

struct Push
{
  int result; //1:push start 2: push end
  float time;
  float MaxDiff;
};

class pushRecognizer
{
  public:
    pushRecognizer();
    ~pushRecognizer();

    void init();
    void updateData();

    double getTemp() { return temp_act; };
    double getHum() { return hum_act; };
    double getPress() { return press_act; };
    
    void readTrim();
    void writeReg(uint8_t reg_address, uint8_t data);
    void readData();
    signed long int calibration_T(signed long int adc_T);
    unsigned long int calibration_P(signed long int adc_P);
    unsigned long int calibration_H(signed long int adc_H);
    
    
    Push input(unsigned long timestamp);
    //Push input(float pressure, unsigned long timestamp);

  private:
    int mState;
    float cof;
    float mDefaultVal;
    float mThreshVal;

    float mPrevVal;
    float mMaxVal;

    float t1;
    float t2;

    //センサの値
    double temp_act;
    double hum_act;
    double press_act;

    unsigned long int hum_raw,temp_raw,pres_raw;
    signed long int t_fine;
    
    uint16_t dig_T1;
     int16_t dig_T2;
     int16_t dig_T3;
    uint16_t dig_P1;
     int16_t dig_P2;
     int16_t dig_P3;
     int16_t dig_P4;
     int16_t dig_P5;
     int16_t dig_P6;
     int16_t dig_P7;
     int16_t dig_P8;
     int16_t dig_P9;
     int8_t  dig_H1;
     int16_t dig_H2;
     int8_t  dig_H3;
     int16_t dig_H4;
     int16_t dig_H5;
     int8_t  dig_H6;
};

