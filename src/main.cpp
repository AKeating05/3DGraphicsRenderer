#include <Arduino.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "trig.h"

#include <utility>
using namespace std;

#define screenX 240
#define screenY 320

#define K1X 200
#define K1Y 200
#define K2 75

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

uint8_t zBuffer[screenX*screenY];
float *sineLkupTable;


int X = 0;
int Y = 0;
int Z = 0;


void setup(void)
{
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(tft.color565(150,150,150));

  memset(zBuffer,255,sizeof(zBuffer));
  sineLkupTable = makeSinTable(180);
}


void clearZBuffer()
{ 
  memset(zBuffer,255,sizeof(zBuffer));
}

void drawPoint(float x, float y, float z, int *color)
{
  int projX = round((K1X*x)/(K2 + z)) + (screenX/2);
  int projY = round((K1Y*y)/(K2 + z)) + (screenY/2);
  
  if(projX >=0 && projX < screenX && projY >=0 && projY < screenY)
  {
    int index = projY*screenX + projX;
    uint8_t zNorm = (uint8_t)(255 - constrain(z, 0, 255));
    if(zNorm<zBuffer[index])
      {
        zBuffer[index] = zNorm;
        tft.drawPixel(projX,projY,tft.color565(color[2],color[1],color[0]));
      }
  }
  
  
}

//Given a vector light direction, surface normal vector and a colour, computed the dot product of the vectors and scales the colour to shade the pixel
int* shadePoint(int *color,float dp)
  //float lightDirVector[3], float normalVector[3], int color[3])
{
  //float dp = (lightDirVector[0]*normalVector[0]) + (lightDirVector[1]*normalVector[1]) + (lightDirVector[2]*normalVector[2]);
  static int newCol[3];
  newCol[0] = round(constrain((dp*255)+(dp*color[0]), 0, 255));
  newCol[1] = round(constrain((dp*255)+(dp*color[1]), 0, 255));
  newCol[2] = round(constrain((dp*255)+(dp*color[2]), 0, 255));

  return newCol;
}

void sphere(int Cx, int Cy, int Cz, int rad, float* lightDir, int * color)
{
  for(float phi = 0; phi<M_PI; phi+= M_PI/180)
  {
    for(float theta = 0; theta<2*M_PI; theta+= M_PI/180)
    {
      float x = rad*sine(phi,180,sineLkupTable) * cosine(theta,180,sineLkupTable);
      float y = rad*sine(phi,180,sineLkupTable) * sine(theta,180,sineLkupTable);
      float z = rad*cosine(phi,180,sineLkupTable);

      //surface normal computation (fixed lighting direction)
      float normX = x / rad;
      float normY = y / rad;
      float normZ = z / rad;

      float dp = normX * lightDir[0] + normY * lightDir[1] + normZ * lightDir[2];
      dp = max(0.0f, dp);

      drawPoint(x+Cx,y+Cy,z+Cz,shadePoint(color,dp));
    }
  }
}

void loop() 
{
  //tft.drawCircle(100, 100, 10, tft.color565(0, 0, 255));
  int color[3] = {255,0,0};
  for(float theta = 0; theta<2*M_PI; theta+= M_PI/180)
  {
    float lightDir[3] = {cosine(theta,180,sineLkupTable), 0, sine(theta,180,sineLkupTable)}; 
    //lightDir[0] = sinf(theta);
    sphere(X,Y,Z,20,lightDir, color);
    
    //tft.fillScreen(TFT_BLACK);
    clearZBuffer();
    //delay(30);
  }
  
  
  
}






