#include <stdio.h>
#include <string.h>

#include "eeprom.h"
#include "motores.h"
#include "linea.h"
#include "turbina.h"
#include "oled.h"
#include "api.h"
#include "rachvel.h"

//------------------------------------------------------------------------------------//




int error[10];
int position_line;
extern int sensores_b;
unsigned int sensorValues[NUM_SENSORS];
//--------------------------------------------------------------------------------------//
//PARAMETROS del Control del Velocista
//AQUI SE MODIFICAN LOS PARAMETROS DE COMPETENCIA
rachvel Rachvel;

 
int   VELCIDAD_MAXIMA        = 30;       //Velocidad Maxima (entre 0 y 255)
int   CTE_PROPORCIONAL       = 10;      //Constante de Control Proporcional (ente 1 y 20)
int   CTE_DERIVATIVA         = 30;      //Constante de Control Diferencia (ente 1 y 20)                                                                                                                                                                                                                                    
int   V_TURBINA              = 60;       //Velocidad Turbina en Competencia (entre 90 y 150)

//Variables para Control Adicional
float power_difference, power_difference_ext;
float power_difference_ant;
int detect_recta_ant, detect_recta;
char stat_sw = 0; 
 
void setup()
{
  int val;
    
  Eeprom_read();
  if(Rachvel.ver == FIRMWARE_VERSION)// DATOS CORRECTOS y CARGADOS
  {
    //Rachvel.setupconfig(VELCIDAD_MAXIMA,CTE_PROPORCIONAL,CTE_DERIVATIVA,CTE_INTEGRAL,V_TURBINA);
  }
  else
  {
    Rachvel.setupconfig(VELCIDAD_MAXIMA,CTE_PROPORCIONAL,CTE_DERIVATIVA,V_TURBINA); //valres por DEFECTO
    Eeprom_save();
  }


  
  pinMode(SW2_UP,INPUT);
  pinMode(SW4_ENTER,INPUT);
  pinMode(SW3_DOWN,INPUT);
  
  digitalWrite(SW2_UP, HIGH);
  digitalWrite(SW4_ENTER, HIGH);
  digitalWrite(SW3_DOWN, HIGH);
        
  Serial.begin(115200);

  pinMode(ON_RF,INPUT);

  Motor_Init();
  SetSpeeds(0,0);
  oled_init();

  int cursorz = 0;
  int menuactivo = 0;
  val = 1;

    // MENU DE CONFIGURACION
  do{
    
    int battery = analogRead(BATTERY);
    float bat = battery*(11)*5.0/1024;
    
     Oled_Api(Rachvel.vavg, Rachvel.kpg, Rachvel.kdg,  Rachvel.pmw_t, cursorz, menuactivo, bat, Rachvel.ver  );
     if(digitalRead(SW3_DOWN) == LOW)
     {
      delay(20);
            if(cursorz<4 && menuactivo == 0)
            {
               cursorz ++;
            }
            else if ( menuactivo == 1)
            {
              if(cursorz == 1)
               Rachvel.vavg = Rachvel.vavg - 1;
              if(cursorz == 2)
               Rachvel.kpg = Rachvel.kpg - 1;
             if(cursorz == 3)
               Rachvel.kdg = Rachvel.kdg - 1;
             if(cursorz == 4)
               Rachvel.pmw_t = Rachvel.pmw_t - 10;

            }
     }
     if(digitalRead(SW2_UP) == LOW)
     {
      delay(20);
             if(cursorz > 0 && menuactivo == 0)
            {
               cursorz --;
            }
            else if ( menuactivo == 1)
            {
              if(cursorz == 1)
                Rachvel.vavg = Rachvel.vavg + 1;
              if(cursorz == 2)
                Rachvel.kpg = Rachvel.kpg + 1;
             if(cursorz == 3)
                Rachvel.kdg = Rachvel.kdg + 1;
              if(cursorz == 4)
                Rachvel.pmw_t = Rachvel.pmw_t + 10;
            }
     }

     if(digitalRead(SW4_ENTER)== LOW)
     {
      delay(20);
            if( menuactivo == 0)
            {
               menuactivo = 1;
               if(cursorz == 0)
               {
                val = 0;
                Eeprom_save();
                Oled_SAVED();
               }
            }
            else if( menuactivo == 1)
            {
               menuactivo = 0;
            }
     }

     //RECIBIR DATOS POR BLUETOOTH
     Serial_command();
  }while (val);
  
  //APAGAR LOS LEDS
  pinMode(LED2, OUTPUT);
  pinMode(LED1, OUTPUT);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED1, HIGH);  
  

//-------------Instrucciones para Empezar a hacer la Calibracion de Sensores--------------------------------------//

  
  //ESPERA QUE SE RETIRE EL DEDO DEL SW1
  delay(1000); 
  
  // Enciende el leds para indicar que se esta calibrando.
  digitalWrite(LED2, LOW);
  digitalWrite(LED1, LOW);
  
  //GIRA MIENTRA CALIBRA
  SetSpeeds(-450, 450);

  Reset_Calibracion();
  Rachvel.colorlinea = Calibrar_Color_Linea(sensorValues);


  int tiempo_cal = 250;
  while(tiempo_cal--)
  {
      Calibrar_Sensores(sensorValues,Rachvel.colorlinea);
      delay(5);
  }
  
 SetSpeeds(0, 0);

  // Apaga el led para indicar que se termino la calibracion.
  digitalWrite(LED2, HIGH);     
  digitalWrite(LED1, HIGH); 
 
 //stop Motors
  SetSpeeds(0,0);
    
  delay(200);
  digitalWrite(LED2, HIGH);     
  digitalWrite(LED1, HIGH);
  delay(200);
  digitalWrite(LED2, LOW);     // Parpadeo para indicar que el robot esta listo.
  digitalWrite(LED1, LOW);
  delay(200);                  // Parpadeo para indicar que el robot esta listo.
  digitalWrite(LED2, HIGH);     
  digitalWrite(LED1, HIGH);    // Parpadeo para indicar que el robot esta listo.
  delay(200);
  digitalWrite(LED2, LOW);     // Parpadeo para indicar que el robot esta listo.
  digitalWrite(LED1, LOW);
  delay(200);
  //---------------------------Fin Calibracion de Sensores----------------------------------------------------//
  
  // SECUENCIA PARA VERIFICAR SI LA CALIBRACION ESTA BIEN
  // TERMINA CUANDO SE OPRIME SW1
  val = digitalRead(SW4_ENTER);   
 do{
     val = digitalRead(SW4_ENTER);
      Oled_Sensores(sensorValues,position_line,Rachvel.colorlinea);
      position_line = Leer_linea(sensorValues,position_line ,Rachvel.colorlinea ); // leemos posicion de la linea en la variable position
  }while (val == HIGH);
  
  Oled_Calibracion();
  val = digitalRead(SW4_ENTER);  
  while (val == HIGH )
  {    
   
   position_line = Leer_linea(sensorValues,position_line ,Rachvel.colorlinea ); // leemos posicion de la linea en la variable position
   //Oled_Sensores(sensorValues,position_line);
   
 if (position_line < -20)
 {
     digitalWrite(LED2, HIGH);
     digitalWrite(LED1, LOW); 
  }
  else  if (position_line > 20 )
  {
     digitalWrite(LED2, LOW);
     digitalWrite(LED1, HIGH);    
  }
  else  
  {
   digitalWrite(LED2, HIGH);
   digitalWrite(LED1, HIGH);    
  }

  error[9]=error[8];
  error[8]=error[7];
  error[7]=error[6];
  error[6]=error[5];
  error[5]=error[4];
  error[4]=error[3];
  error[3]=error[2];
  error[2]=error[1];
  error[1]=error[0];
  error[0]=position_line;
  
  power_difference = (error[0] * Rachvel.kpg) + ((error[0] - error[2]) * Rachvel.kdg);
  SetSpeeds( -power_difference,  power_difference);

   delay(1);
   
   val = digitalRead(SW4_ENTER);
  }
  
   //---------------------------FIN DE PRUEBA DE CALIBRACION----------------------------------------------------//
 //stop Motors
  SetSpeeds(0,0);
  
  digitalWrite(LED2, LOW);
  digitalWrite(LED1, LOW);
   delay(200);
  digitalWrite(LED2, HIGH);     
  digitalWrite(LED1, HIGH);
  delay(200);
  digitalWrite(LED2, LOW);     
  digitalWrite(LED1, LOW);
  delay(200);                  
  digitalWrite(LED2, HIGH);     
  digitalWrite(LED1, HIGH);   
 
 //---------------------------CONTROL DE INICIO DE TURBINA Y TEST (OPCIONAL) ----------------------------------------------------// 


 //---------------------------LISTO PARA COMPETIR----------------------------------------------------// 
 //---------------------------SELECIONAR METODO DE INICO----------------------------------------------------//
  
// ESPERA QUE SE OPRIMA EL SW2

 digitalWrite(LED2, HIGH);
 digitalWrite(LED1, HIGH);

  Oled_TURBIA();
  Turbina_Init();
  Turbina_Start();
  

// INICIO CON MODULO REMOTO
   Oled_READY();
  
   Rachvel.start = 0; //STOP BLUETOOTH
   int rf_control = analogRead(ON_RF);

  digitalWrite(LED2, LOW); //ENCIENDE LEDS
  digitalWrite(LED1, LOW); 
  
   while(rf_control < 50 && Rachvel.start == 0 && stat_sw == 0)
   {
      rf_control = analogRead(ON_RF);
      Serial_command();
      val = digitalRead(SW4_ENTER); 
      if(val == LOW)
      {
          digitalWrite(LED2, HIGH);
          digitalWrite(LED1, HIGH);
          delay(1000);
          digitalWrite(LED2, HIGH);
          digitalWrite(LED1, LOW);
          delay(1000);
          digitalWrite(LED2, LOW);
          digitalWrite(LED1, HIGH);
          delay(1000);
          digitalWrite(LED2, HIGH);
          digitalWrite(LED1, HIGH);
          delay(1000);
          digitalWrite(LED2, LOW);
          digitalWrite(LED1, LOW);
          delay(999);
          stat_sw =  1;
      }
   }


  if(Rachvel.start == 0xFF) // MODULO CONECTADO PERO ARRANCA POR SW
  {
      stat_sw =  1; //NO PARA POR MODULO
  }
  
  if(rf_control > 50) // MODULO CONECTADO PERO ARRANCA POR SW
  {
    stat_sw =  0; //NO PARA POR MODULO
  }

   Rachvel.start = 0xFF; //START BLUETOOTH

  
  Oled_GO();


// Si se necesitan 5 Segundos
//delay(5000); 

//INICIO DE TURBINA
Turbina_Vel(Rachvel.pmw_t);
 
  
}
 
void loop()
{
 
//APAGADO POR MODULO REMOTO
   int rf_control = analogRead(ON_RF);
   if (rf_control < 50 && stat_sw == 0)
   {
       Turbina_Vel(0); 
       SetSpeeds(0, 0);
       Oled_STOP();
       while(1)
       {
          digitalWrite(LED2, HIGH);
          digitalWrite(LED1, HIGH);
          delay(500);
          digitalWrite(LED2, LOW);
          digitalWrite(LED1, LOW);
          delay(500);
        
        }
   } // STOP ROBOT
   
  position_line = Leer_linea(sensorValues,position_line ,Rachvel.colorlinea ); // leemos posicion de la linea en la variable position

   
 if (position_line < -20)
 {
     digitalWrite(LED2, HIGH);
     digitalWrite(LED1, LOW); 
  }
  else  if (position_line > 20 )
  {
     digitalWrite(LED2, LOW);
     digitalWrite(LED1, HIGH);    
  }


  error[9]=error[8];
  error[8]=error[7];
  error[7]=error[6];
  error[6]=error[5];
  error[5]=error[4];
  error[4]=error[3];
  error[3]=error[2];
  error[2]=error[1];
  error[1]=error[0];
  error[0]=position_line;

 detect_recta_ant = detect_recta;
 detect_recta = 1;

      for(int i=0 ; i<10; i++)
      {
        if( error[i] > 8 || error[i] < -8)
        {
          detect_recta = 0;
        }

      }

      if(detect_recta) //es RECTA
      {
         digitalWrite(LED2, HIGH);
         digitalWrite(LED1, HIGH);
      }
      else //ES CURVA
      {
         if(detect_recta_ant) //ENTRA A CURVA
         {
             digitalWrite(LED2, LOW);
             digitalWrite(LED1, LOW);
             SetSpeeds(-900, -900);
             delay(5);
         }
      }


  int vavg= Rachvel.vavg;
  vavg = vavg*10;
  power_difference = (error[0] * Rachvel.kpg) + ((error[0] - error[4]) * Rachvel.kdg);

  if( Rachvel.start )
  {
     SetSpeeds(vavg  - power_difference, vavg +  power_difference);
     Turbina_Vel(Rachvel.pmw_t);
  }
  else
  {
     SetSpeeds(0, 0);
     Turbina_Vel(0); 
  }
//SERIAL STOP
Serial_command();

   delay(1);
}
