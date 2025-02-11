#define IR_1 0x77E1BA6C // 
#define IR_2 0x77E1B06C // 
#define IR_3 0x77E1E06C //  >
#define IR_4 0x77E1106C //  <
#define IR_5 0x77E1406C //  IN

#define AUTO_GAIN 1       
#define VOL_THR 35         
#define LOW_PASS 30        
#define DEF_GAIN 80        
#define FHT_N 128        
#define LOG_OUT 1
#include <FHT.h> 
#include <Wire.h>
#include <MsTimer2.h>
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>
#include <EEPROM.h>
#include <TDA7313.h>
#include <boarddefs.h>
#include <IRremote.h>
  IRrecv irrecv(12); //  IR 
  TDA7313 tda;decode_results ir;
  Encoder myEnc(10, 11);//CLK, DT 
  LiquidCrystal_I2C lcd(0x3f,16,2);
   #define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
   #define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
   byte posOffset[16] = {2, 3, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30};
   unsigned long newPosition,time,time1,oldPosition  = -999;
   int vol,w1,w2=1,gr1,gr2,vol_d,menu,w,z,z0,z1,bass_d,bass,treb_d,treb,i,menu1,in,gain,gain1,gain2,gain3,loud,lr,rr,lf,rf;
   unsigned long gainTimer;
   byte maxValue, maxValue_f,spek;
   float k = 0.1;
   byte gain_sp = DEF_GAIN;  
   int i1,yyy;
    byte ur[16],urr[16]; ///////////////// new code
 
void setup(){ 
  Serial.begin(9600);Wire.begin();irrecv.enableIRIn(); //  IR
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight(); 
   sbi(ADCSRA, ADPS2);
   cbi(ADCSRA, ADPS1);
   sbi(ADCSRA, ADPS0);
  analogReference(INTERNAL);
   pinMode(A0,INPUT);// encoder switch
   pinMode(8,INPUT);// in switch
   pinMode(A1,INPUT);// in
     MsTimer2::set(3, to_Timer);MsTimer2::start();
     vol = EEPROM.read(0);bass = EEPROM.read(1)-7;treb = EEPROM.read(2)-7;in = EEPROM.read(3);
     gain1 = EEPROM.read(4);gain2 = EEPROM.read(5);gain3 = EEPROM.read(6);loud = EEPROM.read(7);
     lr = EEPROM.read(8);rr = EEPROM.read(9);lf = EEPROM.read(10);rf = EEPROM.read(11);
      if(in==0){gain=gain1;}
      if(in==1){gain=gain2;}
      if(in==2){gain=gain3;}
       lcd.setCursor(0,0);lcd.print("   Trick-tech    ");
      lcd.setCursor(0,1);lcd.print(" Please Subscribe");
      
      delay(2000);lcd.clear();audio();
}
 
void to_Timer(){newPosition = myEnc.read()/4;}
 
void loop(){
  if (newPosition != oldPosition) {if(spek==1){cl();menu=0;}}
  if(spek==0){
      byte a1[8] = {0b00000,0b10101,0b10101,0b10101,0b10101,0b10101,0b10101,0b00000};
      byte a2[8] = {0b00000,0b10100,0b10100,0b10100,0b10100,0b10100,0b10100,0b00000};
      byte a3[8] = {0b00000,0b10000,0b10000,0b10000,0b10000,0b10000,0b10000,0b00000};
      lcd.createChar(0,a1);lcd.createChar(1,a2);lcd.createChar(2,a3);
    }else{
      byte v1[8] = {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b01110};
      byte v2[8] = {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b01110, 0b01110};
      byte v3[8] = {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b01110, 0b01110, 0b01110};
      byte v4[8] = {0b00000, 0b00000, 0b00000, 0b00000, 0b01110, 0b01110, 0b01110, 0b01110};
      byte v5[8] = {0b00000, 0b00000, 0b00000, 0b01110, 0b01110, 0b01110, 0b01110, 0b01110};
      byte v6[8] = {0b00000, 0b00000, 0b01110, 0b01110, 0b01110, 0b01110, 0b01110, 0b01110};
      byte v7[8] = {0b00000, 0b01110, 0b01110, 0b01110, 0b01110, 0b01110, 0b01110, 0b01110};
      byte v8[8] = {0b01110, 0b01110, 0b01110, 0b01110, 0b01110, 0b01110, 0b01110, 0b01110};
      lcd.createChar(0, v1);lcd.createChar(1, v2);lcd.createChar(2, v3);lcd.createChar(3, v4);lcd.createChar(4, v5);lcd.createChar(5, v6);lcd.createChar(6, v7);lcd.createChar(7, v8);
      }
  ////////////////// ----------///////////////////////////////////////////
 if ( irrecv.decode( &ir )) {Serial.print("0x");Serial.println( ir.value,HEX);time1=millis();time=millis();w=1;w1=1;if(spek==1){menu=0;cl();};irrecv.resume();}// IR приемник
  if(ir.value==0){gr1=0;gr2=0;}
  if((analogRead(A0)<900)&&menu1==0){menu++;cl();myEnc.write(0);i++;time=millis();w=1;w2=1;if(menu>2){menu=0;}}// меню
  if((analogRead(A0)>900)){i=0;}if(i>3){cl();lcd.print("     MENU 2     ");delay(500);cl();menu1=1;menu=100;}
  if((analogRead(A0)<900)&&menu1>0){menu1++;cl();myEnc.write(0);time=millis();w=1;w2=1;if(menu1>6){cl();menu1=0;menu=0;}}// меню 1
  
     if(ir.value==IR_1){menu++;gr1=0;gr2=0;cl();time=millis();w=1;w2=1;if(menu>2){menu=0;}}// меню кнопка вверх
     if(ir.value==IR_2){menu--;gr1=0;gr2=0;cl();time=millis();w=1;w2=1;if(menu<0){menu=2;}}// меню кнопка вниз
     
     if(ir.value==IR_5){in++;gr1=0;gr2=0;cl();time=millis();w=1;w2=1;if(in>2){in=0;}audio();lcd.setCursor(0,0);lcd.print("IN ");lcd.print(in+1);delay(1000);}//  кнопка IN
     
  //////////////////////////////////////// Volume 0...63 db //////////////////////////////////////
 if(menu==0){
     if(ir.value==IR_3){vol++;gr1=1;gr2=0;cl1();w2=1;if(vol<0){vol=0;}if(vol>63){vol=63;}audio();}//  > 
     if(ir.value==0xFFFFFFFF and gr1==1){vol++;gr2=0;cl1();w2=1;if(vol<0){vol=0;}if(vol>63){vol=63;}audio();}//  >>>>>>
     if(ir.value==IR_4){vol--;gr1=0;gr2=1;cl1();w2=1;if(vol<0){vol=0;}if(vol>63){vol=63;}audio();}//  <
     if(ir.value==0xFFFFFFFF and gr2==1){vol--;gr1=0;cl1();w2=1;if(vol<0){vol=0;}if(vol>63){vol=63;}audio();}//  <<<<<<
   
    if (newPosition != oldPosition) {
    oldPosition = newPosition;
    vol=vol+newPosition;myEnc.write(0);newPosition=0;time=millis();w=1;w2=1;if(vol>63){vol=63;}if(vol<0){vol=0;}audio();}
 
  lcd.setCursor(0,0);lcd.print("Volume ");lcd.print(" ");
  lcd.print(-78.75+(float)vol*1.25,2);lcd.print(" ");lcd.setCursor(14,0);lcd.print("dB");vol_d=vol-15;
   if(w2==1){
   for(z=0,z0=0,z1=0;z<=vol_d;z++,z1++){if(z1>2){z1=0;z0++;}
   if(z1==1){lcd.setCursor(z0,1);lcd.write((uint8_t)0);lcd.setCursor(z0+1,1);lcd.print("   ");}}
   if(z1==3){lcd.setCursor(z0,1);lcd.write((uint8_t)1);}
   if(z1==2){lcd.setCursor(z0,1);lcd.write((uint8_t)2);}w2=0;}}
//////////////////////////////////////////////////////////////////////////////////////////////////////
 
 /////////////////////// Bass -14...14 db ///////////////////
   if(menu==1){ 
     if(ir.value==IR_3){bass++;gr1=1;gr2=0;cl1();w2=1;if(bass>7){bass=7;}if(bass<-7){bass=-7;}audio();}//  > 
     if(ir.value==0xFFFFFFFF and gr1==1){bass++;gr2=0;cl1();w2=1;if(bass>7){bass=7;}if(bass<-7){bass=-7;}audio();}// >>>>>>
     if(ir.value==IR_4){bass--;gr1=0;gr2=1;cl1();w2=1;if(bass>7){bass=7;}if(bass<-7){bass=-7;}audio();}// кнопка < 
     if(ir.value==0xFFFFFFFF and gr2==1){bass--;gr1=0;cl1();w2=1;if(bass>7){bass=7;}if(bass<-7){bass=-7;}audio();}//  <<<<<<
   
    if (newPosition != oldPosition) {
    oldPosition = newPosition;
    bass=bass+newPosition;myEnc.write(0);newPosition=0;time=millis();w=1;w2=1;if(bass>7){bass=7;}if(bass<-7){bass=-7;}audio();} 
   lcd.setCursor(0,0);lcd.print("Bass    ");
   lcd.print(" ");lcd.print(bass*2);lcd.print(" ");lcd.setCursor(13,0);lcd.print("dB");bass_d=bass*2+15;
   if(w2==1){
   for(z=0,z0=0,z1=0;z<=bass_d;z++,z1++){if(z1>2){z1=0;z0++;}
   if(z1==1){lcd.setCursor(z0+3,1);lcd.write((uint8_t)0);lcd.setCursor(z0+1+3,1);lcd.print("   ");}}
   if(z1==3){lcd.setCursor(z0+3,1);lcd.write((uint8_t)1);}
   if(z1==2){lcd.setCursor(z0+3,1);lcd.write((uint8_t)2);}w2=0;}}
/////////////////////////////////////////////////////////////////////////////////////
 
 /////////////////////// trebel  -14...14 db ///////////////////
   if(menu==2){
     if(ir.value==IR_3){treb++;gr1=1;gr2=0;cl1();w2=1;if(treb>7){treb=7;}if(treb<-7){treb=-7;}audio();}// кнопка > 
     if(ir.value==0xFFFFFFFF and gr1==1){treb++;gr2=0;cl1();w2=1;if(treb>7){treb=7;}if(treb<-7){treb=-7;}audio();}// >>>>>>
     if(ir.value==IR_4){treb--;gr1=0;gr2=1;cl1();w2=1;if(treb>7){treb=7;}if(treb<-7){treb=-7;}audio();}// кнопка < 
     if(ir.value==0xFFFFFFFF and gr2==1){treb--;gr1=0;cl1();w2=1;if(treb>7){treb=7;}if(treb<-7){treb=-7;}audio();}//  <<<<<<
   
    if (newPosition != oldPosition) {
    oldPosition = newPosition;
    treb=treb+newPosition;myEnc.write(0);newPosition=0;time=millis();w=1;w2=1;if(treb>7){treb=7;}if(treb<-7){treb=-7;}audio();} 
   lcd.setCursor(0,0);lcd.print("Treble  ");
   lcd.print(" ");lcd.print(treb*2);lcd.print(" ");lcd.setCursor(13,0);lcd.print("dB");treb_d=treb*2+15;
   if(w2==1){
   for(z=0,z0=0,z1=0;z<=treb_d;z++,z1++){if(z1>2){z1=0;z0++;}
   if(z1==1){lcd.setCursor(z0+3,1);lcd.write((uint8_t)0);lcd.setCursor(z0+1+3,1);lcd.print("   ");}}
   if(z1==3){lcd.setCursor(z0+3,1);lcd.write((uint8_t)1);}
   if(z1==2){lcd.setCursor(z0+3,1);lcd.write((uint8_t)2);}w2=0;}}
/////////////////////////////////////////////////////////////////////////////////////
 
  if(digitalRead(8)==HIGH){in++;;cl();myEnc.write(0);time=millis();w=1;w2=1;if(in>2){in=0;}if(in<0){in=2;}audio();lcd.setCursor(0,0);lcd.print("IN ");lcd.print(in+1);delay(1000);}
 
///////////////////////////////////// GAIN //////////////////////////////////////////
 
 if(menu1==1){
      if(in==0){gain=gain1;}
      if(in==1){gain=gain2;}
      if(in==2){gain=gain3;}
 
    if (newPosition != oldPosition) {
    oldPosition = newPosition;
    gain=gain+newPosition;myEnc.write(0);newPosition=0;lcd.clear();time=millis();w=1;w2=1;if(gain>3){gain=3;}if(gain<0){gain=0;}audio();} 
 
      if(in==0){gain1=gain;}
      if(in==1){gain2=gain;}
      if(in==2){gain3=gain;}
    
  lcd.setCursor(0,0);lcd.print(" INPUT ");lcd.print(in+1);lcd.setCursor(0,1);lcd.print(" Gain ");lcd.print((float)gain*3.75);lcd.print(" dB ");
 }
 /////////////////////////////// LOUDNESS //////////////////////////////////////////
 
  if(menu1==2){
    if (newPosition != oldPosition) {
    oldPosition = newPosition;
    loud=loud+newPosition;myEnc.write(0);newPosition=0;lcd.clear();time=millis();w=1;w2=1;if(loud>1){loud=0;}if(loud<0){loud=1;}audio();}
    lcd.setCursor(0,0);lcd.print("LOUDNESS ");if(loud==1){lcd.print("ON ");}else{lcd.print("OFF");}
} 
////////////////////////////////// att  LR //////////////////////////////////////////////
 
   if(menu1==3){
     if (newPosition != oldPosition) {
    oldPosition = newPosition;
    lr=lr-newPosition;myEnc.write(0);newPosition=0;time=millis();w=1;w2=1;if(lr>31){lr=31;}if(lr<0){lr=0;}audio();} 
    lcd.setCursor(0,0);lcd.print("Speaker   ");lcd.print((float)-lr*1.25,2);lcd.print("  ");
    lcd.setCursor(0,1);lcd.print("LR");lcd.setCursor(14,1);lcd.print("dB");
     if(w2==1){
   for(z=0,z0=0,z1=0;z<=-lr+31;z++,z1++){if(z1>2){z1=0;z0++;}
   if(z1==1){lcd.setCursor(z0+3,1);lcd.write((uint8_t)0);lcd.setCursor(z0+1+3,1);lcd.print("   ");}}
   if(z1==3){lcd.setCursor(z0+3,1);lcd.write((uint8_t)1);}
   if(z1==2){lcd.setCursor(z0+3,1);lcd.write((uint8_t)2);}w2=0;}}
 ////////////////////////////////////////////////////////////////////////////////////////
 ////////////////////////////////// att  LF //////////////////////////////////////////////
 
   if(menu1==4){
     if (newPosition != oldPosition) {
    oldPosition = newPosition;
    lf=lf-newPosition;myEnc.write(0);newPosition=0;time=millis();w=1;w2=1;if(lf>31){lf=31;}if(lf<0){lf=0;}audio();} 
    lcd.setCursor(0,0);lcd.print("Speaker   ");lcd.print((float)-lf*1.25,2);lcd.print("  ");
    lcd.setCursor(0,1);lcd.print("LF");lcd.setCursor(14,1);lcd.print("dB");
    if(w2==1){
   for(z=0,z0=0,z1=0;z<=-lf+31;z++,z1++){if(z1>2){z1=0;z0++;}
   if(z1==1){lcd.setCursor(z0+3,1);lcd.write((uint8_t)0);lcd.setCursor(z0+1+3,1);lcd.print("   ");}}
   if(z1==3){lcd.setCursor(z0+3,1);lcd.write((uint8_t)1);}
   if(z1==2){lcd.setCursor(z0+3,1);lcd.write((uint8_t)2);}w2=0;}}
 ////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////// att  RF //////////////////////////////////////////////
 
   if(menu1==5){
     if (newPosition != oldPosition) {
    oldPosition = newPosition;
    rf=rf-newPosition;myEnc.write(0);newPosition=0;time=millis();w=1;w2=1;if(rf>31){rf=31;}if(rf<0){rf=0;}audio();} 
    lcd.setCursor(0,0);lcd.print("Speaker   ");lcd.print((float)-rf*1.25,2);lcd.print("  ");
    lcd.setCursor(0,1);lcd.print("RF");lcd.setCursor(14,1);lcd.print("dB");
    if(w2==1){
   for(z=0,z0=0,z1=0;z<=-rf+31;z++,z1++){if(z1>2){z1=0;z0++;}
   if(z1==1){lcd.setCursor(z0+3,1);lcd.write((uint8_t)0);lcd.setCursor(z0+1+3,1);lcd.print("   ");}}
   if(z1==3){lcd.setCursor(z0+3,1);lcd.write((uint8_t)1);}
   if(z1==2){lcd.setCursor(z0+3,1);lcd.write((uint8_t)2);}w2=0;}}
 ////////////////////////////////////////////////////////////////////////////////////////
   ////////////////////////////////// att  RR //////////////////////////////////////////////
 
   if(menu1==6){
     if (newPosition != oldPosition) {
    oldPosition = newPosition;
    rr=rr-newPosition;myEnc.write(0);newPosition=0;time=millis();w=1;w2=1;if(rr>31){rr=31;}if(rr<0){rr=0;}audio();} 
    lcd.setCursor(0,0);lcd.print("Speaker   ");lcd.print((float)-rr*1.25,2);lcd.print(" ");
    lcd.setCursor(0,1);lcd.print("RR");lcd.setCursor(14,1);lcd.print("dB");
    if(w2==1){
   for(z=0,z0=0,z1=0;z<=-rr+31;z++,z1++){if(z1>2){z1=0;z0++;}
   if(z1==1){lcd.setCursor(z0+3,1);lcd.write((uint8_t)0);lcd.setCursor(z0+1+3,1);lcd.print("   ");}}
   if(z1==3){lcd.setCursor(z0+3,1);lcd.write((uint8_t)1);}
   if(z1==2){lcd.setCursor(z0+3,1);lcd.write((uint8_t)2);}w2=0;}}
 ////////////////////////////////////////////////////////////////////////////////////////
 
 
 if(millis()-time>10000 && w==1){
     EEPROM.update(0,vol);
     EEPROM.update(1,bass+7);
     EEPROM.update(2,treb+7);
     EEPROM.update(3,in);
     EEPROM.update(4,gain1);EEPROM.update(5,gain2);EEPROM.update(6,gain3);
     EEPROM.update(7,loud);
     EEPROM.update(8,lr);EEPROM.update(9,rr);EEPROM.update(10,lf);EEPROM.update(11,rf);
      menu1=0;lcd.clear();w=0,w2=1;menu=100;spek=1;}
     
////////////////////////////////////////////////////////////////////////////////////////////
     if(spek==1){
  analyzeAudio();   // 
  for (int pos = 0; pos < 16; pos++) {   //
   
    if (fht_log_out[posOffset[pos]] > maxValue) maxValue = fht_log_out[posOffset[pos]];

    lcd.setCursor(pos, 0);

    
    int posLevel = map(fht_log_out[posOffset[pos]], LOW_PASS, gain_sp, 0, 15);
    posLevel = constrain(posLevel, 0, 15);

///////////////////// new code 
    urr[pos] = posLevel;
         if(urr[pos]<ur[pos]){ur[pos]=ur[pos]-1;delay(2);}
         else{
         ur[pos] = posLevel;}
///////////////////// end new code  
  
    if (posLevel > 7) {             
      lcd.write((uint8_t)posLevel - 8);    
      lcd.setCursor(pos, 1);          
      lcd.write((uint8_t)7);       
  }
    else {                         
      lcd.print(" ");                 
      lcd.setCursor(pos, 1);          
      lcd.write((uint8_t)posLevel);        
    }
  }

  if (AUTO_GAIN) {
    maxValue_f = maxValue * k + maxValue_f * (1 - k);
    if (millis() - gainTimer > 1500) {     
      
      if (maxValue_f > VOL_THR) gain_sp = maxValue_f;

      
      else gain_sp = 100;
      gainTimer = millis();
    }
  }else{gain_sp = DEF_GAIN;}
  }
}
 
 void cl(){ir.value=0;delay(300);spek=0;lcd.clear();}
 void cl1(){ir.value=0;delay(100);spek=0;}
void audio(){
  tda.setVolume(vol); //  0...63
  tda.setAttLR(lr);   //  LR 0...31
  tda.setAttRR(rr);   //  RR 0...31
  tda.setAttLF(lf);   //  LF 0...31
  tda.setAttRF(rf);   //  RF 0...31
  tda.setSwitch(in,loud,gain); //
  tda.setBass(bass);    //  -7...+7
  tda.setTreble(treb);  //   -7...+7
}

void analyzeAudio() {   
    cli();
for (int i = 0 ; i < FHT_N ; i++) {
    int sample = analogRead(A1);
    fht_input[i] = sample; // put real data into bins
  }
    fht_window(); // window the data for better frequency response
    fht_reorder(); // reorder the data before doing the fht
    fht_run(); // process the data in the fht
    fht_mag_log(); // take the output of the fht 
    sei();
}
