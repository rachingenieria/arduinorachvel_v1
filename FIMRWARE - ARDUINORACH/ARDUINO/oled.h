void oled_init( );
void Oled_Clear();
void Oled_Calibracion(void) ;
void Oled_Sensores(unsigned int* sensorValuesp,int position_line,  int colorlinea);

void Oled_SAVED();
void Oled_TURBIA();
void Oled_READY();
void Oled_GO();
void Oled_STOP();
void Oled_Turbina(int position);
void Oled_Api(int v, int p, int d, int t, int cursorz, int act, float Battery, int versionfirmware);
