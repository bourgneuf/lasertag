int led = 6;
int led_rouge = 13;
int gachette = A3;

boolean toggle0 = 0;
boolean enable_frequence = 0;

void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  Serial.println("Hello !!");
  
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
  
  pinMode(led_rouge, OUTPUT);
  
  pinMode(gachette, INPUT);

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

bool tir_pret = true;

void loop() // run over and over
{
  if(tir_pret == true && digitalRead(gachette))
  {
    send_id(123);
    digitalWrite(led_rouge, HIGH);
    tir_pret = false;
  }
  if(!digitalRead(gachette)) tir_pret = true;
  
  send_id(0);
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
