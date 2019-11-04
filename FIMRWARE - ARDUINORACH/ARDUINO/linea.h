
#define TIMEOUT                 40

//Sensores de Linea PD
#define NUM_SENSORS             8  // Numero de sensores que usa

#define COLOR_LINE true  //true linea blanca
//#define COLOR_LINE false  //false linea negra


void Leer_sensores (unsigned int* sensorValuesp);
void Reset_Calibracion(void);
int Calibrar_Color_Linea(unsigned int* sensorValuesp);
void Calibrar_Sensores(unsigned int* sensorValuesp, int colorlinea);
int Leer_linea(unsigned int* sensorValuesp, int linea_anterior, int colorlinea);
