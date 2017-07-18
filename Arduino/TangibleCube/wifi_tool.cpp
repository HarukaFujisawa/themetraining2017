
#include <ESP8266WiFi.h>

const char* ssid = "aterm-92ffc1-g";
const char* password = "4ff583971aecc";
//const char* ssid = "ixd-2-euler";
//const char* password = "eulerjunkimura";

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


