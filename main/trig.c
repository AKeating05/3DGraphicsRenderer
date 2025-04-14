#include "trig.h"
#include <math.h>


//creates sin table statically in memory and returns a pointer to it
float *makeSinTable(int interval)
{
    float *table = (float) malloc(sizeof(float)*interval);

    for(int i=0, i<interval; i++)
    {
        table[i] = sinf((M_PI*i)/interval);
    }

    return table;
}

//compute sine using lookup table, adjust angle so it is in range 0-pi
float sine(float theta, int interval, float *table)
{
    if(theta>M_PI)
    {
        theta = theta - M_PI;
        int index = floor(theta/(M_PI/interval));
        return -table[index];
    }
    else
    {
        int index = floor(theta/(M_PI/interval));
        return table[index];
    }
}

float cosine(float theta, int interval, float *table)
{
    return sine((theta+(M_PI/2)),interval,table);
}
