#include <arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

int oled_init()
{
 
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    return -1;
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(500); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
}

void Oled_Clear()
{
   display.clearDisplay();
}

void Oled_Calibracion(void) {
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(5,5);             // Start at top-left corner
  display.println(F("Press S4 to Continue"));

  display.setCursor(5,15);             // Start at top-left corner
  display.println(F("Test Calibration"));

  display.display();
  delay(2000);
}


void Oled_Sensores(unsigned int* sensorValuesp,int position_line,  int colorlinea)
{
  int line = (int)position_line * 10;
  display.clearDisplay();
  display.setTextSize(1);

  display.setCursor(0,45);
  display.print(sensorValuesp[0]);
  display.setCursor(112,45);
  display.print(sensorValuesp[7]);
  
  display.setCursor(10,55);
  for(int x=1; x<7; x++)
       {
          display.print(sensorValuesp[x]);
          display.print(F(" "));
       }


   display.setCursor(20,0);
   if(colorlinea)
   {
        display.println(F("LINE BLACK"));
   }
   else
   {
      display.println(F("LINE WHITE"));
   }
       
    display.setTextSize(2);
    display.setCursor(60,20);
    display.println(position_line);

  display.display();
  //delay(100);
}

void Oled_TURBIA()
{
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(60,10);
    display.println(F("WAIT"));
    display.println(F("EDF"));
    display.display();
    delay(100);
}

void Oled_SAVED()
{
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(60,10);
    display.println(F("SAVE"));
    display.display();
    delay(100);
}


void Oled_READY()
{
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(60,10);
    display.println(F("READY"));
    display.display();
    delay(100);
}

void Oled_GO()
{
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(60,10);
    display.println(F("GO GO"));
    display.display();
    delay(100);
}

void Oled_STOP()
{
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(60,10);
    display.println(F("STOP"));
    display.display();
    delay(100);
}

void Oled_Turbina(int position)
{
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(60,10);
    display.println(position);
    display.display();
    delay(100);
}



void Oled_Api(int v, int p, int d, int t, int cursorz, int act, float Battery, int versionfirmware)
{
  display.clearDisplay();
              
  display.setTextColor(WHITE);        // Draw white text

    //display.setTextSize(1); // Normal 1:1 pixel scale
    //display.setCursor(50,20);
    //display.print(Battery);

     display.setTextSize(1);
    display.setCursor(10,5);
    display.print(F("CRONOX"));
    display.print(versionfirmware);   
    display.setCursor(10,15);
    display.print(F("EXIT  "));
    display.print(F(" BAT:"));
    display.print(Battery);
    display.setCursor(10,25);
    display.print(F("SPEED:"));
    display.print(v);
    display.setCursor(10,35);
     display.print(F("CTE P:"));
    display.println(p);
    display.setCursor(10,45);
    display.print(F("CTE D:"));
    display.println(d);
    display.setCursor(10,55);
    display.print(F("EDF:"));
    display.println(t);
    if(act == 0)
    {
      display.setCursor(0,15+cursorz*10);
      display.print(F("_"));
    }
    else
    {
      display.setCursor(0,15+cursorz*10);
      display.print(F("*"));
    }
    display.display();
    delay(100);
}
