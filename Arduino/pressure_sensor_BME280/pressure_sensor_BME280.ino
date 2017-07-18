#include "pushRecognizer.h"

static pushRecognizer mPushRecog;

void setup()
{
  Serial.begin(115200);
  mPushRecog.init();
}


void loop()
{    
  //mPushRecog.updateData();  
  //Push pushResult = mPushRecog.input(mPushRecog.getPress(), millis());
  
  Push pushResult = mPushRecog.input(millis());
  if(pushResult.result)
  {
   Serial.print(pushResult.result);
   Serial.print(" , ");
   Serial.print(pushResult.time);
   Serial.print(" , ");
   Serial.println(pushResult.MaxDiff);
   //Serial.println(mPushRecog.getDefaultVal());
  }
  
  delay(10); //表示は100fps
}

