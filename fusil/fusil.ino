#include "U8glib.h"


int led = 6;
int led_rouge = 13;
int gachette = A3;

boolean toggle0 = 0;
boolean enable_frequence = 0;
bool tir_pret;

U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);

const uint8_t img_logo[] PROGMEM = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,31,0,0,0,0,0,0,0,0,0,0,0,0,0,0,48,63,128,0,0,0,0,0,0,0,0,0,0,0,0,0,32,127,192,0,0,0,0,0,0,0,0,0,0,0,0,0,127,255,128,0,0,0,7,192,0,0,0,0,0,3,254,0,0,255,128,0,0,0,4,160,0,0,0,0,0,6,2,0,0,127,0,0,0,0,4,160,0,0,0,0,0,15,1,0,0,63,0,0,0,0,4,160,0,0,0,0,0,31,129,0,0,30,0,0,0,0,3,32,0,0,0,0,0,63,128,128,0,14,0,0,0,0,0,0,0,0,0,0,0,63,192,128,0,4,0,0,255,240,3,192,0,0,0,0,0,31,255,192,0,0,0,0,255,240,5,64,0,0,0,0,0,31,192,0,0,0,0,0,255,240,5,64,0,0,0,0,0,15,159,240,0,0,0,0,0,0,5,64,0,0,0,0,0,15,48,16,0,0,0,0,0,0,0,128,0,0,0,0,0,6,120,8,0,0,0,0,0,0,16,0,0,0,0,0,0,4,252,8,0,0,0,0,255,240,16,0,0,0,0,0,0,1,252,4,0,0,0,0,255,240,31,192,0,0,0,0,0,3,254,4,0,0,0,0,255,240,16,0,0,0,0,0,0,1,255,254,0,0,0,0,0,0,16,0,0,0,0,7,252,1,255,1,254,127,0,0,0,0,0,0,0,0,0,15,248,0,252,0,255,63,128,0,0,0,0,0,0,0,0,31,240,0,248,0,127,159,192,0,0,112,2,0,0,0,0,31,240,0,120,0,127,159,192,0,0,112,4,0,0,0,0,63,224,0,96,0,63,207,224,0,0,112,4,0,0,0,0,63,224,0,32,0,63,207,224,0,0,112,2,0,0,0,0,127,192,0,0,0,31,231,240,0,255,240,7,192,0,0,0,96,64,0,0,0,16,36,16,0,255,240,0,0,0,0,0,32,96,0,32,0,48,76,32,0,255,240,3,64,0,0,0,48,32,0,112,0,32,72,32,0,0,0,5,64,0,0,0,16,48,0,88,0,96,152,64,0,0,0,5,64,0,0,0,24,16,0,204,0,64,144,64,0,0,0,5,64,0,0,0,12,24,0,134,0,129,32,128,0,192,48,3,128,0,0,0,7,252,1,131,1,254,127,0,0,224,112,0,0,0,0,0,0,0,1,1,254,0,0,0,0,240,240,4,128,0,0,0,0,0,3,1,252,0,0,0,0,121,224,5,64,0,0,0,0,0,1,131,252,0,0,0,0,63,192,5,64,0,0,0,0,0,4,195,248,0,0,0,0,31,128,5,64,0,0,0,0,0,6,103,248,0,0,0,0,63,192,2,64,0,0,0,0,0,11,55,240,0,0,0,0,121,224,0,0,0,0,0,0,0,9,159,240,0,0,0,0,240,240,3,192,0,0,0,0,0,16,192,0,0,0,0,0,224,112,5,64,0,0,0,0,0,16,127,192,0,0,0,0,192,48,5,64,0,0,0,0,0,32,63,128,0,4,0,0,0,0,5,64,0,0,0,0,0,48,127,128,0,14,0,0,0,0,0,128,0,0,0,0,0,24,127,0,0,26,0,0,0,0,0,0,0,0,0,0,0,12,255,0,0,51,0,0,0,0,0,64,0,0,0,0,0,7,254,0,0,97,0,0,0,0,0,64,0,0,0,0,0,3,254,0,0,193,128,0,0,0,0,64,0,0,0,0,0,0,0,0,127,128,128,0,0,0,31,192,0,0,0,0,0,0,0,0,63,192,192,0,0,0,0,0,0,0,0,0,0,0,0,0,63,193,128,0,0,0,0,0,0,0,0,0,0,0,0,0,31,227,0,0,0,0,0,0,0,0,0,0,0,0,0,0,31,230,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,252,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};
int imgX;
int imgY;

void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  Serial.println("Hello !!");
  
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
  
  pinMode(led_rouge, OUTPUT);
  
  pinMode(gachette, INPUT);
  
  tir_pret = true;
  
  imgX = imgY = 0;

  cli();//stop interrupts
  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 210;// = (16*10^6) / (1000*8) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1 prescaler
  TCCR1B |= (1 << CS10);// | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();//allow interrupts
}


void loop() // run over and over
{
  digitalWrite(led_rouge, HIGH);
  
  if(tir_pret == true && digitalRead(gachette))
  {
    send_id(123);
    tir_pret = false;
  }
  if(!digitalRead(gachette)) tir_pret = true;
  
  send_id(0);
  
  
  
  
  u8g.drawBitmapP( (imgX*8), imgY, 1, 1, (img_logo+(imgY+16)+imgX));
  imgX++;
  if(imgX>16) { imgX=0; imgY++; }
  if(imgY>64) imgY=0;
  
  digitalWrite(led_rouge, LOW);
}




#define TEMPS_HIGH 1000
int send_id(byte id)
{
  static int etat=0;
  static unsigned long debut;
  static String myStr;
  static int bits;
  
  if(etat==0)
  {
    if(id != 0)
    {
      myStr = String(id,BIN);
      while(myStr.length() < 8) myStr = "0"+myStr; //myStr.concat(String('0'),myStr);
      
      Serial.println(myStr);
      debut = micros();
      etat=1;
    }
    else return 0;
  }
  
  else if(etat==1)
  {
    enable_frequence = 1;
    if((micros()-debut) >= 9*TEMPS_HIGH)
    {
      etat=2;
    }
  }
  
  else if(etat==2)
  {
    enable_frequence = 0;
    if((micros()-debut) >= 10*TEMPS_HIGH)
    {
      etat=10;
      bits=-1;
    }
  }
  
  else if(etat>=10 && etat <= 25)
  {
    if(etat%2 == 0)
    {
      enable_frequence = 1;
      if((micros()-debut) >= (etat+1)*TEMPS_HIGH) { bits++; etat++; }
    }
    else
    {
      if(myStr[bits]=='0') enable_frequence = 0;
      else enable_frequence = 1;
      if((micros()-debut) >= (etat+1)*TEMPS_HIGH) etat++;
    }
  }
  
  else if(etat == 26)
  {
    enable_frequence = 1;
    if((micros()-debut) >= (etat+1)*TEMPS_HIGH)
    {
      etat=0;
      enable_frequence = 0;
    }
  }

  return 1;
}


ISR(TIMER1_COMPA_vect)
{
  if(enable_frequence)
  {
    if(toggle0)
      PORTD |= 0b1000000;
    else
      PORTD &= 0b0111111;
  }
  else PORTD |= 0b1000000;
  
  toggle0 = !toggle0;
}
