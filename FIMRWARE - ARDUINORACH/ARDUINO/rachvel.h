#ifndef RACHVEL_h
#define RACHVEL_h
#include "Arduino.h"

//--------------------------------------------------------------------------------------//
#define FIRMWARE_VERSION     23

// LISTADO DE PINES Y CONECCIONES
#define LED1           7
#define LED2           4

#define SW2_UP         7
#define SW4_ENTER      8
#define SW3_DOWN       4

#define ON_RF          A7
#define BATTERY        A6

//ESTRUCTURA PARA SALVAR DATOS
 
class rachvel
{
  public:
    rachvel (void);
    void setupconfig(int vavg,int kpg, int  kdg, int pmw_t);

        int    ver;
        int    vavg;
        int    kpg;
        int    kdg;
        int    pmw_t;   //VEL VARIA DE 0 a 9
       
        int start;
        int colorlinea; //LINEA "0"= Blanco ,"1" = NEGRO
        int remoto_enable;
        int sw_enable;
        
  private:
        
        //variables de control
};
 
#endif
