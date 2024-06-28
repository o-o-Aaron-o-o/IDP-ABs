


#include "MAX30105.h"                 // Bibliothek für den Oxi-Sensor
#include "heartRate.h"

#include <math.h>

#include "Display.h"
#include "pitches.h"

#include <ArduinoBLE.h>



MAX30105 Pulsoxi;                    //Wichtig für das Pulsoximeter

#define PEEP D8

//ECG Pins
#define ECG_OUTPUT_SIGNAL A1
#define LO_MINUS A2
#define LO_PLUS A3
#define SDN A6


#define BUFFER_SIZE_EKG 320
#define AVERAGE 300
#define PULSE_THRESHOLD 55

#define GLATTUNG_EKG 60
#define PI 3.14159265359




#define SDA A4                       //Pulsoximeter Pins
#define SCL A5                       //

#define BUFFER_SIZE_OXI 160              //Größe der Zeichen-Arrays
#define GLATTUNG_IR 0.02             //Glättungsfaktor des Mittelwertes des Infrarot-Signals
#define GLATTUNG_R 0.02              //Glättungsfaktor des Mittelwertes des Rot-Signals



void BLE_Init();

void EKG_Init();

void loop_ekg();


unsigned int Max_Position(unsigned int Array[]);

unsigned int Min_Position(unsigned int Array[]);

unsigned int Mittelwert_Messwerte();





void Oxi_Init();

void loop_oxi();




void Line_IRed(int hi);
void Line_Red(int hi);




