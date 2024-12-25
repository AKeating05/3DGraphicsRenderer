// This sketch includes a function to draw circle segments
// for pie charts in 1 degree increments

#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define screenX 240
#define screenY 320

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

typedef struct point
{
    int x;
    int y;
    int z;
} screen;

screen S[screenX][screenY];

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

unsigned char z2lum(int z)
{
    unsigned char lums[13] = {'@','$','#','*','!','=',';',':','~','-',',','.',' '};
    float a = 12*sigmoid(z);
    z = round(a);
    char lum = lums[z];
    //printf("%f\n",a);
    return lum;
}

void drawPoint(float x, float y, float z)
{
    struct point newPoint;
    //newPoint.x = round(((K1*x)/(K2+z)));
    //newPoint.y = round(((K1*y)/(K2+z)));
    newPoint.x = round(x);
    newPoint.y = round(y);
    newPoint.z = round(z);
    
    struct point current = S[newPoint.x][newPoint.y];
    
    /*
    If coordinates are positive and within the bounds of the screen, add the point to the screen array
    */
    if(((0<=newPoint.x) && (newPoint.x<screenX)) && ((0<=newPoint.y) && (newPoint.y<screenY)) && (newPoint.z < current.z))
    {
      S[newPoint.x][newPoint.y] = newPoint;
      tft.drawPixel(newPoint.x,newPoint.y,0xFF0000);
    }
}

void initScreenArray()
{
    for(int i = (screenY-1); i>-1;i--)
    {
        for(int j = 0; j<screenX; j++)
        {   
            struct point newPoint;
            newPoint.x = j;
            newPoint.y = i;
            newPoint.z = 100;

            if(((0<=newPoint.x) && (newPoint.x<screenX)) && ((0<=newPoint.y) && (newPoint.y<screenY)))
            {
                S[j][i] = newPoint;
            }
        }
    }
}

void sphere(float Cx, float Cy, float Cz, float rad)
{
    float x = rad;
    float z = 0;
    float y = 0;

    //rotation around y axis
    for(float theta = 0; theta<(2*M_PI); theta+=M_PI/180)
    {
        float tX = x*cosf(theta) + z*sinf(theta);
        float tY = y;
        float tZ = z*cosf(theta) - x*sinf(theta);
        //rotation around z axis
        for(float phi = 0; phi<(2*M_PI); phi+=M_PI/180)
        {
            //code for circle but in 3d
            float pX = tX*cosf(phi) - tY*sinf(phi);
            float pY = tX*sinf(phi) + tY*cosf(phi);
            float pZ = tZ;

            //z2lum(round(pZ+Cz))
            drawPoint(pX+Cx,pY+Cy,pZ+Cz);
        }
        //printScreen();
    }
}

void loop() 
{
  //tft.drawCircle(100, 100, 10, 0xFF0000);
  sphere(100,100,20,50);
}





