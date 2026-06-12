#include "../include/perlin.h"

#define STB_PERLIN_IMPLEMENTATION
#include "../third_party/stb_perlin.h"

float perlin2D(float x, float z, float frequency, int seed){
    float seed_ = (float)(seed/1000);
    float x_ = x * frequency + seed_;
    float z_ = z * frequency + seed_;
    return stb_perlin_noise3(x_,0,z_,0.0f,0.0f,0.0f);
}

float fractalPerlin2D(float x, float z, float frequency, int octave, float persistence, int seed){
    float total = 0.0f;
    float amplitude = 1.0f;
    float max = 0.0f;

    for(int i = 0; i < octave; i++){
        total += perlin2D(x,z,frequency,seed) * amplitude;
        max += amplitude;
        amplitude *= persistence;
        frequency *= 2.0f;
    }
    return total/max;
}
