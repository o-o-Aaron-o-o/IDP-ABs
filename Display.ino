
#include "Display.h"
#include "fonts.h"
#include "small_fonts.h"

void ST7735_WriteCommand(int i)
{
  digitalWrite(Pin_CS, LOW); //Pin für die CS - Leitung

  digitalWrite(Pin_A0, LOW);   // Pin für die A0 - Leitung

  SPI.transfer(i);

  digitalWrite(Pin_CS, HIGH);
}



void ST7735_WriteData(int i)
{
  digitalWrite(Pin_CS, LOW);

  digitalWrite(Pin_A0, HIGH);

  SPI.transfer(i);

  digitalWrite(Pin_CS, HIGH);
}



void ST7735_WriteData16(int i)
{
  digitalWrite(Pin_CS, LOW);

  digitalWrite(Pin_A0, HIGH);

  SPI.transfer16(i);

  digitalWrite(Pin_CS, HIGH);
}



void ST7735_Init(void)
{
  ST7735_RES_SET;                                                                   // Resetleitung auf HIGH
  delay(50);
  ST7735_RES_CLR;                                                                   // Resetleitung auf LOW 
  delay(50);
  ST7735_RES_SET;                                                                   // Resetleitung wieder auf HIGH
  delay(50);

  ST7735_CS_CLR;                                                                    // CS - Leitung auf Low

  ST7735_WriteCommand(ST7735_SWRESET);                                      // software reset
  delay(150);

  ST7735_WriteCommand(ST7735_SLPOUT);                                       // out of sleep mode
  delay(50);

  ST7735_WriteCommand(ST7735_COLMOD);                                       // set color mode
  ST7735_WriteData(0x05);        // 16-bit color
  delay(10);

  ST7735_WriteCommand(ST7735_FRMCTR1);  // frame rate control - normal mode
  ST7735_WriteData(0x01);  // frame rate = fosc / (1 x 2 + 40) * (LINE + 2C + 2D)
  ST7735_WriteData(0x2C);
  ST7735_WriteData(0x2D);

  ST7735_WriteCommand(ST7735_FRMCTR2);  // frame rate control - idle mode
  ST7735_WriteData(0x01);  // frame rate = fosc / (1 x 2 + 40) * (LINE + 2C + 2D)
  ST7735_WriteData(0x2C);
  ST7735_WriteData(0x2D);

  ST7735_WriteCommand(ST7735_FRMCTR3);  // frame rate control - partial mode
  ST7735_WriteData(0x01); // dot inversion mode
  ST7735_WriteData(0x2C);
  ST7735_WriteData(0x2D);
  ST7735_WriteData(0x01); // line inversion mode
  ST7735_WriteData(0x2C);
  ST7735_WriteData(0x2D);

  ST7735_WriteCommand(ST7735_INVCTR);  // display inversion control
  ST7735_WriteData(0x07);  // no inversion

  ST7735_WriteCommand(ST7735_PWCTR1);  // power control
  ST7735_WriteData(0xA2);
  ST7735_WriteData(0x02);      // -4.6V
  ST7735_WriteData(0x84);      // AUTO mode

  ST7735_WriteCommand(ST7735_PWCTR2);  // power control
  ST7735_WriteData(0xC5);      // VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD

  ST7735_WriteCommand(ST7735_PWCTR3);  // power control
  ST7735_WriteData(0x0A);      // Opamp current small
  ST7735_WriteData(0x00);      // Boost frequency

  ST7735_WriteCommand(ST7735_PWCTR4);  // power control
  ST7735_WriteData(0x8A);      // BCLK/2, Opamp current small & Medium low
  ST7735_WriteData(0x2A);

  ST7735_WriteCommand(ST7735_PWCTR5);  // power control
  ST7735_WriteData(0x8A);                                                                   // BCLK/2, Opamp current small & Medium low
  ST7735_WriteData(0xEE);

  ST7735_WriteCommand(ST7735_VMCTR1);  // power control
  ST7735_WriteData(0x0E);

  ST7735_WriteCommand(ST7735_INVOFF);    // don't invert display

  ST7735_WriteCommand(ST7735_MADCTL);  // memory access control (directions)
  
  // normal R G B order
  // Display orientation
  //ST7735_WriteData(0x00);  // portrait
  //ST7735_WriteData(0xC0);  // portrait, upsidedown
    ST7735_WriteData(0xA0);    // landscape upper left corner is 0,0 
  //ST7735_WriteData(0xE0); // landscape, mirrored lower left corner is 0,0
 // ST7735_WriteData(0x60);  // landscape, inverted lower right corner is 0,0

  ST7735_WriteCommand(ST7735_COLMOD);  // set color mode
  ST7735_WriteData(0x05);        // 16-bit color

  ST7735_WriteCommand(ST7735_CASET);  // column addr set
  ST7735_WriteData(0x00);
  ST7735_WriteData(0x00);   // XSTART = 0
  ST7735_WriteData(0x00);
  ST7735_WriteData(0x7F);   // XEND = 127

  ST7735_WriteCommand(ST7735_RASET);  // row addr set
  ST7735_WriteData(0x00);
  ST7735_WriteData(0x00);    // XSTART = 0
  ST7735_WriteData(0x00);
  ST7735_WriteData(0x9F);    // XEND = 159

  ST7735_WriteCommand(ST7735_GMCTRP1);
  ST7735_WriteData(0x0f);
  ST7735_WriteData(0x1a);
  ST7735_WriteData(0x0f);
  ST7735_WriteData(0x18);
  ST7735_WriteData(0x2f);
  ST7735_WriteData(0x28);
  ST7735_WriteData(0x20);
  ST7735_WriteData(0x22);
  ST7735_WriteData(0x1f);
  ST7735_WriteData(0x1b);
  ST7735_WriteData(0x23);
  ST7735_WriteData(0x37);
  ST7735_WriteData(0x00);
  ST7735_WriteData(0x07);
  ST7735_WriteData(0x02);
  ST7735_WriteData(0x10);
  ST7735_WriteCommand(ST7735_GMCTRN1);
  ST7735_WriteData(0x0f);
  ST7735_WriteData(0x1b);
  ST7735_WriteData(0x0f);
  ST7735_WriteData(0x17);
  ST7735_WriteData(0x33);
  ST7735_WriteData(0x2c);
  ST7735_WriteData(0x29);
  ST7735_WriteData(0x2e);
  ST7735_WriteData(0x30);
  ST7735_WriteData(0x30);
  ST7735_WriteData(0x39);
  ST7735_WriteData(0x3f);
  ST7735_WriteData(0x00);
  ST7735_WriteData(0x07);
  ST7735_WriteData(0x03);
  ST7735_WriteData(0x10);

  ST7735_WriteCommand(0xF6); //Disable ram power save mode
  ST7735_WriteData(0x00);

  ST7735_WriteCommand(ST7735_DISPON);
  delay(100);

  ST7735_WriteCommand(ST7735_NORON);  // normal display on
  delay(10);
  ST7735_CS_SET;                                                                            // CS - Leitung auf HIGH
}



void setAddrWindow(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1)
{
  ST7735_WriteCommand(ST7735_CASET);  // column addr set
  ST7735_WriteData(0x00);
  ST7735_WriteData(x0+0);   // XSTART
  ST7735_WriteData(0x00);
  ST7735_WriteData(x1+0);   // XEND

  ST7735_WriteCommand(ST7735_RASET);  // row addr set
  ST7735_WriteData(0x00);
  ST7735_WriteData(y0+0);    // YSTART
  ST7735_WriteData(0x00);
  ST7735_WriteData(y1+0);    // YEND

  ST7735_WriteCommand(ST7735_RAMWR);  // write to RAM
}



void SetPixel(int x, int y, int color)
{
  setAddrWindow(x, y, x, y);
  ST7735_WriteData16(color);
}



void FillWithColor(int Color)
{
  setAddrWindow(0, 0, SCREEN_HEIGHT, SCREEN_WIDTH);
  
  for(int i = 0; i <= SCREEN_HEIGHT * SCREEN_WIDTH; i++)
  {
    ST7735_WriteData16(Color);
  }

}



void DrawRectangle(int x, int y, int width, int height, int color)
{
  setAddrWindow(x, y, x + width, y + height);

  for(int i = 0; i <= (height + y) * (width + x); i++)
  {
    ST7735_WriteData16(color);
  }
}



/* signum function (Vorzeichen) */
int sgn(int x)
{
    if (x > 0) return +1;
    if (x < 0) return -1;
    return 0;
}


/*----------------- Bresenham - Linien - Algorithmus --------------------------*/

void gbham(int xstart, int ystart, int xend, int yend, int color)
/*--------------------------------------------------------------
 * Bresenham-Algorithmus: Linien auf Rastergeräten zeichnen
 *
 * Eingabeparameter:
 *    int xstart, ystart        = Koordinaten des Startpunkts
 *    int xend, yend            = Koordinaten des Endpunkts
 *
 * Ausgabe:                                                                           <---------- aaaaaaaaaaaaaaaaaaaaaaaAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAaaaaaaaaaaaaaaaaaaaaa
 *    void SetPixel(int x, int y) setze ein Pixel in der Grafik
 *         (wird in dieser oder aehnlicher Form vorausgesetzt)
 *---------------------------------------------------------------
 */
{
    int x, y, t, dx, dy, incx, incy, pdx, pdy, ddx, ddy, deltaslowdirection, deltafastdirection, err;

    /* Entfernung in beiden Dimensionen berechnen */
    dx = xend - xstart;
    dy = yend - ystart;

    /* Vorzeichen des Inkrements bestimmen */
    incx = sgn(dx);
    incy = sgn(dy);
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;

    /* feststellen, welche Entfernung größer ist */
    if (dx > dy)
    {
        /* x ist schnelle Richtung */
        pdx = incx; pdy = 0;    /* pd. ist Parallelschritt */
        ddx = incx; ddy = incy; /* dd. ist Diagonalschritt */
        deltaslowdirection = dy;   deltafastdirection = dx;   /* Delta in langsamer Richtung, Delta in schneller Richtung */
    }
    else
    {
        /* y ist schnelle Richtung */
        pdx = 0;    pdy = incy; /* pd. ist Parallelschritt */
        ddx = incx; ddy = incy; /* dd. ist Diagonalschritt */
        deltaslowdirection = dx;   deltafastdirection = dy;   /* Delta in langsamer Richtung, Delta in schneller Richtung */
    }

    /* Initialisierungen vor Schleifenbeginn */
    x = xstart;
    y = ystart;
    err = deltafastdirection / 2;
    SetPixel(x,y, color);                                                                                                            // <----- aaaaaaaaaaaaaaaaaaAAAAAAAAAAAAAAAAAaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

    /* Pixel berechnen */
    for(t = 0; t < deltafastdirection; ++t) /* t zaehlt die Pixel, deltafastdirection ist Anzahl der Schritte */
    {
        /* Aktualisierung Fehlerterm */
        err -= deltaslowdirection;
        if(err < 0)
        {
            /* Fehlerterm wieder positiv (>=0) machen */
            err += deltafastdirection;
            /* Schritt in langsame Richtung, Diagonalschritt */
            x += ddx;
            y += ddy;
        }
        else
        {
            /* Schritt in schnelle Richtung, Parallelschritt */
            x += pdx;
            y += pdy;
        }
        SetPixel(x, y, color);
    }
}



void rasterCircle(int x0, int y0, int radius, int color)
{
    int f = 1 - radius;                     
    int ddF_x = 0;                          
    int ddF_y = -2 * radius;                
    int x = 0;                              
    int y = radius;                        
                                              
    SetPixel(x0, y0 + radius, color);       
    SetPixel(x0, y0 - radius, color);
    SetPixel(x0 + radius, y0, color);
    SetPixel(x0 - radius, y0, color);

    while(x < y)
    {
        if (f >= 0)
        {
            y -= 1;
            ddF_y += 2;
            f += ddF_y;
        }
        x += 1;
        ddF_x += 2;
        f += ddF_x + 1;

        SetPixel(x0 + x, y0 + y, color);
        SetPixel(x0 - x, y0 + y, color);
        SetPixel(x0 + x, y0 - y, color);
        SetPixel(x0 - x, y0 - y, color);
        SetPixel(x0 + y, y0 + x, color);
        SetPixel(x0 - y, y0 + x, color);
        SetPixel(x0 + y, y0 - x, color);
        SetPixel(x0 - y, y0 - x, color);
    }
}




/*---------------- Function to write a letter of choice on the SPI-Display --------------------------------------------*/

void LCD_WriteLetter(unsigned short xpos, unsigned short ypos, unsigned char letter, unsigned short txtcolor, unsigned short txtbackcolor, unsigned char scale)
{

  switch(scale)                                                             // Switch-case, dass die Skalierung von dem Buchstaben bestimmt
  {
    case 0: setAddrWindow(xpos, ypos, xpos + 5, ypos + 7);
            break;

    case 1: setAddrWindow(xpos, ypos, xpos + 11, ypos + 15);
            break;

    case 2: setAddrWindow(xpos, ypos, xpos + 23, ypos + 31);
            break;

    case 3: setAddrWindow(xpos, ypos, xpos + 35, ypos + 47);
            break;

    default:  
            break;
  }
  

  if(scale != 0)
  {
    int i = 0;                                                                // Hilfsvariable, die der folgenden Schleife ein Ende gibt
    int Zeilenanzahl = 0;                                                     // Hilfsvariable, die der Skalierung der Buchstaben die Anzahl der gemalten Zeilen speichert

    while(i < 32)                                                             // while Schleife die die Array-Inhalte durchläuft --> I wird +2 gerechnet um immer in eine neue Zeile im der Array Tabelle zu springen
    {
      unsigned short Links = console_font_12x16[32 * letter + i];             // Linke Seite einer Zeile des Buchstaben aus dem fonts Array 
      unsigned short Rechts = console_font_12x16[32 * letter + i + 1];        // Rechte Seite einer Zeile des Buchstaben aus dem fonts Array

      unsigned short Ziel = (Links << 8) | Rechts;                            // Veknüpfen der zwei 8bit Zahlen zu einer 16bit Zahl                             

      unsigned short Maske = 0b1000000000000000;


      for(int Spalte = 0; Spalte <= 11; Spalte++)                             // for Schleife die die Bits einer Zeile durchläuft
      {
        unsigned short MaskeSchieb = Maske >> Spalte;                         // Schieben der Maske zur der passende Spalte

    
        if((Ziel & MaskeSchieb) != 0)                                         // Wenn ein Bit in der Zeile eine 1 ist, wird die Textfarbe gemalt 
        {
          for(int j = 1; j <= scale; j++)                                     // For-Schleife, die so viele Pixel in der Textfarbe bemalt wie die Skalierung vorgibt
            ST7735_WriteData16(txtcolor);
        }                           
        else                                                                  // Wenn ein Bit in der Zeile eine 0 ist, wird die Hintergurndfarbe gemalt
        {
          for(int j = 1; j <= scale; j++)                                     // For-Schleife, die so viele Pixel in der Hintergrundfarbe bemalt wie die Skalierung vorgibt
            ST7735_WriteData16(txtbackcolor);
        }

      }

      Zeilenanzahl++;

      if(Zeilenanzahl == scale)                                               // Sobald die Anzahl der gemalten Zeilen der Skalierung entspricht, wird in die nächte Zeile des Buchstaben übergegangen
      {
        i = i + 2;
        Zeilenanzahl = 0;
      }
        

    }
  }
  else
  {
    for(int i = 0; i <= 7; i++)
    {
      unsigned char Zeile = console_font_6x8[8 * letter + i];

      unsigned char Maske = 0b10000000;

      for(int Spalte = 0; Spalte <= 5; Spalte++)
      {
        unsigned char MaskeSchieb = Maske >> Spalte;

        if((Zeile & MaskeSchieb) != 0)                                                                            
          ST7735_WriteData16(txtcolor);
        
        else                                                                                                   
          ST7735_WriteData16(txtbackcolor);

      }

    }
    
  }
  
}





// char Hallo[] = {'H', 'a', 'l', 'l', 'o'};


void LCD_WriteString(char string[], unsigned int x, unsigned int y, unsigned int txtcolor, unsigned int txtbackcolor, unsigned char scale)
{

  int i = 0, new_line_Letter = 0, how_often_newline = 0;                                                                                        //string variable
  int Lbreite, LHoehe;

  if(scale == 0)
  {
    Lbreite = 6;                                                                                   //Letter Breite 6x8 bei ganz klein
    LHoehe = 8;
  }                                                                                   
  else if (scale == 1)
  {
    Lbreite = 12;                                                                                    // 12x 16
    LHoehe = 16;
  }
  else if (scale == 2)
  {
    Lbreite = 24;
    LHoehe = 32;
  }
  else
  {
    Lbreite = 36;
    LHoehe = 48;
  }
   

   
  while( string[i] != 0 )
  {
    if((string[i] == '\n') || (new_line_Letter >= 1))
    {
      if(string[i] == '\n')
      {
        string[i] = 0;
        new_line_Letter = 0;
        how_often_newline++;
      }
      LCD_WriteLetter(x + (Lbreite * new_line_Letter), y + LHoehe * how_often_newline, string[i], txtcolor, txtbackcolor, scale);
      new_line_Letter++;
    }
    else
      LCD_WriteLetter(x + (Lbreite * i), y, string[i], txtcolor, txtbackcolor, scale);                                    //Anfangx, Anfangy, String Anfang, Farben, und Scallierung

    i++;
  }
 
}






