#include <Servo.h> 

Servo servo;
int lectura =0;
void setup(){
 servo.attach(10);  
}

void loop(){
  lectura = servo.read();
  if(lectura ==0)
  servo.write(180);
  else
  servo.write(0);
  
  delay(1500);
  
}
