#include <Adafruit_ILI9341.h>
#include "ILI9341_T4.h"


#include <Wire.h>
#include "MLX90640_API.h"
#include "MLX90640_I2C_Driver.h"

#define BLACK 0x0000
#define NAVY 0x000F
#define DARKGREEN 0x03E0
#define DARKCYAN 0x03EF
#define MAROON 0x7800
#define PURPLE 0x780F
#define OLIVE 0x7BE0
#define LIGHTGREY 0xC618
#define DARKGREY 0x7BEF
#define BLUE 0x001F
#define GREEN 0x07E0
#define CYAN 0x07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define ORANGE 0xFD20
#define GREENYELLOW 0xAFE5
#define PINK 0xF81F

#define CS_PIN 8
#define DC_PIN 10
#define MOSI_PIN 11
#define MISO_PIN 12
#define SCK_PIN 13
#define RST_PIN 9



const byte MLX90640_address = 0x33; //Default 7-bit unshifted address of the MLX90640

//ILI9341_T4::ILI9341Driver tft(CS_PIN, DC_PIN, SCK_PIN, MOSI_PIN, MISO_PIN, RST_PIN); 
Adafruit_ILI9341 tft = Adafruit_ILI9341(CS_PIN, DC_PIN, RST_PIN);
uint16_t fb[240*320];
DMAMEM uint16_t fb_internal1[240*320];
#define TA_SHIFT 8 //Default shift for MLX90640 in open air

float mlx90640To[768];
paramsMLX90640 mlx90640;

const byte calcStart = 33; //Pin that goes high/low when calculations are complete
//This makes the timing visible on the logic analyzer

void setup()
{
  pinMode(calcStart, OUTPUT);

  Wire.begin();
  Wire.setClock(800000); //Increase I2C clock speed to 400kHz

  Serial.begin(115200); //Fast serial as possible

  while (!Serial); //Wait for user to open terminal
  //Serial.println("MLX90640 IR Array Example");

  if (isConnected() == false)
  {
    Serial.println("MLX90640 not detected at default I2C address. Please check wiring. Freezing.");
    while (1);
  }

//  if (!tft.begin())
//  {
//    Serial.print("Ouch!");
//  }
  tft.begin();
  tft.setRotation(0);
//  tft.setFramebuffers(fb_internal1);
//  tft.setRefreshRate(120); // set the display refresh rate around 120Hz
//  tft.setVSyncSpacing(2);

  
  //Get device parameters - We only have to do this once
  int status;
  uint16_t eeMLX90640[832];
  status = MLX90640_DumpEE(MLX90640_address, eeMLX90640);
  if (status != 0)
    Serial.println("Failed to load system parameters");

  status = MLX90640_ExtractParameters(eeMLX90640, &mlx90640);
  if (status != 0)
    Serial.println("Parameter extraction failed");

  //Once params are extracted, we can release eeMLX90640 array

  //Set refresh rate
  //A rate of 0.5Hz takes 4Sec per reading because we have to read two frames to get complete picture
  //MLX90640_SetRefreshRate(MLX90640_address, 0x00); //Set rate to 0.25Hz effective - Works
  //MLX90640_SetRefreshRate(MLX90640_address, 0x01); //Set rate to 0.5Hz effective - Works
  //MLX90640_SetRefreshRate(MLX90640_address, 0x02); //Set rate to 1Hz effective - Works
  //MLX90640_SetRefreshRate(MLX90640_address, 0x03); //Set rate to 2Hz effective - Works
  //MLX90640_SetRefreshRate(MLX90640_address, 0x04); //Set rate to 4Hz effective - Works
  //MLX90640_SetRefreshRate(MLX90640_address, 0x05); //Set rate to 8Hz effective - Works at 800kHz
  MLX90640_SetRefreshRate(MLX90640_address, 0x06); //Set rate to 16Hz effective - Works at 800kHz
  //MLX90640_SetRefreshRate(MLX90640_address, 0x07); //Set rate to 32Hz effective - fails

  //Once EEPROM has been read at 400kHz we can increase to 1MHz
  Wire.setClock(1000000); //Teensy will now run I2C at 800kHz (because of clock division)
}

void loop()
{
  long startTime = millis();
  for (byte x = 0 ; x < 2 ; x++)
  {
    uint16_t mlx90640Frame[834];
    int status = MLX90640_GetFrameData(MLX90640_address, mlx90640Frame);

    digitalWrite(calcStart, HIGH);
    float vdd = MLX90640_GetVdd(mlx90640Frame, &mlx90640);
    float Ta = MLX90640_GetTa(mlx90640Frame, &mlx90640);

    float tr = Ta - TA_SHIFT; //Reflected temperature based on the sensor ambient temperature
    float emissivity = 0.95;

    MLX90640_CalculateTo(mlx90640Frame, &mlx90640, emissivity, tr, mlx90640To);
    digitalWrite(calcStart, LOW);
    //Calculation time on a Teensy 3.5 is 71ms
  }
  long stopReadTime = millis();
  
  for (int x = 0 ; x < 768 ; x++)
  {
    //if(x % 8 == 0) Serial.println();
//    Serial.print(mlx90640To[x], 2);
//    Serial.print(",");
//    int a = x / 24;
//    int b = x % 24;
//    for (int i = 0; i < 10; i++ )
//    {
//      fb[10*x+i] = mlx90640To[x];
//    }
    uint16_t pixel_color;
    int pixel_temp = (int)mlx90640To[x];
//    Serial.println("here");
    
    switch (pixel_temp)
    {
        case  0 ... 9:
            pixel_color = BLACK;
            break;
        case 10 ... 14:
            pixel_color = NAVY;
            break;
        case 15 ... 19:
            pixel_color = BLUE;
            break;
        case 20 ... 24:
            pixel_color = DARKCYAN;
            break;
        case 25 ... 29:
            pixel_color = CYAN;
            break;
        case 30 ... 34:
            pixel_color = PURPLE;
            break;
        case 35 ... 39:
            pixel_color = ORANGE;
            break;
        case 40 ... 45:
            pixel_color = YELLOW;
            break;
        
     
    }
    
   for (int i = 0; i < 7; i++)
   {
     for (int j = 0; j < 7; j++) 
     {
      tft.drawPixel((x%32)*7+i, (x/32)*7+j, pixel_color);
     }
   }
  }
//  tft.update(fb);
  Serial.println("");
  long stopPrintTime = millis();

  Serial.print("Read rate: ");
  Serial.print( 1000.0 / (stopReadTime - startTime), 2);
  Serial.println(" Hz");
  Serial.print("Read plus print rate: ");
  Serial.print( 1000.0 / (stopPrintTime - startTime), 2);
  Serial.println(" Hz");
}

//Returns true if the MLX90640 is detected on the I2C bus
boolean isConnected()
{
  Wire.beginTransmission((uint8_t)MLX90640_address);
  if (Wire.endTransmission() != 0)
    return (false); //Sensor did not ACK
  return (true);
}
