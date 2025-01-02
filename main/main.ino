#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include <utility>
using namespace std;

#define screenX 240
#define screenY 320

#define K1 120
#define K2 60

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

vector<pair<int,int>> projectedXYs;
vector<float> zBuffer;

void setup(void)
{
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
}

float sigmoid(float val)
{
    float sig = 1/(1+(pow(M_E,(-val))));
    return sig;
}

/*
unsigned char z2lum(int z)
{
    unsigned char lums[13] = {'@','$','#','*','!','=',';',':','~','-',',','.',' '};
    float a = 12*sigmoid(z);
    z = round(a);
    char lum = lums[z];
    //printf("%f\n",a);
    return lum;
}

*/
void drawPoint(float x, float y, float z)
{
  int projX = round((K1*x)/(K2 + z));
  int projY = round((K1*y)/(K2 + z));
  pair<int,int> xy;
  xy.first = projX;
  xy.second = projY;
  
  auto it = find(projectedXYs.begin(), projectedXYs.end(), xy);
  int pos = distance(projectedXYs.begin(), it);

  if(pos<projectedXYs.size())
  {
    float currentZ = zBuffer[pos];
    if(currentZ>z)
    {
      zBuffer.at(pos) = z;
      projectedXYs.at(pos) = xy;
      tft.drawPixel(projX,projY,0xFF0000);
    }
  }
  else
  {
    projectedXYs.push_back(xy);
    zBuffer.push_back(z);
    tft.drawPixel(projX,projY,0xFF0000);
  }
}

void sphere(float Cx, float Cy, float Cz, float rad)
{
  for(float phi = 0; phi<M_PI; phi+= M_PI/180)
  {
    for(float theta = 0; theta<2*M_PI; theta+= M_PI/180)
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
  //tft.drawCircle(100, 100, 10, 0xFF0000);
  sphere(100,100,10,10);
}





