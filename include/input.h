#ifndef INPUT_H
#define INPUT_H

#include "utility.h"

typedef struct mouse_{
    SDL_Point position;
    SDL_Point motion;
    int left;
    int right;
}Mouse;

const Uint8 *getKeys(void);

Mouse getMouse(SDL_Event event);

#endif