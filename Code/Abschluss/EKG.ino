
/*   Defines für die Pins in AbsHeader.h */

/*----------Funktion, die die EKG-Messung initialisiert-------------------*/
void EKG_Init()
{
  pinMode(ECG_OUTPUT_SIGNAL, INPUT);                                        //Input Pin für das EKG-Signal initialisieren
  pinMode(LO_MINUS, INPUT);                                                 //Leads Off Comparator Output. In dc leads off detection mode, LOD− is high when the electrode to −IN is disconnected, and it is low when connected. 
  pinMode(LO_PLUS, INPUT);                                                  //Leads Off Comparator Output. In dc leads off detection mode, LOD+ is high when the +IN electrode is disconnected, and it is low when connected.
  pinMode(SDN, OUTPUT);                                                     //Shutdown Control Input. Drive SDN low to enter the low power shutdown mode.
  digitalWrite(SDN, HIGH);                                                  //Shotdown Pin auf High setzen 

  pinMode(LEDR, OUTPUT);                                                    //Rote Led als Output initialisieren
  digitalWrite(LEDR, HIGH);                                                 //Rote Led aus
  pinMode(LEDG, OUTPUT);                                                    //Grüne Led als Output initialisieren
  digitalWrite(LEDG, HIGH);                                                 //Grüne Led aus
}









void loop_ekg() 
{
  
  static char Bpm[50] = {0};                                                //Array, in das die Bpm-Werte reingeschrieben werden die auf dem Display ausgegeben werden
  
  static unsigned int EKG_Messwerte_Array[BUFFER_SIZE_EKG] = {0};           //Array, in das die gemessenen EKG-Werte gespeichert werden
  static unsigned int Zeit_Array[BUFFER_SIZE_EKG] = {0};                    //Array, in das die Zeitpunkte der Messwerte gepeichert werden
  unsigned int Peak_Zeit_Array[5] = {0};                                    //Array, in das die Zeitpunkte der Peaks gespeichert werden
  //unsigned int Peak_Array[5] = {0};                                       //Array, in das die Positionen der Peaks gespeichert werden

  static unsigned int Del_Array[160] = {0};                                 //Array, in das die gezeichnete Line gespeichert wird --> um diese zu löschen

  bool Warning_Flag = false;                                                //Flag, die angibt ob die EKG-Kabel nicht angeschlossen sind <-- false bedeutes, dass die Kabel nicht 'nicht angeschlossen' sind



  if((digitalRead(LO_MINUS)) || (digitalRead(LO_PLUS)))                     //Wenn einer der beiden Pins (die die EKG-Kabel-Verbindung prüfen) auf High ist, wird eine Warnung ausgegeben 
  {
    static char warning[] = "Cables not connected!";                          //Array, in das eine Warnung gespeichert wird
    digitalWrite(LEDG, HIGH);                                                 //Grüne Led aus
    digitalWrite(LEDR, LOW);                                                  //Rote  Led an

    LCD_WriteString(warning, 10, 60, colBlack, colWhite, 0);                  //Die im warning-Array gespeicherte Warnung wird auf das Display geschrieben
    delay(200);                                                               //Warten von 200ms

    FillWithColor(colLightBlue);                                              //Löschen des Displays, mit der Hintergrundfarbe
    Warning_Flag = true;                                                      //Setzen der Flag auf true <-- Kabel sind 'nicht angeschlossen'
  }
  else                                                                      //Wenn beide Pins (die die EKG-Kabel-Verbindung prüfen) auf Low sind, wird keine Warnung ausgegeben
  {
    digitalWrite(LEDR, HIGH);                                                 //Rote Led aus
    digitalWrite(LEDG, LOW);                                                  //Grüne Led an
    Warning_Flag = false;                                                     //Setzen der Flag auf false <-- Kabel sind nicht 'nicht angeschlossen'
  }

  if(!Warning_Flag)                                                         //Wenn die Flag (für die Warnung) auf false <-- Kabel sind nicht 'nicht angeschlossen' <-- Entspricht: if(Warning_Flag == false)
  {
    for(int i = 0; i < BUFFER_SIZE_EKG; i++)                                  //for-Schleife, die Arrays für die Messwerte und deren Zeiten auffüllt
    {
      EKG_Messwerte_Array[i] = Mittelwert_Messwerte();                          //Jeder Platz im Array der Messwerte wird mit einem Aufruf der Mittelwert_Messwerte-Funktion besetzt
      Zeit_Array[i] = millis();                                                 //Jeder Platz im Zeit-Array wir mit einem Zeitstempel, der nach jedem Messwert genommen wird, gefüllt
    }


    int j = 1, Anzahl_Peaks = 0;                                              //Variablen 'j' (mit 1 initialisiert) für die folgende while-Schleife und 'Anzahl_Peaks' um die Anzahl der gefundenen Peaks zu speichern
    bool Peak_Flag = true;                                                    //Flag, die angibt ob gerade ein Peak passiert ist <-- mit ja / true initialisiert


    while(j < BUFFER_SIZE_EKG)                                                //while-Schleife, die läuft solange die Variable 'j' kleiner als der Wert von BUFFER_SIZE_EKG ist
    {
      if((EKG_Messwerte_Array[j] < 800) && (Peak_Flag))                         //Wenn der aktuelle Wert im Array der Messwerte kleiner als '800' ist und die Peak_Flag auf true ist (gerade ein Peak passiert ist)
        Peak_Flag = false;                                                        //Setzen der Peak_Flag auf false <-- Peak vorbei
      
      //Wenn (vorheriger Wert kleiner gleich dem aktuellen)  und       (aktueller Wert größer dem nächsten)        und (aktueller Wert größer gleich 800) und (Anzahl_Peaks kleiner 5) und (kein Peak passiert ist)
      if((EKG_Messwerte_Array[j-1] <= EKG_Messwerte_Array[j]) && (EKG_Messwerte_Array[j] > EKG_Messwerte_Array[j+1]) && (EKG_Messwerte_Array[j] >= 800) && (Anzahl_Peaks < 5) && (!Peak_Flag))
      {
        //Peak_Array[Anzahl_Peaks] = j;
        Peak_Zeit_Array[Anzahl_Peaks] = Zeit_Array[j];                            //Der Zeitpunkt des Peaks wird in das Peak_Zeit_Array gelegt
        Anzahl_Peaks++;                                                           //Die Anzahl der Peaks wird inkrementiert
        Peak_Flag = true;                                                         //Setzen der Peak_Flag auf true <-- Peak passiert
      }

      j++;                                                                      //Variable 'j' wird inkrementiert

    }

    float deltaT = 0.0;                                                       //Variable, die die mittlere Zeit speichern soll
    static float bpm = 0.0;                                                   //Variable, die den Wert für die Herzschläge pro Minute speichern soll

    /*
      for(int ndi = 0; ndi < 5; ndi++)
      {
        Serial.print(Peak_Zeit_Array[ndi]);
        Serial.print("   ");
        
        if(ndi == 4)
        {
          Serial.print("  Peaks  ");
          Serial.print(Anzahl_Peaks);
        }
      }
    */

    if(Anzahl_Peaks >= 2)                                                     //Wenn mindestens 2 Peaks passiert sind
    {
      deltaT = (Peak_Zeit_Array[Anzahl_Peaks-1] - Peak_Zeit_Array[0]) / (Anzahl_Peaks-1); //Variable 'deltaT' wird mit der Rechnung [(Zeitpunkt des zuletzt passierten Peaks) - (Zeitpunkt des zuerst passierten Peaks) / (Anzahl der Peaks) - 1] berechnet

      if(deltaT != 0)                                                           //Wenn die Variable 'deltaT' nicht Null ist      
        bpm = 60000.0 / deltaT;                                                   //Variable 'bpm' wird berechnet
    }

  	/*
      Serial.println();
      Serial.println();
      Serial.print("MitDelT:   ");
      Serial.print(mittleresDelta);
      Serial.print("   DT:  ");
      Serial.print(deltaT);
      Serial.print("   BPM: ");
      Serial.println(bpm);
      
      
      for(int o = 0; o < 5; o++)
      {
        
        Serial.println();
        Serial.print("Peak at:  ");
        Serial.print(Peak_Array[o]);    
        Serial.print("   Delta:  ");
        Serial.print(delta_Array[o]);     

      }
    */

    snprintf(Bpm, sizeof(Bpm), "% .1f Bpm", bpm);                             //Der berechnete Wert für die Variable 'bpm' wird in das Bpm-Array geschrieben


    unsigned int Index_Maximum = Max_Position(EKG_Messwerte_Array);           //Variable 'Index_Maximum' wird mit dem Ergebnis der 'Max_Position(EKG_Messwerte_Array)' initialisiert
    unsigned int Index_Minimum = Min_Position(EKG_Messwerte_Array);           //Variable 'Index_Minimum' wird mit dem Ergebnis der 'Min_Position(EKG_Messwerte_Array)' initialisiert

    unsigned int min = EKG_Messwerte_Array[Index_Minimum];                    //Variable 'min' wird mit dem Wert, an der Stelle 'Index_Minimum' vom 'EKG_Messwerte_Array', initialisiert 
    unsigned int max = EKG_Messwerte_Array[Index_Maximum];                    //Variable 'max' wird mit dem Wert, an der Stelle 'Index_Maximum' vom 'EKG_Messwerte_Array', initialisiert
    unsigned int max_min = max - min;                                         //Variable 'max_min' wird mit der Differenz, von den Werten 'max' und 'min', initialisiert


    for(int i = 0; i < 159; i++)                                              //for-Schleife, die läuft bid die Variable 'i' bis zu dem Wert 158 inkrementiert wurde 
    {

      int Startpunkt_Line = ((EKG_Messwerte_Array[Index_Maximum - 80 + i] - min) * 128) / max_min;  //Variable 'Startpunkt_Linie' wird mit einem, für das Display skalierten Wert, deklariert
      int Endpunkt_Line = ((EKG_Messwerte_Array[Index_Maximum - 80 + i + 1] - min) * 128) / max_min;//Variable 'Endpunkt_Linie' wird mit einem, für das Display skalierten Wert, deklariert


      Startpunkt_Line = 128 - Startpunkt_Line;                                  //Invertierung es Wertes für die Variable 'Startpunkt_Linie' <-- weil Koordinatensystem des Displays falschherrum
      Endpunkt_Line = 128 - Endpunkt_Line;                                      //Invertierung es Wertes für die Variable 'Endpunkt_Linie' <-- weil Koordinatensystem des Displays falschherrum


      Startpunkt_Line = constrain(Startpunkt_Line, 0, 128);                     //Prüfen und gegebenenfalls korrektur, ob die Variable 'Startpunkt_Linie' im Rahmen von 0 bis 128 liegt 
      Endpunkt_Line = constrain(Endpunkt_Line, 0, 128);                         //Prüfen und gegebenenfalls korrektur, ob die Variable 'Endpunkt_Linie' im Rahmen von 0 bis 128 liegt


      gbham(i, Del_Array[i], i + 1, Del_Array[i + 1], colLightBlue);            //Löschen der bisher gezeichneten Linie auf dem Display mit den Werten aus dem 'del_Array' <-- Zeichnung der selben Linie mit der Hintergrundfarbe
      
      gbham(i, Startpunkt_Line, i + 1, Endpunkt_Line, colRed);                  //Zeichnen einer Linie auf dem Display mit den Variablen 'Startpunkt_Line' und 'Endpunkt_Line' als y-Werte der Linie und 'i' als x-Werte; in Rot

      Del_Array[i] = Startpunkt_Line;
      
    }


    LCD_WriteString(Bpm, 90, 12, colViolett, colLightBlue, 0);                //Schreiben des Inhalts vom 'Bpm'-Array auf das Display
    
    //delay(900);                                                             //Verzögerung von 900ms


  }


  Serial.println();                                                         //Ein println am Ende, sodass man in der Mitte / am Anfang nicht darauf achten muss, wo das ln steht
}








/*---------------Funktion, die die Position des größten Wertes von einem übergebenen Arrays zurückgibt-----------------*/
unsigned int Max_Position(unsigned int Array[])
{
  unsigned int max = Array[80];                                             //Variable 'max' wird mit dem 80ten Wert, vom übergebenen Array, initialisiert
  unsigned int max_Index = 80;                                              //Variable 'max_Index' wird mit dem Wert 80 initialisiert

  for(int i = 80; i <= BUFFER_SIZE_EKG - 80; i++)                           //for-Schleife, die läuft bis die Variable 'i' von dem Wert '80' auf den Wert von 'BUFFER_SIZE_EKG - 80' inkrementiert wurde
  {
    if (Array[i] > max)                                                       //Wenn der aktuelle Wert im übergebenen Array größer ist, als der Wert der Variable 'max'
    {
      max = Array[i];                                                           //Variable 'max' bekommt den aktuellen Wert des übergebenen Arrays
      max_Index = i;                                                            //Variable 'max_Index' bekommt den aktuellen Wert der Variable 'i'
    }
     
  }

  return max_Index;                                                         //Der Wert der Variable 'max_Index' wird zurückgegeben

}


/*---------------Funktion, die die Position des kleinsten Wertes von einem übergebenen Arrays zurückgibt-----------------*/
unsigned int Min_Position(unsigned int Array[])
{
  unsigned int min = Array[80];                                             //Variable 'min' wird mit dem 80ten Wert, vom übergebenen Array, initialisiert
  unsigned int min_Index = 80;                                              //Variable 'min_Index' wird mit dem Wert 80 initialisiert

  for(int i = 80; i <= BUFFER_SIZE_EKG - 80; i++)                           //for-Schleife, die läuft bis die Variable 'i' von dem Wert '80' auf den Wert von 'BUFFER_SIZE_EKG - 80' inkrementiert wurde
  {
    if (Array[i] < min)                                                       //Wenn der aktuelle Wert im übergebenen Array kleiner ist, als der Wert der Variable 'min'
    {
      min = Array[i];                                                         //Variable 'min' bekommt den aktuellen Wert des übergebenen Arrays
      min_Index = i;                                                          //Variable 'min_Index' bekommt den aktuellen Wert der Variable 'i'
    }
     
  }
    
  return min_Index;                                                         //Der Wert der Variable 'min_Index' wird zurückgegeben

}



/*--------------------Funktion, die über 6ms EKG-Messwerte aufnimmt und einen Mittelwert bildet----------------*/
unsigned int Mittelwert_Messwerte()
{
  unsigned int Ave = 0;                                                     //Variable 'Ave' wird mit dem Wert '0' initialisiert
  unsigned int Sum = 0;                                                     //Variable 'Sum' wird mit dem Wert '0' initialisiert
  float Anzahl_Messungen = 0.0;                                             //Variable 'Anzahl_Messungen' wird mit dem Wert '0.0' initialisiert
  int Anfang_Messung = micros();                                            //Variable 'Anfang_Messung' wird mit dem aktullen Zeit-Wert des Progamms in Microsekuden initialisiert


  while(micros() <= Anfang_Messung + 6000)                                  //while-Schleife, die läuft bis 6000 Microsecunden (6 Millisekunden) vergangen sind
  {
    Sum = Sum + analogRead(ECG_OUTPUT_SIGNAL);                                //Variable 'sum' wird mit den gemessenen Werten aufaddiert
    Anzahl_Messungen++;                                                       //Variable 'Anzahl_Messungen' wird inkrementiert
  } 

  if(Anzahl_Messungen != 0)                                                 //Wenn die Variable 'Anzahl_Messungen' ungleich '0' ist
    Ave = Sum / Anzahl_Messungen;                                             //Variable 'Ave' wird berechnet mit der Division von der Variable 'Sum' und der Variable 'Anzahl_Messungen'


  return Ave;                                                               //Der Wert der Variable 'Ave' wird zurückgegeben

}







