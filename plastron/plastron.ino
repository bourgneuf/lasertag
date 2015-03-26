bool etat=LOW;

void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(57600);



  Serial.println("Hello plastron !!");
  
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);

  
  pinMode(A0, INPUT); 
  
  
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  digitalWrite(5, LOW);
  digitalWrite(4, LOW);
  

}

void loop() // run over and over
{
  int n=detect_tir();
  if(n == 123)
  {
      etat = !etat;
      digitalWrite(5, etat);
  }
  else Serial.println(n);
    
  
  /*
  unsigned long diff = micros();
  
  while((micros()-diff) < (8*1000)) // attente de l'état du haut du debut
  {
    if(digitalRead(A0)) diff = micros();
  }
  
  while(!digitalRead(A0)); //attente de l'état bas
  
  delayMicroseconds(500);
  
  byte val=0;
  
  for(int iB=0;iB<8;iB++)
  {
    val = val*2;
    delay(1);
    /*
    if(digitalRead(A0) == 1)
    {
      Serial.println("erreur de bit");
      Serial.println(iB);
      return;
    }
    *//*
    delay(1);
    if(!digitalRead(A0)) val++;
  }
  
  
  Serial.println("Coup de fusil !!");
  Serial.println(val);
  */
}

#define TEMPS_HIGH 1000


inline int detect_tir()
{
  unsigned long debut = micros();
  
  //detect 8 * LOW
  while((micros()-debut) < (8*TEMPS_HIGH))
  {
    if(digitalRead(A0)) debut = micros();
  }
  
  //attente de l'état haut
  while(!digitalRead(A0));
  
  digitalWrite(7, HIGH); //tempo
  
  //reset du compteur
  debut = micros();
  float nb_bit = 0;
  byte id=0;
  
  while((micros()-debut) < ((nb_bit+0.2)*TEMPS_HIGH));
  while((micros()-debut) < ((nb_bit+0.8)*TEMPS_HIGH))
  {
    if(digitalRead(A0)==LOW) return -1;
  }
  
  for(int iB=0;iB<8;iB++)
  {
    nb_bit++;
    while((micros()-debut) < ((nb_bit+0.2)*TEMPS_HIGH));
    while((micros()-debut) < ((nb_bit+0.8)*TEMPS_HIGH))
    {
      if(digitalRead(A0)) return -1;
    }
    
    digitalWrite(7, LOW); //tempo
    
    nb_bit++;
    id = id*2;
    
    while((micros()-debut) < ((nb_bit+0.2)*TEMPS_HIGH));
    bool val = digitalRead(A0);
    while((micros()-debut) < ((nb_bit+0.8)*TEMPS_HIGH))
    {
      if(digitalRead(A0) != val) return -1;
    }
    
    digitalWrite(7, HIGH); //tempo
    
    if(!val) id++;
  }
  
  //bit de fin
  nb_bit++;
  
  while((micros()-debut) < ((nb_bit+0.2)*TEMPS_HIGH));
  while((micros()-debut) < ((nb_bit+0.8)*TEMPS_HIGH))
  {
    if(digitalRead(A0)) return -1;
  }
  digitalWrite(7, LOW); //tempo
  
  return id;
}







/*
inline int detect_tir()
{
  unsigned long debut = micros();
  
  //detect 8 * LOW
  while((micros()-debut) < (8*TEMPS_HIGH))
  {
    if(digitalRead(A0)) debut = micros();
  }
  
  //attente de l'état haut
  while(!digitalRead(A0));
  
  digitalWrite(7, HIGH); //tempo
  
  //reset du compteur
  debut = micros();
  float nb_bit = 0.5;
  byte id=0;
  
  for(int iB=0;iB<8;iB++)
  {
    nb_bit++;
    while((micros()-debut) < (nb_bit*TEMPS_HIGH));
    
    digitalWrite(7, LOW); //tempo
    
    if(digitalRead(A0)) return -1;
    
    nb_bit++;
    id = id*2;
    while((micros()-debut) < (nb_bit*TEMPS_HIGH));
    
    digitalWrite(7, HIGH); //tempo
    
    if(!digitalRead(A0)) id++;
  }
  
  //bit de fin
  nb_bit++;
  while((micros()-debut) < (nb_bit*TEMPS_HIGH));
  digitalWrite(7, LOW); //tempo
  if(digitalRead(A0)) return -1;
  
  return id;
}
*/
