/* param pise
19-6-24
roatry edit
wipsa electronics

*/
//16x2 LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>//SDA = A4, SCL = A5
LiquidCrystal_I2C lcd(0x27,16,2); 
int tic;
// Define rotary encoder pins
#define ENC_A 2
#define ENC_B 3
#define SW A0


unsigned long _lastIncReadTime = micros(); 
unsigned long _lastDecReadTime = micros(); 
int _pauseLength = 25000;
int _fastIncrement = 10;

volatile int counter = 0;

void setup() {

// initialize the lcd 
  lcd.init();           
  lcd.backlight();

   lcd.setCursor(0,0); //Defining position to write from first row, first column .
  lcd.print("    XRANGER"); //0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
  lcd.setCursor(0,1); //second line, 1st block
  lcd.print("       7"); //You can write 16 Characters per line .
  delay(3000); //wait 3 sec

  // Set encoder pins and attach interrupts
  pinMode(SW, INPUT);
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC_A), read_encoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_B), read_encoder, CHANGE);
  // Start the serial monitor to show output
  Serial.begin(115200);
}

void loop() {
  static int lastCounter = 0;

  // If count has changed print the new value to serial
  if(counter != lastCounter){
    Serial.println(counter);
    lastCounter = counter;
  }

////////////////////////////////////////////////////////////////////////////////    volume menue
  if(counter==1)
  {
  lcd.setCursor(0,0); //Defining position to write from first row, first column .
  lcd.print("volume");

  if (digitalRead(SW)==1)
   { tic=1;
   delay(200);
   while(tic==1)
   {
    lcd.setCursor(0,1); //second line, 1st block
    lcd.print(counter); //You can write 16 Characters per line .
    if (digitalRead(SW)==1)
      { tic=0;
        counter=1;  
        lcd.clear();}
   }}}

////////////////////////////////////////////////////////////////////////////////    bass menue

  if(counter==2)
  {
  lcd.setCursor(0,0); //Defining position to write from first row, first column .
  lcd.print("bass");

  if (digitalRead(SW)==1)
   { tic=1;
   delay(200);
   while(tic==1)
   {
    lcd.setCursor(0,1); //second line, 1st block
    lcd.print(counter); //You can write 16 Characters per line .
    if (digitalRead(SW)==1)
      { tic=0;
        counter=2;  }
   }}}




////////////////////////////////////////////////////////////////////////////////    tribal menue

  if(counter==3)
  {
  lcd.setCursor(0,0); //Defining position to write from first row, first column .
  lcd.print("tribal");

  if (digitalRead(SW)==1)
   { tic=1;
   delay(200);
   while(tic==1)
   {
    lcd.setCursor(0,1); //second line, 1st block
    lcd.print(counter); //You can write 16 Characters per line .
    if (digitalRead(SW)==1)
      { tic=0;
        counter=3;  }
   }}}   

  
}

void read_encoder() {
  // Encoder interrupt routine for both pins. Updates counter
  // if they are valid and have rotated a full indent
 
  static uint8_t old_AB = 3;  // Lookup table index
  static int8_t encval = 0;   // Encoder value  
  static const int8_t enc_states[]  = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0}; // Lookup table

  old_AB <<=2;  // Remember previous state

  if (digitalRead(ENC_A)) old_AB |= 0x02; // Add current state of pin A
  if (digitalRead(ENC_B)) old_AB |= 0x01; // Add current state of pin B
  
  encval += enc_states[( old_AB & 0x0f )];

  // Update counter if encoder has rotated a full indent, that is at least 4 steps
  if( encval > 3 ) {        // Four steps forward
    int changevalue = 1;
    if((micros() - _lastIncReadTime) < _pauseLength) {
      changevalue = _fastIncrement * changevalue; 
    }
    _lastIncReadTime = micros();
    counter = counter + changevalue;              // Update counter
    encval = 0;
  }
  else if( encval < -3 ) {        // Four steps backward
    int changevalue = -1;
    if((micros() - _lastDecReadTime) < _pauseLength) {
      changevalue = _fastIncrement * changevalue; 
    }
    _lastDecReadTime = micros();
    counter = counter + changevalue;              // Update counter
    encval = 0;
  }
} 