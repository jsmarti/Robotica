int sensorCorriente = 2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {

   double voltajeMedido = analogRead(sensorCorriente);
   double conversion = voltajeMedido*5/1023;
   
   Serial.println("Voltaje Medido: "+String(conversion));
    delay(1000);
}
