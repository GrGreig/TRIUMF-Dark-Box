// Code for T/RH measurement and actuation of high voltage switches for signal routing.
// Acts as a slave to the accompanying LabVIEW code.
// Author: Graham Greig, ggreig@sfu.ca

// Code for T,RH and luminosity measurment. Output is to the serial monitor and 
// LabVIEW. Device is to be placed inside of probe station so OLED display has
// been removed.

//#include <Sensirion.h>
//#include <Adafruit_TSL2591.h>
#include <Wire.h>
#include "SHTSensor.h"
#include <math.h>

const unsigned long TRHSTEP   = 5000UL;   // Sensor query period

// Make the temperature sensor object
SHTSensor sht;
 
// Instantiate the luminosity sensor object
//Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);

//Declare variables
//uint32_t lum;
//uint16_t ir, full;
unsigned int rawData;
//float lux;
float temperature;
float humidity;
byte measActive = false;
//byte measType = TEMP;
unsigned long trhMillis = 0;        // Time interval tracking
int interlock;
String Pins;
String nextPin = "56";                   // Some value that will never be used.

void setup()
{
  // Setup the Luminosity sensor
  //tsl.begin();
  //tsl.setGain(TSL2591_GAIN_MAX);      // 9876x gain --> for very low light
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light)
 
  // Setup the temperature sensor
  Wire.begin();
  sht.init();

  //Set the pin modes to output.
  pinMode(0,OUTPUT);
  pinMode(1,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(14,OUTPUT);
  pinMode(15,OUTPUT);
  pinMode(23,OUTPUT);
  pinMode(25,OUTPUT);
  pinMode(27,OUTPUT);
  pinMode(29,OUTPUT);
  pinMode(31,OUTPUT);
  pinMode(33,OUTPUT);
  pinMode(35,OUTPUT);
  pinMode(37,OUTPUT);
  pinMode(39,OUTPUT);
  pinMode(41,OUTPUT);
  pinMode(43,OUTPUT);
  pinMode(45,OUTPUT);
  pinMode(47,OUTPUT);
  pinMode(49,OUTPUT);
  pinMode(51,OUTPUT);
  pinMode(53,OUTPUT);
  
  Serial.begin(9600);
  byte stat;
  byte error = 0;
  delay(5000);                  // Wait >= 5s before first cmd, before SHT output is 0
  Serial.print("Program Started");
  Serial.print("\n");
  Serial.flush();
}
 
void loop(){
  
  //----- necessary for SHT readout -----
  unsigned long curMillis = millis();       // Get current time
  
  if (curMillis - trhMillis >= TRHSTEP){   // Time for new measurements?
    // Get the T/RH measurement and print to the serial monitor.
    if (sht.readSample()) {
  //      Serial.print("  RH:  ");
  //      Serial.print(" %");
       humidity = sht.getHumidity();
  //      Serial.print(rh, 2);
  //      Serial.print("\n");
  //      Serial.print("  T:  ");
        temperature = sht.getTemperature();
  //      Serial.print(temp, 2);
  //      Serial.print(" °C");
  //      Serial.print("\n");
        trhMillis = curMillis;
     } else {
      //Serial.print("Error in readSample()\n");
    }
  }
  //Get the luminosity measurement (Commented out for now)
  //lum = tsl.getFullLuminosity(); // Read luminosity
  //ir = lum >> 16;
  //full = lum & 0xFFFF;

  // Check for overflow.
  //if (full == 0xFFFF || ir == 0xFFFF) {
    //lux = 0xFFFF;
    //Serial.print("  Lum:  ");
    //Serial.print( "SATURATION" );
    //Serial.print("\n");
  //} else {
    //lux = tsl.calculateLux( full, ir);
    //Serial.print("  Lum:  ");
    //Serial.print(lux*1000, 2);
    //Serial.print(" mlux");
    //Serial.print("\n");
  //}

  //Check that the light in the dark box is at a reasonable level.
  //if (lux <= 100) {
    //interlock = 0;
  //} else {
    //interlock = 1;
  //}

  // Respond to commands from LabVIEW
  if (Serial.available() > 0) {
    int inByte = Serial.read();

    if (inByte == 'a'){
      // Modified to always return 0. The arduino will not be used for edge 
      // detection as the probe station has this capability or it is not needed. 
      Serial.print(0);
      Serial.print("\n");
    }

    else if (inByte == 'b'){
      //Serial.print(interlock); (Need to get Lux sensor to SFU)
      Serial.print(0);
      Serial.print("\n");
      Serial.flush();
    }
    
    else if (inByte == 'c'){ //Send T/RH inforamtion.
      Serial.print(temperature);
      Serial.print(" ");
      Serial.print(humidity);
      Serial.print("\n");
    }
    else if (inByte == 'd'){ //Close the HV switches based on the input command sequence. Note, the Arduino serial buffer is only 64 bytes so will need to send in multiple parts.

      // Readout the serial buffer stream (max of 8 commands).
      Pins = Serial.readStringUntil(';'); 
      // Serial.println(Pins);
    

      while(Pins.length() > 0){   //Parse the string by commas. Input must also end with a comma to work. 
           int index = 0;
           while(Pins.charAt(index) != ','){
           index++;
           } 
           
           nextPin = Pins.substring(0,index);  // Get the substring
           Pins.remove(0,index+1);
           Serial.print(nextPin);
           Serial.print(" ");
           delay(500);
           
           bool ans = digitalRead(nextPin.toInt());  //set the pin output status
           Serial.print("Before ");
           Serial.print(ans);
           digitalWrite(nextPin.toInt(), HIGH);
           ans = digitalRead(nextPin.toInt());
           Serial.print("After ");
           Serial.print(ans);
           Serial.print("\n");
      }
     }
     else if (inByte == 'e'){ // Code to open all relays.
       digitalWrite(0,LOW);
       digitalWrite(1,LOW);
       digitalWrite(3,LOW);
       digitalWrite(4,LOW);
       digitalWrite(5,LOW);
       digitalWrite(6,LOW);
       digitalWrite(7,LOW);
       digitalWrite(8,LOW);
       digitalWrite(9,LOW);
       digitalWrite(10,LOW);
       digitalWrite(11,LOW);
       digitalWrite(12,LOW);
       digitalWrite(13,LOW);
       digitalWrite(14,LOW);
       digitalWrite(15,LOW);
       digitalWrite(21,LOW);
       digitalWrite(23,LOW);
       digitalWrite(25,LOW);
       digitalWrite(27,LOW);
       digitalWrite(29,LOW);
       digitalWrite(31,LOW);
       digitalWrite(33,LOW);
       digitalWrite(35,LOW);
       digitalWrite(37,LOW);
       digitalWrite(39,LOW);
       digitalWrite(41,LOW);
       digitalWrite(43,LOW);
       digitalWrite(45,LOW);
       digitalWrite(47,LOW);
       digitalWrite(49,LOW);
       digitalWrite(51,LOW);
       digitalWrite(53,LOW);
     }
         else if (inByte == 'f'){ //Open the HV switches based on the input command sequence. Note, the Arduino serial buffer is only 64 bytes so will need to send in multiple parts.

      // Readout the serial buffer stream (max of 8 commands).
      Pins = Serial.readStringUntil(';'); 
      // Serial.println(Pins);
    

      while(Pins.length() > 0){   //Parse the string by commas. Input must also end with a comma to work. 
           int index = 0;
           while(Pins.charAt(index) != ','){
           index++;
           } 
           
           nextPin = Pins.substring(0,index);  // Get the substring
           Pins.remove(0,index+1);
           Serial.print(nextPin);
           Serial.print(" ");
           delay(500);
           
           bool ans = digitalRead(nextPin.toInt());  //set the pin output status
           Serial.print("Before ");
           Serial.print(ans);
           digitalWrite(nextPin.toInt(), LOW);
           ans = digitalRead(nextPin.toInt());
           Serial.print("After ");
           Serial.print(ans);
           Serial.print("\n");
      }
    }
  }
    Serial.flush();
  
  delay(100);  
}
