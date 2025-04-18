#include "trig.h"
#include <math.h>


//creates sin table statically in memory and returns a pointer to it
float *makeSinTable(int interval)
{
    float *table = (float*) malloc(sizeof(float)*interval);

    for(int i=0; i<interval; i++)
    {
        table[i] = sinf((M_PI*i)/interval);
    }

    return table;
}

//compute sine using lookup table, adjust angle so it is in range 0-pi
float sine(float theta, int interval, float *table)
{
    while (theta < 0) theta += 2 * M_PI;
    while (theta >= 2 * M_PI) theta -= 2 * M_PI;

    float step = M_PI / interval;

    if (theta <= M_PI) {
        int index = floor(theta / step);
        if (index >= interval) index = interval - 1;
        return table[index];

    } else {
        // sin(θ) = -sin(θ - π)
        theta -= M_PI;
        int index = floor(theta / step);
        if (index >= interval) index = interval - 1;
        return -table[index];
    }
}

float cosine(float theta, int interval, float *table)
{
    return sine((theta+(M_PI/2)),interval,table);
}
