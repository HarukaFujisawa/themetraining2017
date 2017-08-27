
#include <WiFiUDP.h>
#include <list>
#include <string>
#include <ArduinoJson.h>
#include "LED.h"

class PeripheralManager
{
public:
  PeripheralManager();
  virtual ~PeripheralManager();
  bool init(int port);
  bool send(const char *pData);
  bool send(const char *pData, const char *pAddress, int port);
  bool send(const char *pData, IPAddress address, int port);
  void update();
  void createAbilityBuffer();
  void onLED(uint8_t r, uint8_t g, uint8_t b){ led.onLED(r, g, b); };
  
  typedef struct _PeripheralDevice
  {
    IPAddress address;
    int port;
    unsigned long latestAlive;
  } PeripheralDevice;
  
  PeripheralManager::PeripheralDevice *getDevice(IPAddress &address, int port);

  WiFiUDP &getUDPSocket() {
    return m_udp;
  }

  std::list<std::string> &getAbility() {
    return m_ability;
  }

private:
  void checkReceive();
  void checkInactiveDevice();
  void sendAvailable(IPAddress address, int port);
  void sendBye(IPAddress address, int port);
  void sendAlive();

  WiFiUDP m_udp;
  std::list<std::string> m_ability;
  std::list<PeripheralDevice> m_devices;

  LED led;
  bool isCreatedMyAbilityBuff;
  char myAbilityBuff[255]; //sendAvailableで送るためのabilityのJson文字列
  bool containsNestedKey(const JsonObject& obj, const char* key);

};



