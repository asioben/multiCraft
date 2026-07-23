#ifndef MODEL_H
#define MODEL_H

#include <cglm/struct.h>

#include "block.h"

void generateTree(vec3s *positions, BlockID *blocks);

void checkTreeValidPosition(vec3s position, vec3s start, float *output);
#endif