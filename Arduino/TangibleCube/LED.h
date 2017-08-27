#include <Adafruit_NeoPixel.h>

#define PIN            13
#define NUMPIXELS      2

class LED
{
  public:
      Adafruit_NeoPixel pixels;
      
      LED()
      {        
      }

      ~LED()
      {        
      }
      
      void init() { 
        pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
        pixels.begin();

        Serial.println("LED init");
      }

      void onLED(uint8_t r, uint8_t g, uint8_t b)
      {
        //noInterrupts();//割り込み禁止！
        Serial.println("PIKA");
        pixels.setPixelColor(0, pixels.Color(r, g, b));
        pixels.setPixelColor(1, pixels.Color(r, g, b));
        pixels.show(); // This sends the updated pixel color to the hardware.  
        //interrupts(); //割り込み再開！      
      }

      void offLED()
      {
        noInterrupts();//割り込み禁止！        
        pixels.clear();
        pixels.show(); // This sends the updated pixel color to the hardware.        
        interrupts(); //割り込み再開！      
      }

};

