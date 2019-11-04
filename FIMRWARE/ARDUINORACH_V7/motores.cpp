#include <arduino.h>
#include "motores.h"

static int motord, motori;

void Motor_Init(void)
{
  pinMode(MOTORD_AIN1, OUTPUT);
  pinMode(MOTORD_AIN2, OUTPUT);
  pinMode(MOTORI_AIN1, OUTPUT);
  pinMode(MOTORI_AIN2, OUTPUT);

  TCCR2A =   B00000011;  // Fast PWM MODE - OCA DISCONETED
  TCCR2B =  (TCCR2A & B11110111) ;  
  
  TIMSK2 = (TIMSK2 & B11111000) | 0x07;
}

void SetSpeeds(int mi,int md)
{
 motord = md/4;
 motori = mi/4 ;

 int motor_derecho = md/4;
 int motor_izquierdo = mi/4;
 
 if(motor_derecho > 0)
  {
    
     if(motor_derecho>255)
         {
            motor_derecho = 255;
         }
     motor_derecho = 255 - motor_derecho;    
     OCR2B = motor_derecho;
  }
  else if(motor_derecho < 0)
    {
      
      if(motor_derecho<-255) //LIMITE GIRO ATRAS
         {
            motor_derecho = -255;
         }
         
     motor_derecho = 255 + motor_derecho;
     OCR2B = motor_derecho;
  }
 
 
 
  if(motor_izquierdo > 0)
  { 
    if(motor_izquierdo>255)
    {
      motor_izquierdo = 255;
    }
    motor_izquierdo = 255 - motor_izquierdo;
    OCR2A = motor_izquierdo;
  }
  else if (motor_izquierdo < 0)
    {
       if(motor_izquierdo<-255)
         {
            motor_izquierdo = -255;
         }
      motor_izquierdo = 255 + motor_izquierdo;     
      OCR2A = motor_izquierdo;
  }
  
}

ISR(TIMER2_OVF_vect){
    digitalWrite(MOTORI_AIN1, HIGH);
    digitalWrite(MOTORI_AIN2, HIGH);
    digitalWrite(MOTORD_AIN1, HIGH);
    digitalWrite(MOTORD_AIN2, HIGH);
}

ISR(TIMER2_COMPA_vect){
  
 if(motori > 0)
  { 
     digitalWrite(MOTORI_AIN1, LOW);
  }
  else if(motori < 0) 
  {
     digitalWrite(MOTORI_AIN2, LOW);
  }
}
 
ISR(TIMER2_COMPB_vect){
   if(motord > 0)
  { 
     digitalWrite(MOTORD_AIN1, LOW);
  }
  else if(motord < 0) 
  {
     digitalWrite(MOTORD_AIN2, LOW);
  }
}
