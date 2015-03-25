
//Pin Sensor
int sensorCorriente = 0;
 
void setup() 
{ 
    Serial.begin(9600);  
} 
 
void loop() 
{ 

  //Sensado de corriente
  double voltajeMedido = analogRead(sensorCorriente);
  double conversion = voltajeMedido*5/1023;
  double corrienteSensada = (conversion + 0.9997)/1.9821;
  
  Serial.println(corrienteSensada);
  
  if(corrienteSensada > 2.5){
    Serial.println('Robot bloqueado');
  }
  

}


