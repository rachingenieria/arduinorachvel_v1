#include <arduino.h>
#include <Servo.h>
#include "turbina.h"

//TURBINA -- ESC FUNCIONA COMO SERVO
Servo ESC; 

void Turbina_Init()
{
//INICIO DE PUERTO DE ESC
  ESC.attach(TURBINA);
  ESC.writeMicroseconds(TURBINA_DC_INIT); //1000 = 1ms MINIMO
  delay(1000);  
  ESC.writeMicroseconds(2000); //2ms MAXIMO
  delay(2000);
  ESC.writeMicroseconds(TURBINA_DC_INIT); //ESC READY 
  delay(2000); 
}


void Turbina_Start()
{
  ESC.writeMicroseconds(1250); //TEST TURBINA
  delay(2000);
  ESC.writeMicroseconds(TURBINA_DC_INIT); //1000 = 1ms
  delay(1000);
}

void Turbina_Vel(int vel)
{
    ESC.writeMicroseconds(TURBINA_DC_INIT + vel*10); //1000 = 1ms
}
