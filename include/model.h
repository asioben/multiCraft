#ifndef MODEL_H
#define MODEL_H

#include <cglm/struct.h>

#include "block.h"

#define TREE_SIZE 45

void generateTree(vec3s *positions, BlockID *blocks);

void checkTreeValidPosition(vec3s position, float *output);

#endif