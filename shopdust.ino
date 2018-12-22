#include <Wire.h> 
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header


hd44780_I2Cexp lcd;
int PM = 6;
int RELAY = 3;
int BUTTON = 9;
unsigned long sampletime=10000;

unsigned long duration;
unsigned long starttime;
unsigned long lpo=0;
unsigned long timediff;
float threshold = 3.0;
float ratio = 0;

int fan = 2; // 3 States :: 0=OFF / 1=ON / 2=AUTO

const int LCD_COLS = 16;
const int LCD_ROWS = 2;
int buttonState = 0;         // variable for reading the pushbutton status
int buttonTimer;
int longPressTime = 250;
bool buttonPressed = false;
bool override = false;


void setup() {

  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.clear();
  lcd.print("Choking Index")

  Serial.begin(9600);
  
  pinMode(PM,INPUT);
  pinMode(BUTTON,INPUT);
  pinMode(RELAY,OUTPUT);

  starttime=millis();  // Start the timer

  /* // Make sure relay is off...that's right off.  
     // I know, it doesn't make sense.
     // Depends on if your relay
     // Change this to low if it seems you are in backward world
 */
  
  digitalWrite(RELAY,HIGH); 
}

void loop() {
  // put your main code here, to run repeatedly:

  buttonState = digitalRead(BUTTON);

  if ((buttonState == LOW) && ButtonPressed = true) {
      // button was pressed but not longpressed
      override = true;
      lcd.clear();
      if (fan = 0) {
        lcd.print("OVERRIDE Fan ON");
        fan = 1;
        digitalWrite(RELAY, LOW);
      } else {
        lcd.print("OVERRIDE Fan OFF");
        fan = 0;
        digitalWrite(RELAY, HIGH);
      }
      
  }
  if (buttonState == HIGH) {
    // SET Longpress timer
      if (buttonPressed = false) {
        buttonPressed = true;
        buttonTimer = millis();
      }
      if ((millis() - buttonTimer > longPressTime) {
        longPressActive = true;
        fan = 2; // Back to Auto 
        ovveride = false;
        buttonPressed = false;
      }
  }

  // DISPLAY CURRENT CRUD LEVEL 
  
  duration = pulseIn(PM, LOW);
  lpo=lpo+duration;
  if((millis() - starttime) > duration) {
    ratio=lpo/(displaytime*10.0);
    lcd.setCursor(6,1);
    lcd.print("        ");
    lcd.setCursor(6,1);
    lcd.print(ratio);
    starttime=millis();
    lpo=0;

  }

  // SET FAN LEVEL
  if ((fan = 2) && (ratio > threshold)) {
    digitalWrite(RELAY,LOW);
  } else {
    digitalWrite(RELAY, HIGH);
  }

}
