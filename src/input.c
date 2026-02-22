#include "../include/input.h"

const Uint8 *getKeys(void){
    //get all the key state of the keyboard
    return SDL_GetKeyboardState(NULL);
}

Mouse getMouse(SDL_Event event){
    Mouse mouse_info;
    //get some mouse state such as his clicked buttons and his current postion
    Uint32 buttons = SDL_GetMouseState(NULL,NULL);
    //check right or left click
    mouse_info.left = buttons & SDL_BUTTON_LMASK;
    mouse_info.right = buttons & SDL_BUTTON_RMASK;
    //get his relative motion
    mouse_info.motion.x = event.motion.xrel;
    mouse_info.motion.y = event.motion.yrel;
    //get its position
    mouse_info.position.x = event.motion.x;
    mouse_info.position.y = event.motion.y;

    return mouse_info;
}