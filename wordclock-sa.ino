//includs the Wire library
#include <Wire.h>

//includes the FastLED library
#include "FastLED.h"

// Fixed global definitions - data, clock
#define DATA_PIN 7
#define CLOCK_PIN 8

//because APA102s are retarded, I guess?
#define COLOR_ORDER BGR
#define LED_TYPE APA102
#define NUM_LEDS 144

// what is this
// Initialize changeable global variables.
uint8_t max_bright = 255;

// what is this
// what is this
struct CRGB leds[NUM_LEDS];






//---------
//RTC SETUP
//---------

// I2C bus address
#define DS1307_I2C_ADDRESS 0x68

// Static variables
// Declare time variables
byte second, minute, hour, weekDay, monthDay, month, year;

// Function to convert normal decimal number to numbers 0-9 (BCD)
byte decBcd(byte val) {
  return ( (val/10*16) + (val%10) );
}

// Function to convert numbers 0-9 (BCD) to normal decimal number
byte bcdDec(byte val) {
  return ( (val/16*10) + (val%16) );
}

// Function to set the system clock
void configureTime() {
   // 1) Set the date and time values
   second       = 00;
   minute       = 12;
   hour         = 1;
   weekDay      = 2;
   monthDay     = 29;
   month        = 11;
   year         = 16;

   // 2) Commands to start up the clock
   Wire.beginTransmission(DS1307_I2C_ADDRESS);
   Wire.write(0x00);
   Wire.write(decBcd(second));
   Wire.write(decBcd(minute));
   Wire.write(decBcd(hour));
   Wire.write(decBcd(weekDay));
   Wire.write(decBcd(monthDay));
   Wire.write(decBcd(month));
   Wire.write(decBcd(year));
   Wire.endTransmission();
}

// Function to call up the system time
void requestTime()
{
  // Reset the pointer to the register
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0x00);
  Wire.endTransmission();

  // Call up the time and date
  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);
  second        = bcdDec(Wire.read() & 0x7f);
  minute        = bcdDec(Wire.read());
  hour          = bcdDec(Wire.read() & 0x3f);
  weekDay       = bcdDec(Wire.read());
  monthDay      = bcdDec(Wire.read());
  month         = bcdDec(Wire.read());
  year          = bcdDec(Wire.read());

  // Print the date and time via serial monitor
  Serial.print(hour, DEC);
  Serial.print(":");
  Serial.print(minute, DEC);
  Serial.print(":");
  Serial.print(second, DEC);
  Serial.print("  ");
  Serial.print(monthDay, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print("  ");
}




//---------------------
//DEFINE THE LED GROUPS
//---------------------

//define the minutes
int MFIVE[4] = {43, 44, 45, 46};
int MTEN[3] = {1, 2, 3};
int MQUARTER[7] = {25, 26, 27, 28, 29, 30, 31};
int MTWENTY[6] = {36, 37, 38, 39, 40, 41};
int MTWENTYFIVE[10] = {36, 37, 38, 39, 40, 41, 43, 44, 45, 46};
int MHALF[4] = {19, 20, 21, 22};

//define the hours
int HONE[3] = {84, 85, 86};
int HTWO[3] = {87, 88, 89};
int HTHREE[5] = {67, 68, 69, 70, 71};
int HFOUR[4] = {115, 116, 117, 118};
int HFIVE[4] = {127, 128, 129, 130};
int HSIX[3] = {100, 101, 102};
int HSEVEN[5] = {121, 122, 123, 124, 125};
int HEIGHT[5] = {103, 104, 105, 106, 107};
int HNINE[4] = {132, 133, 134, 135};
int HTEN[3] = {97, 98, 99};
int HELEVEN[6] = {90, 91, 92, 93, 94, 95};
int HTWELVE[6] = {109, 110, 111, 112, 113, 114};

//define the other things
int ITIS[4] = {6, 7, 9, 10};
int PAST[4] = {62, 63, 64, 65};
int TO[2] = {60, 61};
int HBA[19] = {13, 14, 15, 16, 17, 50, 51, 52, 53, 54, 55, 56, 57, 78, 79, 80, 81, 82, 83};
int HBS[18] = {13, 14, 15, 16, 17, 50, 51, 52, 53, 54, 55, 56, 57, 72, 73, 74, 75, 76};
int OCLOCK[6] = {137, 139, 140, 141, 142, 143};


void setup() {

  // Initialize the I2C
  Wire.begin();
  
  delay(1000);
  Serial.begin(9600);
  LEDS.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  // FastLED.setMaxPowerInVoltsAndMilliamps(5, 100);
  FastLED.setBrightness(max_bright);





  // Set the time: ONLY THE FIRST TIME YOU RUN THE CODE!!
  configureTime(); 
  
}

void lightsOff() {
    for (int dot = 0; dot < NUM_LEDS; dot++) { 
        leds[dot] = CRGB::Black;    
    }
}

void lightsOn(int word[], int len) {
    int x;
    for (int dot = 0; dot < len; dot++){
      x = word[dot];
      leds[x] = CRGB::White;
    }
}


void loop() {
  
//Request the time from the RTC
requestTime();

//Send time to the monitor
Serial.println(" ");

//Kill unnecessary lights
lightsOff();
  
//Keep "IT IS" lit  
lightsOn(ITIS, 4);

//On the hour
if (minute < 5) {  lightsOn(OCLOCK, 6); }

//Five past
if ((minute > 4) && (minute < 10)) {  lightsOn(MFIVE, 4);  lightsOn(PAST, 4);}

//Ten past
if ((minute > 9) && (minute < 15)) {  lightsOn(MTEN, 3);  lightsOn(PAST, 4);}

//Quarter past
if ((minute > 14) && (minute < 20)) {  lightsOn(MQUARTER, 7);  lightsOn(PAST, 4);}

//Twenty past
if ((minute > 19) && (minute < 25)) {  lightsOn(MTWENTY, 6);  lightsOn(PAST, 4);}

//Twenty-five past
if ((minute > 24) && (minute < 30)) {  lightsOn(MTWENTYFIVE, 10);  lightsOn(PAST, 4);}

//Half past
if ((minute > 29) && (minute < 35)) {  lightsOn(MHALF, 4);  lightsOn(PAST, 4);}

//Twenty-five to
if ((minute > 34) && (minute < 40)) {  lightsOn(MTWENTYFIVE, 10);  lightsOn(TO, 2);}

//Twenty to
if ((minute > 39) && (minute < 45)) {  lightsOn(MTWENTY, 6);  lightsOn(TO, 2);}

//Quarter to
if ((minute > 44) && (minute < 50)) {  lightsOn(MQUARTER, 7);  lightsOn(TO, 2);}

//Ten to
if ((minute > 49) && (minute < 55)) {  lightsOn(MTEN, 3);  lightsOn(TO, 2);}

//Five to
if ((minute > 54) && (minute < 60)) {  lightsOn(MFIVE, 4);  lightsOn(TO, 2);}

//Hours
if ((hour == 1) && (minute <= 30)) {  lightsOn(HONE, 3);}
if ((hour == 1) && (minute > 30)) {  lightsOn(HTWO, 3);}
if ((hour == 13) && (minute <= 30)) {  lightsOn(HONE, 3);}
if ((hour == 13) && (minute > 30)) {  lightsOn(HTWO, 3);}

if ((hour == 2) && (minute <= 30)) {  lightsOn(HTWO, 3);}
if ((hour == 2) && (minute > 30)) {  lightsOn(HTHREE, 5);}
if ((hour == 14) && (minute <= 30)) {  lightsOn(HTWO, 3);}
if ((hour == 14) && (minute > 30)) {  lightsOn(HTHREE, 5);}

if ((hour == 3) && (minute <= 30)) {  lightsOn(HTHREE, 5);}
if ((hour == 3) && (minute > 30)) {  lightsOn(HFOUR, 4);}
if ((hour == 15) && (minute <= 30)) {  lightsOn(HTHREE, 5);}
if ((hour == 15) && (minute > 30)) {  lightsOn(HFOUR, 4);}

if ((hour == 4)  && (minute <= 30)) {  lightsOn(HFOUR, 4);}
if ((hour == 4) && (minute > 30)) {  lightsOn(HFIVE, 4);}
if ((hour == 16) && (minute <= 30)) {  lightsOn(HFOUR, 4);}
if ((hour == 16) && (minute > 30)) {  lightsOn(HFIVE, 4);}

if ((hour == 5) && (minute <= 30)) {  lightsOn(HFIVE, 4);}
if ((hour == 5) && (minute > 30)) {  lightsOn(HSIX, 3);}
if ((hour == 17) && (minute <= 30)) {  lightsOn(HFIVE, 4);}
if ((hour == 17) && (minute > 30)) {  lightsOn(HSIX, 3);}

if ((hour == 6) && (minute <= 30)) {  lightsOn(HSIX, 3);}
if ((hour == 6) && (minute > 30)) {  lightsOn(HSEVEN, 5);}
if ((hour == 18) && (minute <= 30)) {  lightsOn(HSIX, 3);}
if ((hour == 18) && (minute > 30)) {  lightsOn(HSEVEN, 5);}

if ((hour == 7) && (minute <= 30)) {  lightsOn(HSEVEN, 5);}
if ((hour == 7) && (minute > 30)) {  lightsOn(HEIGHT, 5);}
if ((hour == 19) && (minute <= 30)) {  lightsOn(HSEVEN, 5);}
if ((hour == 19) && (minute > 30)) {  lightsOn(HEIGHT, 5);}

if ((hour == 8) && (minute <= 30)) {  lightsOn(HEIGHT, 5);}
if ((hour == 8) && (minute > 30)) {  lightsOn(HNINE, 4);}
if ((hour == 20) && (minute <= 30)) {  lightsOn(HEIGHT, 5);}
if ((hour == 20) && (minute > 30)) {  lightsOn(HNINE, 4);}

if ((hour == 9) && (minute <= 30)) {  lightsOn(HNINE, 4);}
if ((hour == 9) && (minute > 30)) {  lightsOn(HTEN, 3);}
if ((hour == 21) && (minute <= 30)) {  lightsOn(HNINE, 4);}
if ((hour == 21) && (minute > 30)) {  lightsOn(HTEN, 3);}

if ((hour == 10) && (minute <= 30)) {  lightsOn(HTEN, 3);}
if ((hour == 10) && (minute > 30)) {  lightsOn(HELEVEN, 6);}
if ((hour == 22) && (minute <= 30)) {  lightsOn(HTEN, 3);}
if ((hour == 22) && (minute > 30)) {  lightsOn(HELEVEN, 6);}

if ((hour == 11) && (minute <= 30)) {  lightsOn(HELEVEN, 6);}
if ((hour == 11) && (minute > 30)) {  lightsOn(HTWELVE, 6);}
if ((hour == 23) && (minute <= 30)) {  lightsOn(HELEVEN, 6);}
if ((hour == 23) && (minute > 30)) {  lightsOn(HTWELVE, 6);}

if ((hour == 12) && (minute <= 30)) {  lightsOn(HTWELVE, 6);}
if ((hour == 12) && (minute > 30)) {  lightsOn(HONE, 3);}
if ((hour == 0) && (minute <= 30)) {  lightsOn(HTWELVE, 6);}
if ((hour == 0) && (minute > 30)) {  lightsOn(HONE, 3);}

//Sandy birthday
if (monthDay == 4) {  if (month == 10) {  lightsOn(HBS, 18);}}

//Ali birthday
if (monthDay == 21) {  if (month == 3) {  lightsOn(HBA, 19);}}

//Lights on!
FastLED.show();

//Wait ten seconds
delay(1000);
}
