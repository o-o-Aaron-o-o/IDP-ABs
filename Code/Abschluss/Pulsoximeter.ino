
// Hardware Connections (Breakoutboard to Arduino):
/*// -5V = 5V (3.3V is allowed)
  // -GND = GND
  // -SDA = A4 (or SDA)
  // -SCL = A5 (or SCL)
  // -INT = Not connected
*/






void Oxi_Init()
{
  if (!Pulsoxi.begin(Wire, I2C_SPEED_FAST))                          // Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }

  //Setup to sense a nice looking saw tooth on the plotter

  byte ledBrightness = 0x32;                                       //Options: 0=Off to 255=50mA
  byte sampleAverage = 8;                                          //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2;                                                //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  int sampleRate = 3200;                                           //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411;                                            //Options: 69, 118, 215, 411
  int adcRange = 4096;                                             //Options: 2048, 4096, 8192, 16384

  Pulsoxi.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);          //Configure sensor with these settings

}




void loop_oxi() 
{
  //static int Array_IR[BUFFER_SIZE_OXI] = {0}, Array_R[BUFFER_SIZE_OXI] = {0};//Arrays, die für das Zeichnen der Sensordaten genutzt werden 
  //static int Del_IR[BUFFER_SIZE_OXI] = {0}, Del_R[BUFFER_SIZE_OXI] = {0};    //Arrays, die für das Löschen der Zeichnung der Sensordaten genutzt werden 

  static float DC_IR = 0.0, DC_R = 0.0;                                     //Variablen für die DC-Anteile des Oxi-Signals
  static float AC_IR = 0.0, AC_R = 0.0, Z = 1.00;                           //Variablen für die DC-Anteile des Oxi-Signals und die Z - Zwischenwertvariable für die Formel der Sauerstoffsätigung
  static int BpmFlag = 0;                                                   //Variable, die bei der Peakdetection hilft
  static int ArrayFuellung = 0;                                             //Variabe, die zählt wie weit die Zeichen-arrays aufgefüllt sind                                          



  float letztes_DC_IR = 0.0, letztes_DC_R = 0.0;                                  
  float letztes_AC_IR = 0.0, letztes_AC_R = 0.0;


  bool Flag_Sauerstoff_OK = false;



  static char SauerstoffsaettigungsBuchstaben[50] = {0};                    //Array, womit die SpO2- und Bpm-Daten auf dem Display ausgegeben werden  





  int IR_Messung = Pulsoxi.getIR();                                         //Erfassen des Oxi-Infrarot-Signals

  letztes_DC_IR = DC_IR;

  DC_IR = (1 - GLATTUNG_IR) * DC_IR + GLATTUNG_IR * IR_Messung;             //Berechnen des DC-Wertes / Mittelwertes des Infrarot-Signals des Oximeters


  IR_Messung -= DC_IR;                                                      //Rausnahme des Mittelwertes (DC-Wertes) aus dem Infrarot-Signals


  letztes_AC_IR = AC_IR;

  AC_IR = sqrt(GLATTUNG_IR * pow(IR_Messung, 2) + (1 - GLATTUNG_IR) * pow(AC_IR, 2));//Berechnung des AC-Wertes des Infrarot-Signals


  Serial.print(" RohIR:");  	                                              //
  Serial.print(IR_Messung);                                                 //
  Serial.print(" DCIR:");                                                   //
  Serial.print(DC_IR);                                                      // DEBUGGING
  Serial.print(" ACIR:");                                                   //
  Serial.print(AC_IR);                                                      //




  int R_Messung = Pulsoxi.getRed();                                         //Erfassen des Oxi-Infrarot-Signals

  letztes_DC_R = DC_R;
  
  DC_R = (1 - GLATTUNG_R) * DC_R + GLATTUNG_R * R_Messung;                  //Berechnen des DC-Wertes / Mittelwertes des Rot-Signals des Oximeters


  R_Messung -= DC_R;                                                        //Rausnahme des Mittelwertes (DC-Wertes) aus dem Rot-Signals


  letztes_AC_R;

  AC_R = sqrt(GLATTUNG_R * pow(R_Messung, 2) + (1 - GLATTUNG_R) * pow(AC_R, 2));//Berechnung des AC-Wertes des Rot-Signals
  
 
  Serial.print(" RohR:");  	                                                //
  Serial.print(R_Messung);   	                                              //
  Serial.print(" DCR:");  	                                                //
  Serial.print(DC_R);  	                                                    // DEBUGGING
  Serial.print(" ACR:");  	                                                //
  Serial.print(AC_R);  	                                                    //

  if((AC_R * DC_IR) != 0) 
    Z = (AC_IR * DC_R) / (AC_R * DC_IR);                                    //Berechnung des Z-Wertes für die Formel der Sauerstoffsättigung

  float SpO2 = (-45.06*Z + 30.354)*Z + 94.845;                              //Berechnen der Sauerstoffsätigung



  //Konfidenzintervalle, ob Finger drauf ist
  if((abs(DC_IR - letztes_DC_IR) <= 100) && (abs(DC_R - letztes_DC_R) <= 100) && (SpO2 > 85) && (IR_Messung + DC_IR >= 50000)) 
    Flag_Sauerstoff_OK = true;
  else
    Flag_Sauerstoff_OK = false;


  Serial.print("            Z: ");                                          //
  Serial.print(Z);                                                          // DEBUGGING
  Serial.print("          SPO2: ");                                         //
  Serial.print(SpO2);                                                       //



  if(Flag_Sauerstoff_OK)                                                    //Schauen ob die Signalwerte in einem logischen Rahmen, für die Berechnung der Bpm und der Zeichnung sind
  {


    //Line_IRed(DC_IR);
    Line_Red(DC_R);


    // BPM !!!!!!!!!



  }



  if(Flag_Sauerstoff_OK)
  {
    snprintf(SauerstoffsaettigungsBuchstaben, sizeof(SauerstoffsaettigungsBuchstaben), "SpO2: % .1f  Bpm: -- ", SpO2); //Reinschreiben des Textes und der Werte in das Wortarray
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDR, HIGH);
  }
  else
  {
    snprintf(SauerstoffsaettigungsBuchstaben, sizeof(SauerstoffsaettigungsBuchstaben), "SpO2: --  Bpm: -- "); //Reinschreiben des Textes und der Werte in das Wortarray
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, HIGH);
  }

  LCD_WriteString(SauerstoffsaettigungsBuchstaben, 10, 110, colViolett, colLightBlue, 0); //Zeichnen des Wortarrays


  





  Serial.println();
}



//Hier Max und Min Funktion
/*
  int FindMax(int maxArray[])
  {
    int maximum = maxArray[0];

    for(int i = 1; i < BUFFER_SIZE_OXI; i++)
    {
      if (maxArray[i] > maximum) 
        maximum = maxArray[i];
    }
      

    return maximum;

  }



  int FindMin(int minArray[])
  {
    int minimum = minArray[0];

    for(int j = 1; j < BUFFER_SIZE_OXI; j++)
    {
      if (minArray[j] <= minimum) 
        minimum = minArray[j];
    }
      

    return minimum;

  }

*/




void Line_IRed(int hi)
{
    static int MaX = hi + 4500;
    static int MiN = hi - 4500;
    static int del[160] = {50};   // Used to save the drawn values, in order to reuse them for the deletion of the last drawn line
    static int counter = 0;
    static int lastPixel = 50;

    if (hi > MaX)
    {
        MiN = MiN + (hi - MaX);
        MaX = hi;
    }
  
    if (hi < MiN)
    {
        MaX = MaX - (MiN - hi);
        MiN = hi;
    }


    gbham(counter, del[counter], counter + 1, del[counter + 1], colLightBlue);



    int Pixel1 = map(hi, MiN, MaX, 100, 0);

    gbham(counter, lastPixel, counter + 1, Pixel1, colBlack);

    del[counter+1] = Pixel1;
    del[counter] = lastPixel;
    lastPixel = Pixel1;

    counter = (counter + 1) % 159;

}


void Line_Red(int hi)
{
    static int MaX = hi + 4500;
    static int MiN = hi - 4500;
    static int del[160] = {50};   // Used to save the drawn values, in order to reuse them for the deletion of the last drawn line
    static int counter = 0;
    static int lastPixel = 50;

    if (hi > MaX)
    {
        MiN = MiN + (hi - MaX);
        MaX = hi;
    }
  
    if (hi < MiN)
    {
        MaX = MaX - (MiN - hi);
        MiN = hi;
    }


    gbham(counter, del[counter], counter + 1, del[counter + 1], colLightBlue);



    int Pixel1 = map(hi, MiN, MaX, 100, 0);

    gbham(counter, lastPixel, counter + 1, Pixel1, colBlack);

    del[counter+1] = Pixel1;
    del[counter] = lastPixel;
    lastPixel = Pixel1;

    counter = (counter + 1) % 159;
}




