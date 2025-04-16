#ifndef TRIG_H_
#define TRIG_H_

float *makeSinTable(int interval);
float sine(float theta, int interval, float *table);
float cosine(float theta, int interval, float *table);

#endif