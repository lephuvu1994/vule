#include "matrix.h"
#include <stdlib.h>




float kalman_single(float z, float measure_noise, float process_noise);
void kalman( float *in, float *out, float measure_noise, float process_noise);
float HPF(float x, float cutoff, float sample_rate);
float LPF(float x, float cutoff, float sample_rate);
