
//Salida de trigger

int trigger = 3;

//Entrada del sensor

int sensorDistancia = 2;

void setup(){
  //Configuracion de pines de salida y entrada
  pinMode(sensorDistancia,INPUT);
  pinMode(trigger,OUTPUT);
  Serial.begin(9600);
}


void loop(){
  
  //Activa el trigger para el envio de la onda ultrasonica
  digitalWrite(trigger,HIGH);
  delay(5);
  digitalWrite(trigger,LOW);
  
  //Lectura del sensor y medicion de ancho de pulso
  double val = pulseIn(sensorDistancia,HIGH);
  //Estimacion
  double d = 0.0169*val - 0.9637;
  Serial.println(d);
  
}
  


