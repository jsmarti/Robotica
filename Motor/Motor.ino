 //Arduino PWM Speed Control：
//Motor 1
int E1 = 5;  
int M1 = 4; 
//Motor 2
int E2 = 6;                      
int M2 = 7;

//Pin Sensor
int sensorCorriente = 0;
 
void setup() 
{ 
    pinMode(M1, OUTPUT);   
    pinMode(M2, OUTPUT); 
    Serial.begin(9600);
    
} 
 
void loop() 
{ 
  //PWM
  int value = 200;
  //Sensado de corriente
  /*double voltajeMedido = analogRead(sensorCorriente);
  double conversion = voltajeMedido*5/1023;
  double corrienteSensada = conversion*1.9821-0.9997;
  */
  delay(1000);

    
  digitalWrite(M1,HIGH);
  digitalWrite(M2,HIGH);
  analogWrite(E1,value);
  analogWrite(E2,value);
  delay(1000);
  digitalWrite(M1,LOW);
  digitalWrite(M2,LOW);
  analogWrite(E1,value);
  analogWrite(E2,value);
  delay(1000);

  

}
