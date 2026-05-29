#ifndef PERLIN_H
#define PERLIN_H


float perlin2D(float x, float z, float frequency, int seed);

float fractalPerlin2D(float x, float z, float frequency, float octave, float persistence, int seed);

#endif