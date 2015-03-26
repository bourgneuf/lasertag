int led = 6;
boolean toggle0 = 0;
boolean enable_frequence = 0;

void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  Serial.println("Hello !!");
  
  pinMode(led, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(led, HIGH);
  
  pinMode(A3, INPUT);

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
  if(digitalRead(A3))
  {
    send_id(123);
    digitalWrite(13, HIGH);
    delay(50);
  }
  else digitalWrite(13, LOW);
}

#define TEMPS_HIGH 1000
void send_id(byte id)
{
  String myStr = String(id,BIN);
  int zeros = 8 - myStr.length();
  
  unsigned long debut = micros();
  
/*  if(debut > 4294937) return; */ //Ne pas tir quand le compteur est proche de l'overflow
  
  //9 * HIGH
  enable_frequence = 1;
  while((micros()-debut) < 9*TEMPS_HIGH);
  
  //1 * LOW (+9)
  enable_frequence = 0;
  while((micros()-debut) < 10*TEMPS_HIGH);
  
  //les zeros du debut
  int nb_bit = 10;

  for (int i=0; i<zeros; i++)
  {
    //niveau HIGH
    nb_bit++;
    enable_frequence = 1;
    while((micros()-debut) < nb_bit*TEMPS_HIGH);
    
    //niveau LOW
    nb_bit++;
    enable_frequence = 0;
    while((micros()-debut) < nb_bit*TEMPS_HIGH);
  }
  
  //les bits restants
  for (int i=0; i<myStr.length(); i++)
  {
    //niveau HIGH
    nb_bit++;
    enable_frequence = 1;
    while((micros()-debut) < nb_bit*TEMPS_HIGH);
    
    //niveau du bit
    nb_bit++;
    if(myStr[i]=='0')
    {
      enable_frequence = 0;
      while((micros()-debut) < nb_bit*TEMPS_HIGH);
    }
    else
    {
      enable_frequence = 1;
      while((micros()-debut) < nb_bit*TEMPS_HIGH);
    }
  }
  
  
  //niveau HIGH de la fin
  nb_bit++;
  enable_frequence = 1;
  while((micros()-debut) < nb_bit*TEMPS_HIGH);
  
  //remise a zero
  enable_frequence = 0;
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
