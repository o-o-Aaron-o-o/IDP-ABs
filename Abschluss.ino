/* Program: Abschluss.ino including EKG.ino, Pulsoximeter.ino, Display.ino, Display.h, AbsHeader.h, fonts.h and small_fonts_h
 * Authors: Lucia Risuglia, Aaron Venter, Nadine Roth, Jazli Hyzan -- based on Code from Georg Passig
 * uC:  Arduino nano 33 BLE
 * Created July 22nd, 2024
 *
 * Geleistete Arbeit am Gesamtprojekt:  Nadine Roth:    25 %
 *                                      Jazli Hyzan:    25 %
 *                                      Lucia Risuglia: 25 %
 *                                      Aaron Venter:   25 %
 *
 *
 * Das Projekt wurde nicht von einer Einzelperson bearbeitet.  
 */




#include "Display.h"
#include "AbsHeader.h"




BLEService Moduswechsel("C89C");

BLEIntCharacteristic switchSensorCharacteristic("FAF3", BLEWrite |  BLEWriteWithoutResponse);





void setup() 
{
  Serial.begin(115200);

  EKG_Init();

  Oxi_Init();

  Display_Init(colLightBlue);

  BLE_Init();
}






void loop()  
{
  static int Modus = 0;
  BLEDevice central = BLE.central();

  if (central) 
  {

    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected())                       //While-Schleife, die der loop ist solange das Arduino-Board mit einem Central-Gerät verbunden ist
    {
      if(switchSensorCharacteristic.written())
        Modus = switchSensorCharacteristic.value();


      switch(Modus) 
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



void BLE_Init()
{
  // Begin initialization
  if (!BLE.begin()) 
  {
    Serial.println("Starting BLE failed!");
    while (1);
  }
  
  BLE.setLocalName("My-Little-Pony");

  BLE.setAdvertisedService(Moduswechsel);

  Moduswechsel.addCharacteristic(switchSensorCharacteristic);

  BLE.addService(Moduswechsel);

  BLE.advertise();

  Serial.println("Waiting for connections...");
}



