#include "pins_arduino.h"
/* File: _idp_ST7735.h
 * Author: Georg Passig, based on Code from Armstrong Subero, modified from Adafruit Libraries and expanded by the Authors of Abschluss.ino
 * uC:  Arduino nano 33 BLE
 * Created May 26th, 2020
 * Fully modified January 18th, 2024
 */

#include <stdint.h>
#include <SPI.h>

#define Pin_A0 D4
#define Pin_Reset D7
#define Pin_CS D6



/*
  0x0     =   0   
    0x10    =  16   
      0x20    =  32   
        0x30    =  48   
          0x40  	=  64   
            0x50    =  80   
              0x60    =  96
                0x70    = 112
                  0x80    = 128
                    0x90  	= 144
                      0xA0    = 160
                        0xB0    = 176
                          0xC0    = 192
                            0xD0    = 208
                              0xE0    = 224
                                0xF0    = 240
*/


// ST7735 registers & defines
#define SCREEN_WIDTH 128        // 0x80
#define SCREEN_HEIGHT 160       // 0xA0

#define ST7735_NOP 0x0          // 0 
#define ST7735_SWRESET 0x01     // 1
#define ST7735_RDDID 0x04       // 4
#define ST7735_RDDST 0x09       // 9

#define ST7735_SLPIN  0x10      // 16
#define ST7735_SLPOUT  0x11     // 17
#define ST7735_PTLON  0x12      // 18
#define ST7735_NORON  0x13      // 19

#define ST7735_INVOFF 0x20      // 32
#define ST7735_INVON 0x21       // 33
#define ST7735_DISPOFF 0x28     // 40
#define ST7735_DISPON 0x29      // 41
#define ST7735_CASET 0x2A       // 42
#define ST7735_RASET 0x2B       // 43
#define ST7735_RAMWR 0x2C       // 44
#define ST7735_RAMRD 0x2E       // 45

#define ST7735_COLMOD 0x3A      // 58
#define ST7735_MADCTL 0x36      // 54


#define ST7735_FRMCTR1 0xB1     // 177
#define ST7735_FRMCTR2 0xB2     // 178
#define ST7735_FRMCTR3 0xB3     // 179
#define ST7735_INVCTR 0xB4      // 180
#define ST7735_DISSET5 0xB6     // 182

#define ST7735_PWCTR1 0xC0      // 192
#define ST7735_PWCTR2 0xC1      // 193
#define ST7735_PWCTR3 0xC2      // 194
#define ST7735_PWCTR4 0xC3      // 195
#define ST7735_PWCTR5 0xC4      // 196
#define ST7735_VMCTR1 0xC5      // 197

#define ST7735_RDID1 0xDA       // 218
#define ST7735_RDID2 0xDB       // 219
#define ST7735_RDID3 0xDC       // 220
#define ST7735_RDID4 0xDD       // 221

#define ST7735_PWCTR6 0xFC      // 252

#define ST7735_GMCTRP1 0xE0     // 224
#define ST7735_GMCTRN1 0xE1     // 225

// macro to convert to RGB888 (24-bit value) to RGB565(12-bit, 4096 color)
#define RGB565(r,g,b) ((((r>>3)<<11) | ((g>>2)<<5) | (b>>3)))

// some color definitions
#define colWhite RGB565( 255, 255,255)
#define colBlue RGB565( 0, 0, 255)
#define colGreen RGB565( 0,128, 0)
#define colRed RGB565(255, 0, 0)
// define your own...
#define colViolett RGB565(128, 0, 128)
#define colBlack RGB565(0, 0, 0)
#define colTurquoise RGB565(40, 185, 171)
#define colGrey RGB565(128, 128, 128)
#define colLightBlue RGB565(0, 255, 255)
#define colYellow RGB565(255, 255, 0)
#define colOrange RGB565(255, 128, 0)
#define colPink RGB565(235, 50, 113)




#define ST7735_RES_SET digitalWrite(Pin_Reset, HIGH)
#define ST7735_RES_CLR digitalWrite(Pin_Reset, LOW)

#define ST7735_CS_CLR digitalWrite(Pin_CS, LOW)
#define ST7735_CS_SET digitalWrite(Pin_CS, HIGH)



/*******************************************************************************
* Functions
*******************************************************************************/


void ST7735_WriteCommand(int i);


void ST7735_WriteData(int i);


void ST7735_WriteData16(int i);


void Display_Init();


void ST7735_Init(void);


void setAddrWindow(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1);


void SetPixel(int x, int y, int color);


void FillWithColor(int Color);


void DrawRectangle(int x, int y, int width, int height, int color);


int sgn(int x);


void gbham(int xstart, int ystart, int xend, int yend, int color);


void rasterCircle(int x0, int y0, int radius, int color);


void LCD_WriteLetter(unsigned short xpos, unsigned short ypos, unsigned char letter, unsigned short txtcolor, unsigned short txtbackcolor, unsigned char scale);


void LCD_WriteString(char string[], unsigned int x, unsigned int y, unsigned int colorFG, unsigned int colorBG, unsigned char scale);




