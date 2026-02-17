#include "../include/utility.h"

int safe_exit(const char *message, SDL_Window *window, SDL_GLContext context){
    printf("%s: %s\n",message,SDL_GetError());
    if(context != NULL) SDL_GL_DestroyContext(context);
    if(window != NULL) SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}