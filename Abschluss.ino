
#include "Display.h"
#include "AbsHeader.h"







void setup() 
{
  Serial.begin(115200);


  SPI.begin();                                                          // Display ist Commandobereit

  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));     // Wie Serial.beginn fürs Display
  ST7735_Init();


  EKG_Init();

  Oxi_Init();

  FillWithColor(colLightBlue);

}






void loop()  
{
  loop_oxi();
}
