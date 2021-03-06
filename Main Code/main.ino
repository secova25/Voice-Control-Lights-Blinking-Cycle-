/*
Voice Control Lights Blinking Cycle.
Created by:
Chris Loang, tloang2, UIN needed
Jay Patel, jpate245. UIN needed
Sergio Covarrubias, scova2, UIN needed

Link to a demo of our project:


Description of our project.
We will have an Android emulator on Windows or Mac to connect to bluetooth with a HC - 05 bluetooth sensor module 
to enable voice recognition. We will have a Serial port from Arduino, and perform serial readStrings to get the string from the serial port,
and that will be our input for the project. Our input will be an input to command LEDs functionality, we will have NeoPixels and use our NeoPixels LEDs to display disco,
wave functionality, also changing the colors. To display disco functionality, we will have it blink multiple in random, and for a wave functionality, 
we will flash the first led, follow with a second one, and along the strip of the neopixel leds. At the same time, we will have an LCD display that displays current 
information about the NeoPixels functionality that is being used, and for the second row of the LCD, we will show suggestions for the alternative functionality of the 
LEDs that you can input. We will also have a button or a switch, that you can manually change the LEDs functionalities rather than have your voices as input.
 
 The circuit:
 * LCD RS pin to Arduino digital pin 12
 * LCD Enable pin to Arduino digital pin 11
 * LCD D4 pin to Arduino digital pin 5
 * LCD D5 pin to Arduino digital pin 4
 * LCD D6 pin to Arduino digital pin 3
 * LCD D7 pin to Arduino digital pin 2
 * LCD R/W pin to Arduino Analog pin GND
 * 10K resistor
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 * Bluetooth RXD pin to  Arduino digital pin RX/D0
 * Bluetooth TXD pin to  Arduino digital pin TX/D1
 * Bluetooth VCC pin to Arduino Analog  pin 5V
 * Bluetooth GND pin to Arduino Analog pin GND
 * Potentiometer 100K 
 * 220 resistor:
 
 *RGB 60 LED light ground pin to Arduino Analog GND
 *RGB 60 LED light power pin to Arduino Analog 5v
 *RGB 60 LED light pin connector to Arduino digital pin 6
 *RGB 60 LED external(RED) power cable to power adaptor 5v port
 *RGB 60 LED external(WHITE) GND cable to power adaptor GND port
 
 *Switch button pin to digital pin 7
 *Switch button pin ground to Arduino Analog GND
 *Switch button 220 resistor to Arduino Analog 5
*/

// include the library code:
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <FastLED.h>

//RGB 60 led strip.
#define wave_pin 6
#define wave_count 60
#define color__combination GRB
#define chip WS2812B
#define Bright 200
#define Frames_per_second 60

CRGB leds[wave_count];// used to turn on a random lights 

// initialize any needed LCD interface pin, bluetooth module pin, button switch, and LED. 
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//call each pin to the bluetooth adaptor 
 String store_voice_string = "";
 
//used for button switch strings that will be combine and be able to print out one string in the lcd.print instead of multiple printouts:
String bl1 = "";

//used for the bluetooth adaptor strings that will be combine and be able to print out one string in the lcd.print instead of multiple printouts:
String ba1 = "";
//ba2 and ba3 will be used for both switch button and bluetooth adapter. 
String ba2 = "";
String ba3 = "";

//assist in shifting words on the screen to the left
int toleft = 0;
int aidleft = 16;
//call a variable for the button switch
const int button_switch = 7;
int button_count = 0;
long random_number = 0;

//this will help change the led lights to different color pallete
CRGBPalette16 gPal;

//setup has changed to fit a neopixel led light. 
void setup(){
  // set up the LCD's number of columns and rows and input:
  lcd.begin(16, 2);

 // setting up the random colors for Disco pattern
  delay(3000); // sanity delay
  FastLED.addLeds<chip, wave_pin, color__combination>(leds, wave_count).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( Bright );
  gPal = HeatColors_p;
   
  //Button Switch:
  pinMode(button_switch, INPUT);

  //setting up the coloration for wave pattern and circular pattern
  FastLED.addLeds<NEOPIXEL, wave_pin>(leds, wave_count);

  // All outputs will be set up.

  Serial.begin(9600);

  randomSeed(analogRead(0));
  // Print a message that introduces the group(optional).
  lcd.print("Welcome to Voice Control Lights Blinking Cycle.");
}

//loop is not done yet please continue working on it.
void loop() {

 int readButton = digitalRead(button_switch);
 
//seeing if the bluetooth is connected and if it worked use the store_voice_string in void voice_ledfrequency(int voice_recoded){}.
if(Serial.available ()){  
  while (Serial.available()){
    delay(10);
    char c = Serial.read();
    if (c == '#'){break;}
    store_voice_string += c; // shorthand for voice = voice + c
  }
  lcd.setCursor(0,1);
  store_voice_string.toLowerCase();//makes the entire string have lower case character.
  ReceivingVocieFromPhone(store_voice_string);
  lcd.print(move_left(comblue()));
    }
else if (readButton == HIGH){
      readButton = LOW;
      button_count++;
      lcd.setCursor(0,1);
      blinkWave();
      
      lcd.print(move_left(comswitch()));
    } 
   
  delay(1000);//delay can be modified
}

//--------------------------------------------------------------------------------------------------

//Helper Functions: 
//if the switch button selects this function then
//let the button_count be randomly selected to choose one of the three patterns.
void RandomCycle(){
 ba2 = "Random selects ";
  button_count = random(3);// will select a number between 0 through 2.
  
  // figuring out which one to turn on. 
  if (button_count == 1){
    ba3 = "Wave pattern.";
    WaveCycle();
    }
  else if (button_count == 2){
    ba3 = "Circular pattern.";
    CircularWave();
   }
  else{
    ba3 = "Disco pattern.";
    DiscoWave();
  } 
 button_count = 0;
}
   
//this function is called when the switch button has been pressed. 
void blinkWave(){
//depending on the button_count determines which of the four wave patterns, circular pattern, disco pattern or random selection pattern will be activated. 
 
 //Depending on the button_count, determines which one will be turned on. 
   if (button_count == 1){
   bl1 = " turn on Wave pattern."; 
   WaveCycle();
    }
  else if (button_count == 2){
    bl1 = " turn on Circular pattern."; 
    CircularWave();
   }
  else if (button_count == 3){
    bl1 = " turn on Disco pattern."; 
    DiscoWave(); 
  }
  else{
    bl1 = " give random a chance to select which one will turn on."; 
    RandomCycle();
  }
}


// Depending on our command issued and the frequency of our voice either one of the //four WaveCycle(), CiccularCycle(), DiscoCycle() or RandomCycle(), will be called. 
void ReceivingVocieFromPhone(String voice_recoded){
  //this will turn the wave pattern on. 
  if (store_voice_string == "*turn on wave"){
    WaveCycle();
   
 	  ba1 = " Wave pattern!";
  }
  //this will turn the circular pattern on.
    else if(store_voice_string == "*turn on circular"){
    CircularWave();
   
    ba1 = " Circular pattern!";
  }
  //this will activate the disco pattern.
    else if(store_voice_string == "turn on disco"){
    DiscoWave();
    ba1 = " Disco pattern!";
  }
  //this will activate a random pattern.
  else if(store_voice_string == "turn on random"){
     
    ba1 = " a random pattern! ";
    RandomCycle();
  }
    ba1 = " not a command try again.";
}  

//this function will create a string that will be used in printing out a message on what was selected after our voice frequency command turns on one of the three selections. 
String comblue(){
  String combine = "";

  //test to see if ba1 is the string for random selection.
  if (ba1 == " a random pattern! "){
    combine = "Voice frequency detected and will power up " + ba1 + ba2 + ba3;
    }
   else{
    combine = "Voice frequency detected and will power up " + ba1;
    } 
    return combine;
  }

 //this function will create a string that will be used in printing out a message on what was selected after the button switch was pressed. 
String comswitch(){
  String combine = "";

  //test to see if ba1 is the string for random selection.
  if (bl1 == " a random pattern! "){
    combine = "Switch button has the power to " + bl1 + ba2 + ba3;
    }
   else{
    combine = "Switch button has the power to " + bl1;
    } 
    return combine;
  }
 

//helps display messages by scrolling to the left. 
String move_left(String personal){
  
  String shift = personal + "";//used to store in the string and create an ending.
  
  //shifting the characters starting from zero all the way to the end.
  toleft++;
  aidleft++;
  int strlength = shift.length();
  if (toleft > strlength){
    toleft = 0;
    aidleft = 16;
    }
  return (shift.substring(toleft, aidleft)); 
  } 
//-------------------------------------------------------------------------------------------------------------------

//Strip activation functions:

//Splitting the RGB 60 LED lights into two groups.
// code is part of Mark Kriegsman Multiple controller examples found 
// in the FastLed library made by Daniel Garcia. 
//the Mirroring method has been used in both the WaveCycle2() and CircularWave2().

//This function will test the first 30 neopixal lights in a wave pattern.
void WaveCycle(){

  int j = 153;//Alters Green
  int p = 255;//Alters Red
  int q = 51;// Alter Blue
  int i = 0; //Random color selection

  
  //This while loop will help in altering the color per each led light on the strip.  
  while(i < 254){
      p = p + i;
      j = j - i;
      q = q + i;
      
      //testing each variable to make sure they are not out of bounce.If they are we will rest them to default.
      if (i > 254){
        i = 0;
        } 
      if (j < 0 ){
        j = 255;
        }
     if (q > 254){
        q = 0;
       }
    if (p > 254){
        p = 0;
        }
    
    //this for loop is forward.
    for(int t = 0; t < 30; t++) {
     // setting up our current led light
      i++;
      p = p + i;
      j = j - i;
      q = q + i;
      
      leds[t] = CRGB((p),(j),(q));
      FastLED.show();
      // clear our current dot before we move on
      leds[t] = CRGB::Black;
      delay(1000);
    }
    
    //this for loop is to go backward. 
      for(int t = 29; t > -1; t = t - 1) {
      // setting our current led light.         
      i++;
      p = p + i;
      j = j - i;
      q = q + i;
      
      leds[t] = CRGB((p),(j),(q));
      FastLED.show();
      // clear our current dot before we move on
      leds[t] = CRGB::Black;
      delay(1000);
    }
      
  }
}

  //This function will assist to test the  last 30 neopixel lights
  //in a circular pattern. 
  void CircularWave(){
  int j = 153;//Alters Green
  int p = 255;//Alters Red
  int q = 51;// Alter Blue
  int i = 0; //Random color selection

  
  //This while loop will help in altering the color per each led light on the strip.  
  while(i < 254){
      p = p + i;
      j = j - i;
      q = q + i;
      
      //testing each variable to make sure they are not out of bounce.If they are we will rest them to default.
      if (i > 254){
        i = 0;
        } 
      if (j < 0 ){
        j = 255;
        }
     if (q > 254){
        q = 0;
       }
    if (p > 254){
        p = 0;
        }
    
    //this for loop is forward.
    for(int t = 30; t < 60; t++) {
     // setting up our current led light
      i++;
      p = p + i;
      j = j - i;
      q = q + i;  
      leds[t] = CRGB((p),(j),(q));
      FastLED.show();
      delay(100);
    }
    //this for loop is forward.
    for(int t = 30; t < 60; t++) {
     // setting up our current led light
      leds[t] = CRGB::Black;
      FastLED.show();
      
    }
    
    //this for loop is to go backward. 
      for(int t = 59; t > 29; t = t - 1) {
      // setting our current led light.         
      i++;
      p = p + i;
      j = j - i;
      q = q + i;
      
      leds[t] = CRGB((p),(j),(q));
      FastLED.show();
      delay(100);
    }
    //this for loop is backwards.
    for(int t = 59; t > 29; t = t-1) {
     // setting up our current led light
      leds[t] = CRGB::Black;
      FastLED.show();
      
    } 
  }
  }

  // for DisoWave()we will be using the same method as the other two with an added  while loop instead.
  void DiscoWave(){
    
    int i = 0; // the changer of lights 
    int p = 153; // holds the Green positions.
    int j = 255; // holds the Red postions.
    int q = 51; // holds the Blue positions. 

    
    //this while loop will change the color per iteration.
    while (i < 255){
      
      p = p + i;
      j = j - i;
      q = q + i;
      //testing each variable to make sure they are not out of bounce.If they are we will rest them to default.
      if (i > 254){
        i = 0;
        } 
      if (j < 0 ){
        j = 255;
        }
     if (q > 254){
        q = 0;
       }
    if (p > 254){
        p = 0;
        }
      
    //using the for loop to light up and turn off all 60 led lights.
      for(int t = 0; t < 60; t++) {
      // set each light to the same pattern
        leds[t] = CRGB(p,j,q);
      } 
      FastLED.show();
      //this for loop will turn all the lights off.
       for(int t = 0; t < 60; t++) {
      // set each light to the same pattern
        leds[t] = CRGB::Black;
      }  
      i++;
      delay(1000);//delay for a second 
    }
  } 
