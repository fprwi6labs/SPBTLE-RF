#include <SPI.h>
#include <SPBTLE_RF.h>
#include <beacon_service.h>

/* Configure SPI3:
  MOSI: PC12
  MISO: PC11
  SCLK: PC10
  */
SPIClass SPI_3(44, 43, 42);
SPBTLERFClass BTLE(&SPI_3, 50, 57, 31, LED4);

// Mac address
uint8_t SERVER_BDADDR[] = {0x12, 0x34, 0x00, 0xE1, 0x80, 0x03};

// Beacon ID, the 6 last bytes are used for NameSpace
uint8_t NameSpace[] = "www.st.com";
uint8_t BEACON_ID[] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6};

void setup() {
  Serial.begin(9600);

  if(BTLE.begin())
  {
    Serial.println("Bluetooth module configuration error!");
    while(1);
  }

  if(BeaconService.begin(SERVER_BDADDR, BEACON_ID, NameSpace))
  {
    Serial.println("Beacon service configuration error!");
    while(1);
  }
  else
  {
    Serial.println("Beacon service started!");
  }
}

void loop() {
  BTLE.update();
}
