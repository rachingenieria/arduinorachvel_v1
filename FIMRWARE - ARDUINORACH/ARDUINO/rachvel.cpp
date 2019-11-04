#include "Arduino.h"
#include "rachvel.h"

extern rachvel Rachvel;

rachvel::rachvel (void)
{
  
}//constructor


void  rachvel::setupconfig(int vavgx,int kpgx, int  kdgx, int pmw_tx)
{   
    ver = FIRMWARE_VERSION;
    vavg = vavgx;
    kpg = kpgx;
    kdg = kdgx;
    pmw_t = pmw_tx;   //VEL VARIA DE 0 a 9
    Rachvel.start = 0xFF;
  
}
