
#include "pitches.h"






void EKG_Init()
{
  pinMode(ECG_OUTPUT_SIGNAL, INPUT);
  pinMode(LO_MINUS, INPUT);                                             // Leads Off Comparator Output. In dc leads off detection mode, LOD− is high when the electrode to −IN is disconnected, and it is low when connected. 
  pinMode(LO_PLUS, INPUT);                                              // Leads Off Comparator Output. In dc leads off detection mode, LOD+ is high when the +IN electrode is disconnected, and it is low when connected.
  pinMode(SDN, OUTPUT);                                                 // Shutdown Control Input. Drive SDN low to enter the low power shutdown mode.
  digitalWrite(SDN, HIGH);

  pinMode(LEDR, OUTPUT);
  digitalWrite(LEDR, HIGH);
  pinMode(LEDG, OUTPUT);
  digitalWrite(LEDG, HIGH);
}









void loop_ekg() 
{
  
  static char ecg[] = "This is your ECG - Reading";
  char Bpm[50] = {0};
  
  static unsigned int ECG_Measurements[BUFFER_SIZE_EKG] = {0};
  static unsigned int Time_Array[BUFFER_SIZE_EKG] = {0};
  unsigned int Peak_Time_Array[5] = {0};
  unsigned int Peak_Array[5] = {0};

  static unsigned int Del_Array[160] = {0};

  

  static unsigned int Warning = 0, Pixel1 = 0, Pixel2 = 0, min = 0, max = 0, max_min = 0, field;



  if((digitalRead(LO_MINUS)) || (digitalRead(LO_PLUS)))
  {
    static char warning[] = "Cables not connected!";
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDR, LOW);

    LCD_WriteString(warning, 10, 60, colBlack, colWhite, 0);
    delay(200);

    FillWithColor(colLightBlue);
    Warning = 1;
  }
  else
  {
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, LOW);
    Warning = 0;
  }

  if(Warning == 0)
  {
    for(int i = 0; i < BUFFER_SIZE_EKG; i++)
    {
      ECG_Measurements[i] = Average_and_Scaling();
      Time_Array[i] = millis();
    }


    int j = 1, n = 0, flag = 1;


    while(j < BUFFER_SIZE_EKG)
    {
      if((ECG_Measurements[j] < 800) && (flag == 1))
        flag = 0;
      

      if((ECG_Measurements[j-1] <= ECG_Measurements[j]) && (ECG_Measurements[j] > ECG_Measurements[j+1]) && (ECG_Measurements[j] >= 800) && (n < 5) && (flag == 0))
      {
        Peak_Array[n] = j;
        Peak_Time_Array[n] = Time_Array[j];
        n++;
        flag = 1;
      }

      j++;

    }

    float deltaT = 0.0, delta_Array[5] = {0.0};
    float mittleresDelta = 0.0;
    float bpm = 0.0;

    Serial.println();
    Serial.println();

    for(int ndi = 0; ndi < 5; ndi++)
    {
      Serial.print(Peak_Time_Array[ndi]);
      Serial.print("   ");
      
      if(ndi == 4)
      {
        Serial.print("  Peaks  ");
        Serial.print(n);
      }
    }

    if(n >= 2)
    {
      deltaT = (Peak_Time_Array[n-1] - Peak_Time_Array[0]) / (n-1);

      bpm = 60000.0 / deltaT;
      
    }

    Serial.println();
    Serial.println();
    Serial.print("MitDelT:   ");
    Serial.print(mittleresDelta);
    Serial.print("   DT:  ");
    Serial.print(deltaT);
    Serial.print("   BPM: ");
    Serial.println(bpm);
    
    
    // for(int o = 0; o < 5; o++)
    // {
      
    //   Serial.println();
    //   Serial.print("Peak at:  ");
    //   Serial.print(Peak_Array[o]);    
    //   Serial.print("   Delta:  ");
    //   Serial.print(delta_Array[o]);     

    // }

    snprintf(Bpm, sizeof(Bpm), "  Your Pulse: % .1f Bpm", bpm);

    LCD_WriteString(ecg, 2, 10, colViolett, colLightBlue, 0);


    field = FindMaxPosition(ECG_Measurements);                                                  // Find the QRS Complex / the location of the max value
    min = FindMinPosition(ECG_Measurements);

    min = ECG_Measurements[min];
    max = ECG_Measurements[field];
    max_min = max - min;


    for(int i = 0; i < 159; i++)
    {

      Pixel1 = ((ECG_Measurements[field - 80 + i] - min) * 128) / max_min;
      Pixel2 = ((ECG_Measurements[field - 80 + i + 1] - min) * 128) / max_min;

      // Pixel1 = ECG_Measurements[field - 80 + i] / 8;
      // Pixel2 = ECG_Measurements[field - 80 + i + 1] / 8;

      Pixel1 = 128 - Pixel1;
      Pixel2 = 128 - Pixel2;

      // Serial.print("Pixel1:   ");
      // Serial.print(Pixel1);
      // Serial.print("      Pixel2:   ");
      // Serial.println(Pixel2);


      Pixel1 = constrain(Pixel1, 0, 128);
      Pixel2 = constrain(Pixel2, 0, 128);


      gbham(i, Del_Array[i], i + 1, Del_Array[i + 1], colLightBlue);          
      
      gbham(i, Pixel1, i + 1, Pixel2, colRed);

      Del_Array[i] = Pixel1;


      
    }

    

  	
    
    LCD_WriteString(Bpm, 2, 118, colViolett, colLightBlue, 0);
    
    delay(900);


  }


  
}









unsigned int FindMaxPosition(unsigned int Array[])
{
  unsigned int max = Array[80];
  unsigned int max_Index = 80;

  for(int i = 80; i <= BUFFER_SIZE_EKG - 80; i++)
  {
    if (Array[i] > max) 
    {
      max = Array[i];
      max_Index = i;
    }
     
  }
    

  return max_Index;

}



unsigned int FindMinPosition(unsigned int Array[])
{
  unsigned int min = Array[80];
  unsigned int min_Index = 80;

  for(int i = 80; i <= BUFFER_SIZE_EKG - 80; i++)
  {
    if (Array[i] < min) 
    {
      min = Array[i];
      min_Index = i;
    }
     
  }
    

  return min_Index;

}



/*-------------------- Function to calculate and return the Average and correct scaling of this value*/
unsigned int Average_and_Scaling()
{
  unsigned int Ave = 0;
  unsigned int Sum = 0;
  float Anzahl_Messungen = 0.0;
  int Anfang_Messung = micros();


  while(micros() <= Anfang_Messung + 6000)
  {
    Sum = Sum + analogRead(ECG_OUTPUT_SIGNAL);                    // Take the Sum of AVERAGE readings
    Anzahl_Messungen++;
  }
    
  if(Anzahl_Messungen != 0)
    Ave = Sum / Anzahl_Messungen;                                                // Take the Average of this Sum


  return Ave;                                    // Invert the scaling, for the display        

}







