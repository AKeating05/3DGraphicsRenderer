#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include <utility>
using namespace std;

#define screenX 240
#define screenY 320

#define K1X 150
#define K1Y 200
#define K2 75

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

uint8_t zBuffer[screenX*screenY];

int X = 0;
int Y = 0;

void setup(void)
{
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  for(int i=0; i<screenX*screenY; i++)
  {
      zBuffer[i] = 255;
  }
}

float sigmoid(float val)
{
    float sig = 1/(1+(pow(M_E,(-val))));
    return sig;
}

void clearZBuffer()
{
  for(int i = 0; i < screenX * screenY; i++)
    zBuffer[i] = 255;
}

void drawPoint(float x, float y, float z)
{
  int projX = round((K1X*x)/(K2 + z));
  int projY = round((K1Y*y)/(K2 + z));
  
  if(projX >=0 && projX < screenX && projY >=0 && projY < screenY)
  {
    int index = projY*screenX + projX;
    uint8_t zNorm = (uint8_t)(255 - constrain(z, 0, 255));
    if(zNorm<zBuffer[index])
      {
        zBuffer[index] = zNorm;
        tft.drawPixel(projX,projY,tft.color565(0, 0, 255));
      }
  }
  
  
}

void sphere(int Cx, int Cy, int Cz, int rad)
{
  for(float phi = 0; phi<M_PI; phi+= M_PI/90)
  {
    for(float theta = 0; theta<2*M_PI; theta+= M_PI/90)
    {
      float x = rad*sinf(phi) * cosf(theta);
      float y = rad*sinf(phi) * sinf(theta);
      float z = rad*cosf(phi);

      drawPoint(x+Cx,y+Cy,z+Cz);
    }
  }
}

void loop() 
{
  //tft.drawCircle(100, 100, 10, tft.color565(0, 0, 255));
  sphere(120,Y,20,30);
  delay(30);
  tft.fillScreen(TFT_BLACK);
  clearZBuffer();
  Y++;
}





