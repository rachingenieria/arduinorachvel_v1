#define TURBINA   6 //PORT

#define TURBINA_DC_INIT 1000 //TIEMPO EN ms para PUNTO DE INICION DEL ESC

void Turbina_Init(void);
void Turbina_Start(void);
void Turbina_Vel(int vel);
