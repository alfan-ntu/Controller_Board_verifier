/*
 * This sketch is used to verify the wiring of devices, including sensors and control board, in the test bed
 * Coded : Al Fan@2017/10/13
 */

#include <SoftwareSerial.h>

// #define CHU_PD_ONBOARD
#define DEFAULT_BAUD_RATE   9600
 
int Led = 13;               // define LED pin
int buttonPin = 12;         // define pushbutton input
int switchPin = 11;         // dip switch pin 
int photoDiodePin = 10;     // photo diode input

#ifdef  CHU_PD_ONBOARD
  int chuPhotoDiodePin = 6;   // chuPhotoDiode input; multiplexed, the same as software serial port#2 
#endif 
int rLED = 2;               // define R-LED pin
int gLED = 3;               // define G-LED pin
int bLED = 4;               // define B-LED pin
int chuLED = 5;             // define chu-LED pin 
int sSP1_Rx = 6;            // software serial port#1 Rx
int sSP1_Tx = 7;            // software serial port#1 Tx
int sSP2_Rx = 8;            // software serial port#2 Rx
int sSP2_Tx = 9;            // software serial port#2 Tx

int val, sw;                // define a numeric variable
char rgbRegister = 0;       // RGB LED register
const char rLED_ON = 1;     
const char gLED_ON = 2;
const char bLED_ON = 4;
const int lenCommand = 128;

SoftwareSerial sSP1(sSP1_Rx, sSP1_Tx, 1);     // Rx, Tx, inversion, sSP1 port is reserved for listening to sensor report
SoftwareSerial sSP2(sSP2_Rx, sSP2_Tx, 1);     // Rx, Tx, inversion, sSP2 port is reserved for communication with the backend controller


void setup()
{
  pinMode(Led, OUTPUT);           // LED pin as output
  pinMode(rLED, OUTPUT);          // rLED pin as output
  pinMode(gLED, OUTPUT);          // gLED pin as output
  pinMode(bLED, OUTPUT);          // bLED pin as output
  pinMode(chuLED, OUTPUT);        // chuLED pin as output
  pinMode(buttonPin, INPUT);      // push button pin as input
  pinMode(switchPin, INPUT);      // dip switch pin as input
  pinMode(photoDiodePin, INPUT);  // photo diode pin as input
  
//#ifdef CHU_PD_ONBOARD  
//  pinMode(chuPhotoDiodePin, INPUT); // chu Photo diode pin as input
//#endif

  digitalWrite(gLED, LOW);
  digitalWrite(rLED, LOW);
  digitalWrite(bLED, LOW);
  delay(500);
  rgbRegister = 1;                // Rgb register
  Serial.begin(DEFAULT_BAUD_RATE);
  sSP2.begin(DEFAULT_BAUD_RATE);
  sSP1.begin(DEFAULT_BAUD_RATE);
  Serial.println("controller board is ready...");
  
  sSP2.println("controller board is ready...");  
  sSP1.listen();
//  sSP2.listen();                // only one Software Serial port can be listening at one time
}

void loop()
{
   char c = 0, rxStr[lenCommand];
   int i=0, lenRx=0;

   for (i=0; i++; i < lenCommand) {
      rxStr[i] = "a";
   }
   i = 0;

  do{ 
    digitalWrite(rLED, LOW);
    if (sSP1.available())
    {
      digitalWrite(rLED, HIGH);
      c = (char)sSP1.read();
      Serial.print(c);
      rxStr[i++] = c;
    }
  }
  while (c!=0x0A & i < lenCommand);
  lenRx = i; 
//
// relaying the information to the Backend Controller
//  
  for (i=0; i<lenRx; i++)
    sSP2.print(rxStr[i]);  

/*
  val=digitalRead(buttonPin);     //read the value of the sensor 
  if(val == HIGH)                 // turn on LED when sensor is blocked 
  {
    digitalWrite(Led,HIGH);
    digitalWrite(chuLED, HIGH);
    Serial.println("pushbutton is released.....");
  }
  else
  {
    digitalWrite(Led,LOW);
    digitalWrite(chuLED, LOW);
    Serial.println("pushbutton is pressed.....");
  }
  
  if (rgbRegister == rLED_ON)
  {
    digitalWrite(rLED, HIGH);
    digitalWrite(gLED, LOW);
    digitalWrite(bLED, LOW);
    rgbRegister = gLED_ON;
  } else if(rgbRegister == gLED_ON) {
    digitalWrite(rLED, LOW);
    digitalWrite(gLED, HIGH);
    digitalWrite(bLED, LOW);
    rgbRegister = bLED_ON;
  } else {
    digitalWrite(rLED, LOW);
    digitalWrite(gLED, LOW);
    digitalWrite(bLED, HIGH);
    rgbRegister = rLED_ON;
    delay(1000);
    digitalWrite(rLED, HIGH);
    digitalWrite(gLED, HIGH);
    digitalWrite(bLED, HIGH);
  }
*/ 
}
