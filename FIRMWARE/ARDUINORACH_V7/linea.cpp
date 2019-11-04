#include "linea.h"

#include <arduino.h>

unsigned char pins[NUM_SENSORS] = {2,A0,A1,A2,A3,13,12,5}; // SENSORES DEL 0 AL 8 QTR8
int pesos[NUM_SENSORS] = {-35,-25,-15,-5,5,15,25,35}; // SENSORES DEL 0 AL 8 QTR8
unsigned int sensorValues_max[NUM_SENSORS];
unsigned int sensorValues_min[NUM_SENSORS];

int sensores_b;

void Reset_Calibracion(void)
{
   for(int x=0; x<NUM_SENSORS; x++)
   {
     sensorValues_max[x] = 0; 
     sensorValues_min[x] = TIMEOUT;
   }
}


int Calibrar_Color_Linea(unsigned int* sensorValuesp)
{  
   int color_fondo = 4;
   int rango_comparacion = 10; //CAMBIAR PARA CALIBRAR

 while(color_fondo == 4)
 {
     Leer_sensores (sensorValuesp);
     delay(500);
     Leer_sensores (sensorValuesp);
     
    //fondo NEGRO - LINEA BLANCA
    if(sensorValuesp[0] > rango_comparacion && sensorValuesp[NUM_SENSORS-1] > rango_comparacion  )
    {
      color_fondo = 0;
    }
     //fondo BLANCO - LINEA NEGRA 
    if(sensorValuesp[0] < rango_comparacion && sensorValuesp[NUM_SENSORS-1] < rango_comparacion  )
    {
      color_fondo = 1;
    }
 }
     
return color_fondo;
}


void Calibrar_Sensores(unsigned int* sensorValuesp, int colorlinea)
{   
    Leer_sensores (sensorValuesp);
    
    for(int x=0; x<NUM_SENSORS; x++)
     {
       if(sensorValuesp[x] > sensorValues_max[x])
       {
          sensorValues_max[x] = sensorValuesp[x];
       }
       
       if(sensorValuesp[x] < sensorValues_min[x])
       {
          sensorValues_min[x] = sensorValuesp[x];
       }
     }
}

 
int Leer_linea(unsigned int* sensorValuesp, int linea_anterior, int colorlinea)
{   
   int linea, suma, activos;
   
    Leer_sensores (sensorValuesp);
    
    suma = 0;
    linea = 0;
    activos = 0;
    sensores_b = 0;
    
    for(int x=0; x<NUM_SENSORS; x++)
     {
       int rango_comparacion = (sensorValues_max[x] - sensorValues_min[x])/2; //15
       
       if(colorlinea)
       {
             if(sensorValuesp[x] > rango_comparacion) // NEGRA LINEA NEGRA - FONDO BLANCO
             {
                suma += pesos[x];
                sensores_b += 1 << x;
                activos ++;
             }
       }
       else
       {
             if(sensorValuesp[x] < rango_comparacion) // ES MENOR ES POR QUE ES BLANCO LINEA-- FONDO NEGRO
             {
                suma += pesos[x];
                sensores_b += 1 << x;
                activos ++;
             }       
       }
     }
     
     if(activos > 0)
     {
         linea = suma/activos;
     }
     else
     {
         
          if(linea_anterior > 10)
          {linea = 60;}
          else if(linea_anterior < -10)
          {linea = -60;}
          else
          {linea = 0;}
     
     }
     return linea;
     
}


void Leer_sensores (unsigned int* sensorValuesp)
{
  int sensor_time = 0;
  int val;
  
      for(int x=0; x<NUM_SENSORS; x++)
       {
              sensorValuesp[x] = TIMEOUT;
       }
   
     for(int x=0; x<NUM_SENSORS; x++)
     { 
       pinMode(pins[x], INPUT);
     }

     do{
            for(int x=0; x<NUM_SENSORS; x++)
             {
                  val = digitalRead(pins[x]);
                  if( (val==LOW) && (sensorValuesp[x] == TIMEOUT) )
                  {
                    sensorValuesp[x]=sensor_time;
                  }
             }
             sensor_time++;
     }while(sensor_time < TIMEOUT); 

      for(int x=0; x<NUM_SENSORS; x++)
       {
              pinMode(pins[x], OUTPUT);
              digitalWrite(pins[x], HIGH);
       }
}
