bool etat=LOW;

#define FRONT_RED 3
#define FRONT_GREEN 4
#define FRONT_BLUE 5
#define LEFT_RED 9
#define LEFT_GREEN 6
#define LEFT_BLUE 7

#define RIGHT_RED 10
#define RIGHT_GREEN 13
#define RIGHT_BLUE 12

#define FRONT_CAPTEUR 2
#define LEFT_CAPTEUR 8
#define RIGHT_CAPTEUR 11

void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(57600);



  Serial.println("Hello casquette !!");
  
  pinMode(FRONT_RED, OUTPUT);
  pinMode(FRONT_GREEN, OUTPUT);
  pinMode(FRONT_BLUE, OUTPUT);
  pinMode(LEFT_RED, OUTPUT);
  pinMode(LEFT_GREEN, OUTPUT);
  pinMode(LEFT_BLUE, OUTPUT);
  pinMode(RIGHT_RED, OUTPUT);
  pinMode(RIGHT_GREEN, OUTPUT);
  pinMode(RIGHT_BLUE, OUTPUT);
  
  pinMode(FRONT_CAPTEUR, INPUT); 
  pinMode(LEFT_CAPTEUR, INPUT); 
  pinMode(RIGHT_CAPTEUR, INPUT); 
  
  
  digitalWrite(FRONT_RED, LOW);
  digitalWrite(FRONT_GREEN, LOW);
  digitalWrite(FRONT_BLUE, LOW);
  digitalWrite(LEFT_RED, LOW);
  digitalWrite(LEFT_GREEN, LOW);
  digitalWrite(LEFT_BLUE, LOW);
  digitalWrite(RIGHT_RED, LOW);
  digitalWrite(RIGHT_GREEN, LOW);
  digitalWrite(RIGHT_BLUE, LOW);
  

}

void loop() // run over and over
{
  /*
  int n=detect_tir();
  if(n == 123)
  {
      etat = !etat;
      
      digitalWrite(FRONT_RED, HIGH);
      digitalWrite(LEFT_RED, HIGH);
      digitalWrite(RIGHT_RED, HIGH);
      delay(200);
      digitalWrite(FRONT_RED, LOW);
      digitalWrite(LEFT_RED, LOW);
      digitalWrite(RIGHT_RED, LOW);
      delay(200);
      digitalWrite(FRONT_RED, HIGH);
      digitalWrite(LEFT_RED, HIGH);
      digitalWrite(RIGHT_RED, HIGH);
      delay(200);
      digitalWrite(FRONT_RED, LOW);
      digitalWrite(LEFT_RED, LOW);
      digitalWrite(RIGHT_RED, LOW);
      delay(200);
      digitalWrite(FRONT_RED, HIGH);
      digitalWrite(LEFT_RED, HIGH);
      digitalWrite(RIGHT_RED, HIGH);
      delay(200);
      digitalWrite(FRONT_RED, LOW);
      digitalWrite(LEFT_RED, LOW);
      digitalWrite(RIGHT_RED, LOW);
     
  }
  else Serial.println(n);
  
  */
  
  static unsigned long pause_front=0;
  if(detect_tir_front()==123) { pause_front=millis();}
  if(millis() < (pause_front+500)) { digitalWrite(FRONT_RED, HIGH); digitalWrite(FRONT_GREEN, LOW); }
  else { digitalWrite(FRONT_RED, LOW); digitalWrite(FRONT_GREEN, HIGH); }
  
  
  
  
  static unsigned long pause_right=0;
  if(detect_tir_right()==123) { pause_right=millis();}
  if(millis() < (pause_right+500)) { digitalWrite(RIGHT_RED, HIGH); digitalWrite(RIGHT_GREEN, LOW); }
  else { digitalWrite(RIGHT_RED, LOW); digitalWrite(RIGHT_GREEN, HIGH); }
  
  static unsigned long pause_left=0;
  if(detect_tir_left()==123) { pause_left=millis();}
  if(millis() < (pause_left+500)) { digitalWrite(LEFT_RED, HIGH); digitalWrite(LEFT_GREEN, LOW); }
  else { digitalWrite(LEFT_RED, LOW); digitalWrite(LEFT_GREEN, HIGH); }
  
//  if(detect_tir_left()==123)
    
}

#define TEMPS_HIGH 1000


inline int detect_tir()
{
  unsigned long debut = micros();
  
  //detect 8 * LOW
  while((micros()-debut) < (8*TEMPS_HIGH))
  {
    if(digitalRead(FRONT_CAPTEUR)) debut = micros();
  }
  
  //attente de l'état haut
  while(!digitalRead(FRONT_CAPTEUR));
  
  //reset du compteur
  debut = micros();
  float nb_bit = 0;
  byte id=0;
  
  while((micros()-debut) < ((nb_bit+0.2)*TEMPS_HIGH));
  while((micros()-debut) < ((nb_bit+0.8)*TEMPS_HIGH))
  {
    if(digitalRead(FRONT_CAPTEUR)==LOW) return -1;
  }
  
  for(int iB=0;iB<8;iB++)
  {
    nb_bit++;
    while((micros()-debut) < ((nb_bit+0.2)*TEMPS_HIGH));
    while((micros()-debut) < ((nb_bit+0.8)*TEMPS_HIGH))
    {
      if(digitalRead(FRONT_CAPTEUR)) return -1;
    }
    
    nb_bit++;
    id = id*2;
    
    while((micros()-debut) < ((nb_bit+0.2)*TEMPS_HIGH));
    bool val = digitalRead(FRONT_CAPTEUR);
    while((micros()-debut) < ((nb_bit+0.8)*TEMPS_HIGH))
    {
      if(digitalRead(FRONT_CAPTEUR) != val) return -1;
    }
    
    if(!val) id++;
  }
  
  //bit de fin
  nb_bit++;
  
  while((micros()-debut) < ((nb_bit+0.2)*TEMPS_HIGH));
  while((micros()-debut) < ((nb_bit+0.8)*TEMPS_HIGH))
  {
    if(digitalRead(FRONT_CAPTEUR)) return -1;
  }

  return id;
}


inline int detect_tir_front()
{
 static int etat=0; //init
 static unsigned long debut=0;
 static int vbit;
 static int id=0;
 int mpin = FRONT_CAPTEUR;
 
 if(etat == 0) //attente des 8*LOW
 {
   id=0;
   if(digitalRead(mpin)) debut = micros();
   if((micros()-debut) >= (8*TEMPS_HIGH)) etat=1;
 }
 else if(etat == 1)
 {
   if((micros()-debut) > (9.2*TEMPS_HIGH) && (micros()-debut) < (9.8*TEMPS_HIGH) && digitalRead(mpin)==LOW) { etat=0; debut = micros(); }
   if((micros()-debut) >= (10*TEMPS_HIGH)) etat=10;
 }
 
 
 //8 bits
 else if(etat>=10 && etat <=25)
 {
   if((etat%2) == 0)
   {
     if((micros()-debut) > ((etat+0.2)*TEMPS_HIGH) && (micros()-debut) < ((etat+0.8)*TEMPS_HIGH) && digitalRead(mpin)==HIGH) { etat=0; debut = micros(); }
     if((micros()-debut) >= ((etat+1)*TEMPS_HIGH)) etat++;
     vbit=-1;
   }
   else
   {
     if((micros()-debut) > ((etat+0.2)*TEMPS_HIGH) && (micros()-debut) < ((etat+0.8)*TEMPS_HIGH))
     {
       if(vbit==-1) vbit = digitalRead(mpin);
       else
       {
         if(digitalRead(mpin) != vbit) { etat=0; debut = micros(); }
       }
     }
     if((micros()-debut) >= ((etat+1)*TEMPS_HIGH))
     {
       etat++;
       id = id*2;
       if(!vbit) id++;
     }
   }
 }
 
 else if(etat == 26)
 {
   if((micros()-debut) > (18.2*TEMPS_HIGH) && (micros()-debut) < (18.8*TEMPS_HIGH) && digitalRead(mpin)==HIGH) { etat=0; debut = micros(); }
   if((micros()-debut) >= (19*TEMPS_HIGH)) etat=27;
 }
 
 if(etat == 27)
 {
   etat=0; debut = micros();
   return id;
 }
 else return -1;
}

inline int detect_tir_left()
{
 static int etat=0; //init
 static unsigned long debut=0;
 static int vbit;
 static int id=0;
 int mpin = LEFT_CAPTEUR;
 
 if(etat == 0) //attente des 8*LOW
 {
   id=0;
   if(digitalRead(mpin)) debut = micros();
   if((micros()-debut) >= (8*TEMPS_HIGH)) etat=1;
 }
 else if(etat == 1)
 {
   if((micros()-debut) > (9.2*TEMPS_HIGH) && (micros()-debut) < (9.8*TEMPS_HIGH) && digitalRead(mpin)==LOW) { etat=0; debut = micros(); }
   if((micros()-debut) >= (10*TEMPS_HIGH)) etat=10;
 }
 
 
 //8 bits
 else if(etat>=10 && etat <=25)
 {
   if((etat%2) == 0)
   {
     if((micros()-debut) > ((etat+0.2)*TEMPS_HIGH) && (micros()-debut) < ((etat+0.8)*TEMPS_HIGH) && digitalRead(mpin)==HIGH) { etat=0; debut = micros(); }
     if((micros()-debut) >= ((etat+1)*TEMPS_HIGH)) etat++;
     vbit=-1;
   }
   else
   {
     if((micros()-debut) > ((etat+0.2)*TEMPS_HIGH) && (micros()-debut) < ((etat+0.8)*TEMPS_HIGH))
     {
       if(vbit==-1) vbit = digitalRead(mpin);
       else
       {
         if(digitalRead(mpin) != vbit) { etat=0; debut = micros(); }
       }
     }
     if((micros()-debut) >= ((etat+1)*TEMPS_HIGH))
     {
       etat++;
       id = id*2;
       if(!vbit) id++;
     }
   }
 }
 
 else if(etat == 26)
 {
   if((micros()-debut) > (18.2*TEMPS_HIGH) && (micros()-debut) < (18.8*TEMPS_HIGH) && digitalRead(mpin)==HIGH) { etat=0; debut = micros(); }
   if((micros()-debut) >= (19*TEMPS_HIGH)) etat=27;
 }
 
 if(etat == 27)
 {
   etat=0; debut = micros();
   return id;
 }
 else return -1;
}

inline int detect_tir_right()
{
 static int etat=0; //init
 static unsigned long debut=0;
 static int vbit;
 static int id=0;
 int mpin = RIGHT_CAPTEUR;
 
 if(etat == 0) //attente des 8*LOW
 {
   id=0;
   if(digitalRead(mpin)) debut = micros();
   if((micros()-debut) >= (8*TEMPS_HIGH)) etat=1;
 }
 else if(etat == 1)
 {
   if((micros()-debut) > (9.2*TEMPS_HIGH) && (micros()-debut) < (9.8*TEMPS_HIGH) && digitalRead(mpin)==LOW) { etat=0; debut = micros(); }
   if((micros()-debut) >= (10*TEMPS_HIGH)) etat=10;
 }
 
 
 //8 bits
 else if(etat>=10 && etat <=25)
 {
   if((etat%2) == 0)
   {
     if((micros()-debut) > ((etat+0.2)*TEMPS_HIGH) && (micros()-debut) < ((etat+0.8)*TEMPS_HIGH) && digitalRead(mpin)==HIGH) { etat=0; debut = micros(); }
     if((micros()-debut) >= ((etat+1)*TEMPS_HIGH)) etat++;
     vbit=-1;
   }
   else
   {
     if((micros()-debut) > ((etat+0.2)*TEMPS_HIGH) && (micros()-debut) < ((etat+0.8)*TEMPS_HIGH))
     {
       if(vbit==-1) vbit = digitalRead(mpin);
       else
       {
         if(digitalRead(mpin) != vbit) { etat=0; debut = micros(); }
       }
     }
     if((micros()-debut) >= ((etat+1)*TEMPS_HIGH))
     {
       etat++;
       id = id*2;
       if(!vbit) id++;
     }
   }
 }
 
 else if(etat == 26)
 {
   if((micros()-debut) > (18.2*TEMPS_HIGH) && (micros()-debut) < (18.8*TEMPS_HIGH) && digitalRead(mpin)==HIGH) { etat=0; debut = micros(); }
   if((micros()-debut) >= (19*TEMPS_HIGH)) etat=27;
 }
 
 if(etat == 27)
 {
   etat=0; debut = micros();
   return id;
 }
 else return -1;
}
