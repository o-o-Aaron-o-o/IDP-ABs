
  // Hardware Connections (Breakoutboard to Arduino):
  // -5V = 5V (3.3V is allowed)
  // -GND = GND
  // -SDA = A4 (or SDA)
  // -SCL = A5 (or SCL)
  // -INT = Not connected







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
  static unsigned int Array_IR[BUFFER_SIZE_OXI] = {0}, Array_R[BUFFER_SIZE_OXI] = {0};          //Arrays, die für das Zeichnen der Sensordaten genutzt werden 
  static unsigned int Del_IR[BUFFER_SIZE_OXI] = {0}, Del_R[BUFFER_SIZE_OXI] = {0};              //Arrays, die für das Löschen der Zeichnung der Sensordaten genutzt werden 

  static float DC_IR = 0.0, DC_R = 0.0;                                  //Variablen für die DC-Anteile des Oxi-Signals
  static float AC_IR = 0.0, AC_R = 0.0, Z = 1.00;                        //Variablen für die DC-Anteile des Oxi-Signals und die Z - Zwischenwertvariable für die Formel der Sauerstoffsätigung
  static int ArrayFuellung = 0;                                          //Variabe, die zählt wie weit die Zeichen-arrays aufgefüllt sind                                          
  static int BpmFlag = 0;                                                //Variable, die bei der Peakdetection hilft


  float letztes_DC_IR = 0.0, letztes_DC_R = 0.0;                                  
  float letztes_AC_IR = 0.0, letztes_AC_R = 0.0;


  bool Flag_Sauerstoff_OK = false;



  static char SauerstoffsaettigungsBuchstaben[50] = {0};                 //Array, womit die SpO2- und Bpm-Daten auf dem Display ausgegeben werden  





  int IR_Messung = Pulsoxi.getIR();                                      //Erfassen des Oxi-Infrarot-Signals

  letztes_DC_IR = DC_IR;

  DC_IR = (1 - GLATTUNG_IR) * DC_IR + GLATTUNG_IR * IR_Messung;          //Berechnen des DC-Wertes / Mittelwertes des Infrarot-Signals des Oximeters


  IR_Messung -= DC_IR;                                                   //Rausnahme des Mittelwertes (DC-Wertes) aus dem Infrarot-Signals


  letztes_AC_IR = AC_IR;

  AC_IR = sqrt(GLATTUNG_IR * pow(IR_Messung, 2) + (1 - GLATTUNG_IR) * pow(AC_IR, 2));     //Berechnung des AC-Wertes des Infrarot-Signals


  Serial.print(" RohIR:");  	                                            //
  Serial.print(IR_Messung);                                               //
  Serial.print(" DCIR:");                                                 //
  Serial.print(DC_IR);                                                    // DEBUGGING
  Serial.print(" ACIR:");                                                 //
  Serial.print(AC_IR);                                                    //




  int R_Messung = Pulsoxi.getRed();                                       //Erfassen des Oxi-Infrarot-Signals

  letztes_DC_R = DC_R;
  
  DC_R = (1 - GLATTUNG_R) * DC_R + GLATTUNG_R * R_Messung;                //Berechnen des DC-Wertes / Mittelwertes des Rot-Signals des Oximeters


  R_Messung -= DC_R;                                                      //Rausnahme des Mittelwertes (DC-Wertes) aus dem Rot-Signals


  letztes_AC_R;

  AC_R = sqrt(GLATTUNG_R * pow(R_Messung, 2) + (1 - GLATTUNG_R) * pow(AC_R, 2));      //Berechnung des AC-Wertes des Rot-Signals
  
 
  Serial.print(" RohR:");  	                                            //
  Serial.print(R_Messung);   	                                          //
  Serial.print(" DCR:");  	                                            //
  Serial.print(DC_R);  	                                                // DEBUGGING
  Serial.print(" ACR:");  	                                            //
  Serial.print(AC_R);  	                                                //

  if((AC_R * DC_IR) != 0) 
    Z = (AC_IR * DC_R) / (AC_R * DC_IR);                                                //Berechnung des Z-Wertes für die Formel der Sauerstoffsättigung

  float SpO2 = (-45.06*Z + 30.354)*Z + 94.845;                          //Berechnen der Sauerstoffsätigung




  //Konfidenzintervalle, ob Finger drauf ist
  if((abs(DC_IR - letztes_DC_IR) <= 100) && (abs(DC_R - letztes_DC_R) <= 100) && (SpO2 > 85)) 
    Flag_Sauerstoff_OK = true;
  else
    Flag_Sauerstoff_OK = false;


  Serial.print("            Z: ");                                      //
  Serial.print(Z);                                                      // DEBUGGING
  Serial.print("          SPO2: ");                                     //
  Serial.println(SpO2);                                                 //



  if((R_Messung < 2000) && (IR_Messung < 2000))                         //Schauen ob die Signalwerte in einem logischen Rahmen, für die Berechnung der Bpm und der Zeichnung sind
  {
    static float Bpm = 0.0;                                             //Variable, die die Bpm speichern soll

    Array_R[ArrayFuellung] = DC_R;                                 //Auffüllen des Zeichen-Arrays für die Werte des Rot-Signals
    Array_IR[ArrayFuellung] = DC_IR;                               //Auffüllen des Zeichen-Arrays für die Werte des Infrarot-Signals
    ArrayFuellung++;

    if((R_Messung >= 1300) && (BpmFlag == 0))                           //Schauen, ob gerade ein Peak passiert
    {
      static float DeltaT = 100;                                          //Variable, die einen Mittelwert der Zeitabstände zwischen Peaks speichert
      static int t1 = millis();                                         //Zeitstempelvariable, die zur Berechnung der Abstände hilft
      BpmFlag = 1;                                                      //Setzten der Flagvariable auf True

      unsigned int MachtKeinenSinn_1 = millis() - t1; 	                  //Hilfsvariable, die zur Berechnung des Delta T hilft
      float MachtKeinenSinn_2 = 0.95 * DeltaT;
      float MachtKeinenSinn_3 = 0.05 * MachtKeinenSinn_1;

      DeltaT = MachtKeinenSinn_2 + MachtKeinenSinn_3;               //Berechnung, des Mittelwerts der Zeitabstände zwischen Peaks

      t1 = millis();                                                    //Erneuerung der Zeitstempel-Hilfsvariable

      if(DeltaT != 0)
        Bpm = 60000.0 / DeltaT;                                           //Berechnung der Bpm
    }

    if((R_Messung <= 1300) && (BpmFlag == 1))                           //Schauen, ob der Peak vorbei ist
      BpmFlag = 0;                                                      //Setzten der Flagvariable auf False

    
    if(Flag_Sauerstoff_OK == true)
    {
      snprintf(SauerstoffsaettigungsBuchstaben, sizeof(SauerstoffsaettigungsBuchstaben), "SpO2: % .1f  Bpm: % .f ", SpO2, Bpm); //Reinschreiben des Textes und der Werte in das Wortarray
      digitalWrite(LEDG, LOW);
      digitalWrite(LEDR, HIGH);
    }
    else
    {
      snprintf(SauerstoffsaettigungsBuchstaben, sizeof(SauerstoffsaettigungsBuchstaben), "SpO2: --  Bpm: -- "); //Reinschreiben des Textes und der Werte in das Wortarray
      digitalWrite(LEDR, LOW);
      digitalWrite(LEDG, HIGH);
    }

    LCD_WriteString(SauerstoffsaettigungsBuchstaben, 10, 110, colViolett, colLightBlue, 0);                                   //Zeichnen des Wortarrays


  }





  if(ArrayFuellung >= 159)                                              //Schauen, ob die Zeichen-Array aufgefüllt wurden
  {
    ArrayFuellung = 0;                                                  //Resetten der Auffüll-Variable

    int Max_IR = FindMax(Array_IR);
    int Min_IR = FindMin(Array_IR);
    int Max_R = FindMax(Array_R);
    int Min_R = FindMin(Array_R);

    for(int Pix = 0; Pix < 160; Pix++)                                  //Schleife, die die Arrays zeichnen soll
    {
      gbham(Pix, Del_IR[Pix], Pix+1, Del_IR[Pix+1], colLightBlue);      //Bresenham-Linienalgorithmus, der die Linie des Infrarot-Signals löscht
      gbham(Pix, Del_R[Pix], Pix+1, Del_R[Pix+1], colLightBlue);        //Bresenham-Linienalgorithmus, der die Linie des Rot-Signals löscht

      int IR_1 = map(Array_IR[Pix], Max_IR, Min_IR, 100, 20);              //Variable, in die der erste skalierte Wert für die Linienzeichnung des Infrarot-Signals gespeichert wird 
      int IR_2 = map(Array_IR[Pix+1], Max_IR, Min_IR, 100, 20);            //Variable, in die der zweite skalierte Wert für die Linienzeichnung des Infrarot-Signals gespeichert wird 
      int R_1 = map(Array_R[Pix], Max_R, Min_R, 100, 20);                //Variable, in die der erste skalierte Wert für die Linienzeichnung des Rot-Signals gespeichert wird 
      int R_2 = map(Array_R[Pix+1], Max_R, Min_R, 100, 20);              //Variable, in die der zweite skalierte Wert für die Linienzeichnung des Rot-Signals gespeichert wird 

      IR_1 = constrain(IR_1, 20, 100);                                  // 
      IR_2 = constrain(IR_2, 20, 100);                                  //Nochmalige Prüfung, ob die skalierten Werte im richtigen Rahmen liegen
      R_1 = constrain(R_1, 20, 100);                                    //
      R_2 = constrain(R_2, 20, 100);                                    //
      

      Del_IR[Pix] = IR_1;                                               //Speichern des gezeichneten Werts, in das Lösch-Array des Infrarot-Signals
      Del_R[Pix] = R_1;                                                 //Speichern des gezeichneten Werts, in das Lösch-Array des Rot-Signals

      gbham(Pix, IR_1, Pix+1, IR_2, colRed);                            //Bresenham-Linienalgorithmus, der die Linie des Infrarot-Signals zeichnet
      gbham(Pix, R_1, Pix+1, R_2, colBlack);                            //Bresenham-Linienalgorithmus, der die Linie des Rot-Signals zeichnet
    }

  }

}





int FindMax(unsigned int Array[])
{
  unsigned int max = Array[0];

  for(int i = 0; i <= BUFFER_SIZE_OXI; i++)
  {
    if (Array[i] > max) 
      max = Array[i];
  }
    

  return max;

}



int FindMin(unsigned int Array[])
{
  unsigned int min = Array[0];

  for(int i = 80; i <= BUFFER_SIZE_OXI; i++)
  {
    if (Array[i] < min) 
      min = Array[i];
  }
    

  return min;

}


