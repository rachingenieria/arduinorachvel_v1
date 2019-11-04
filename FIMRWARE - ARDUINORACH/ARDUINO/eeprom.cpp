#include <arduino.h>
#include <EEPROM.h>

#include "rachvel.h"
extern rachvel Rachvel;

int address = 0;

void Eeprom_read(void) {

  int address_offset = address;
  // read a byte from the current address of the EEPROM
  Rachvel.ver = EEPROM.read(address_offset);
  address_offset = address_offset +4;
  Rachvel.vavg = EEPROM.read(address_offset);
  address_offset = address_offset +4;
  Rachvel.kpg = (EEPROM.read(address_offset));
  address_offset = address_offset +4;
  Rachvel.kdg = (EEPROM.read(address_offset));
  address_offset = address_offset +4;
  Rachvel.pmw_t = EEPROM.read(address_offset);

}


void Eeprom_save(void) 
{
  // read a byte from the current address of the EEPROM
  int address_offset = address;
  // read a byte from the current address of the EEPROM
  EEPROM.write(address_offset,Rachvel.ver);
  address_offset = address_offset +4;
  EEPROM.write(address_offset,Rachvel.vavg);
  address_offset = address_offset +4;
  EEPROM.write(address_offset,Rachvel.kpg);
  address_offset = address_offset +4;
  EEPROM.write(address_offset,Rachvel.kdg);
  address_offset = address_offset +4;
  EEPROM.write(address_offset,Rachvel.pmw_t);
}
