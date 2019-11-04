#include <arduino.h>
#include "rachvel.h"
#include "eeprom.h"

extern rachvel Rachvel;

int API_CONFIG(char* uart_buffer_rx)
{
int respuesta = 0;
          if( (uart_buffer_rx[0] == 'V' || uart_buffer_rx[0] == 'D' || uart_buffer_rx[0] == 'P' || uart_buffer_rx[0] == 'I'|| uart_buffer_rx[0] == 'T' || uart_buffer_rx[0] == 'S') 
              && (uart_buffer_rx[1] >= '0'  && uart_buffer_rx[1] <= '9')
              && (uart_buffer_rx[2] >= '0'  && uart_buffer_rx[2] <= '9'))
          {
                  if(uart_buffer_rx[0] == 'V')
                  {
                     Rachvel.vavg = (uart_buffer_rx[1] - '0' )*10 + (uart_buffer_rx[2]- '0' );
                  }
                  else if(uart_buffer_rx[0] == 'P')
                  {
                     Rachvel.kpg = (float)((uart_buffer_rx[1] - '0' )*10 + (uart_buffer_rx[2]- '0' ));
                  }
                  else if(uart_buffer_rx[0] == 'D')
                  {
                     Rachvel.kdg = (float)((uart_buffer_rx[1] - '0' )*10 + (uart_buffer_rx[2]- '0' ));
                  }
                  else if(uart_buffer_rx[0] == 'T')
                  {
                     Rachvel.pmw_t = ( uart_buffer_rx[1] - '0' )*10 + (uart_buffer_rx[2] - '0' );
                  }
                  else if(uart_buffer_rx[0] == 'S')
                  {
                     if((uart_buffer_rx[1] == '1'  && uart_buffer_rx[2] == '5'))
                     {
                         Eeprom_save();
                     }

                     if((uart_buffer_rx[1] == '2'  && uart_buffer_rx[2] == '0'))
                     {
                         Rachvel.start = 0XFF;
                     }

                     if((uart_buffer_rx[1] == '2'  && uart_buffer_rx[2] == '5'))
                     {
                         Rachvel.start = 0;
                     }

                     if((uart_buffer_rx[1] == '3'  && uart_buffer_rx[2] == '0'))
                     {
                         //Rachvel.remoto_enable = 1;
                     }

                     if((uart_buffer_rx[1] == '3'  && uart_buffer_rx[2] == '5'))
                     {
                         //Rachvel.remoto_enable = 0;
                     }

                     if((uart_buffer_rx[1] == '4'  && uart_buffer_rx[2] == '0'))
                     {
                         //Rachvel.sw_enable = 1;
                     }

                     if((uart_buffer_rx[1] == '4'  && uart_buffer_rx[2] == '5'))
                     {
                         //Rachvel.sw_enable = 0;
                     }

                     //PISO
                      if((uart_buffer_rx[1] == '5'  && uart_buffer_rx[2] == '0'))
                     {
                         Rachvel.colorlinea = 0;
                     }

                     if((uart_buffer_rx[1] == '5'  && uart_buffer_rx[2] == '5'))
                     {
                         Rachvel.colorlinea = 1;
                     }
                  }
                  respuesta = 1;
          }
          else
          {
            uart_buffer_rx[0] = 0;
            uart_buffer_rx[1] = 0;
            uart_buffer_rx[2] = 0;
          }

return respuesta;
}


int API_BUFFER(char uart_buffer_rx)
{
int respuesta = 0;
          if( uart_buffer_rx== 'V' || uart_buffer_rx == 'D' || uart_buffer_rx == 'P' || uart_buffer_rx == 'I'|| uart_buffer_rx == 'T' || uart_buffer_rx == 'S') 
          {
            respuesta = 1;
          }
  return respuesta;
}


int count;
char uart_buffer_tx[32];
char uart_buffer_rx[32];

void Serial_command(void)
{
while (Serial.available()) {
      char datau =Serial.read();
      if( API_BUFFER(datau))
       {
         count = 0;
       }
      uart_buffer_rx[count] = datau;
      count ++;
      if( count == 3)
      {
        API_CONFIG(uart_buffer_rx);
        Serial.print('V');
        Serial.print(Rachvel.vavg);
        Serial.print(" -P");
        Serial.print(Rachvel.kpg);
        Serial.print(" -D");
        Serial.print(Rachvel.kdg);
        Serial.print(" -T");
        Serial.println(Rachvel.pmw_t);
      }
      if(count > 3)
        count = 0;
    }
}
