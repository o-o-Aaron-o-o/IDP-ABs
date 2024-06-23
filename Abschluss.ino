
#include "Display.h"
#include "AbsHeader.h"
#include <ArduinoBLE.h>

BLEService Modiwechsel("C89C");

BLEIntCharacteristic switchSensorCharacteristic("FAF3", BLEWrite |  BLEWriteWithoutResponse);





void setup() 
{
  Serial.begin(115200);


  SPI.begin();                                                          // Display ist Commandobereit

  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));     // Wie Serial.beginn fürs Display
  ST7735_Init();

  // Begin initialization
  if (!BLE.begin()) 
  {
    Serial.println("Starting BLE failed!");
    while (1);
  }
  
  BLE.setLocalName("My-Little-Pony");

  BLE.setAdvertisedService(Modiwechsel);

  Modiwechsel.addCharacteristic(switchSensorCharacteristic);

  BLE.addService(Modiwechsel);

  BLE.advertise();




  EKG_Init();

  Oxi_Init();

  FillWithColor(colLightBlue);

}






void loop()  
{
  static int Modi = 0;
  BLEDevice central = BLE.central();

  if (central) 
  {

    Serial.print("Connected to central: ");
    Serial.println(central.address());

    // While the central is connected
    while (central.connected()) 
    {
      if(switchSensorCharacteristic.written())
        Modi = switchSensorCharacteristic.value();


      switch (Modi) 
      {
        case 0:
                Serial.println("Stopped");
                break;
        case 1:
                loop_ekg();
                break;
        case 2:
                loop_oxi();
                break;
        default:
                break;
      
      }

    }
  }

}
