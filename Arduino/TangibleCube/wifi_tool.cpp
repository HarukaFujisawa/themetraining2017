
#include <ESP8266WiFi.h>

//const char* ssid = "Puppetalk";//"aterm-92ffc1-g";
//const char* password = "atddixd2";//"4ff583971aecc";
const char* ssid = "aterm-3d20cb-g";
const char* password = "745ca754993a4";

bool wifiConnect()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  return true;
}


