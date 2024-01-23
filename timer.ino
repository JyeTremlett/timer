#include <Keypad.h>
#include <LiquidCrystal_I2C.h>



/*
This code was written for an Arduino Uno,  using a matrix keypad and I2C capable/adapted 
16x2 LCD screen. This code defines a simple timer, where upon startup, the user can 
select a time to count from, and then press start. At this point, the timer will count
down to 0, and display a "finished" message. 
*/

// pin on arduino used for reset button
const int startOrResetPin = 11;



// ---- KEYPAD VARIABLES ----
// rows and columns for the matrix keypad
const byte ROWS = 4;
const byte COLS = 4;

// pins on arduino used for keypad input
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

// defining matrix keypad values
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// initialise keypad object
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);



// create lcd object
LiquidCrystal_I2C lcd(32, 16, 2);

// variables for recording timer data
char timerMaxDigits = 2;  // max number of digits available to timeInput
int digitCount = 0;		    // number of digits
int timeInput = 0;		    // time to count from as entered by the user

// array to put user input in before calculating timeInput 
int digits[] = {0, 0};

// ----------------------------------------------------------------------

void setup() {
  // set up lcd 
  lcd.backlight();
  lcd.init();
  
  // set pin for resetting or starting timer to output
  pinMode(startOrResetPin, INPUT);
  
  Serial.begin(9600);
}

// ----------------------------------------------------------------------

void loop() {

  // init lcd and print instructions
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Timer Ready");

  readKeyStrokes();       // get input from user and put into digits[] arrray
  calcTimeInput();        // calculate timeInput based on user input
  countDown();            // start countdown 
  printFinishedMessage(); // print message when countdown finished
}

// ----------------------------------------------------------------------

// function for reading user input
void readKeyStrokes() {
  
  bool buttonState = LOW;
  int magnitude;

  while(buttonState != HIGH) {

    // get key value if pressed
    char customKey = customKeypad.getKey();

    // if key pressed and not reached max digits
    if(customKey && digitCount < timerMaxDigits) {

      int input = customKey-48;			  // convert char to int
      digits[digitCount] = input;     // add input to array of digits
      lcd.setCursor(digitCount, 1);		// set cursor
      lcd.print(input);  				      // print input
      digitCount++;						        // increment digitCount
    }
	    
    // check buttonState
    buttonState = digitalRead(startOrResetPin);
  }
}

// ----------------------------------------------------------------------

void calcTimeInput() {
  
  int digit;
  int magnitude;
  int reverseCounter = digitCount-1;
 

  for(int i = 0; i < digitCount; i++) {
    // convert to appropriate magnitude
    magnitude = pow(10, reverseCounter);
    Serial.println(magnitude);
    digit = digits[i];
    digit *= magnitude;
    timeInput += digit;
    
    reverseCounter--;
  }
  
}

// ----------------------------------------------------------------------

void countDown() {
  int curr = timeInput;
  unsigned long time;
  bool buttonState = LOW;
  
  while(curr >= 0 && buttonState != HIGH) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(curr--);
    
    time = millis();
    while(millis() - time < 1000) {
      delay(10); //TEMP FOR TINKERCAD
    }
    
    buttonState = digitalRead(startOrResetPin);
  }
}

// ----------------------------------------------------------------------

void printFinishedMessage() {
  
  int x = 5;
  unsigned long time;
  
  while(x-- >= 0) {
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("TIMER FINISHED");
    

    time = millis();
    while(millis() - time < 200) {
      delay(10); //TEMP FOR TINKERCAD;
    }
  }
}

// ----------------------------------------------------------------------

// quick exponentiation function
int pow(int base, int exponent) {
  
  int result = base;
  
  if(exponent == 0)
  	result = 1;
  else {
    result = base;
  
    while(exponent > 1) {
      result *= base;
      exponent--;
    }  
  }
  return result;
}
    
    
    
    
    
    
    
    
    
    
    