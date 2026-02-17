#include <stdio.h>
#include "../include/utility.h"

#define WIDTH 800
#define HEIGHT 600

int main(){
    //printf("Hello World !\n");

    if(SDL_Init(SDL_INIT_VIDEO) < 0)return(safe_exit("Erreur SDL",NULL,NULL));

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);

    SDL_Window *window = SDL_CreateWindow("Hello World",WIDTH,HEIGHT,SDL_WINDOW_OPENGL|0);
    bool loop = true;
 
    if(!window)return(safe_exit("Erreur Window",window,NULL));
    
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if(!context)return(safe_exit("Erreur Context",window,context));

    if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))return(safe_exit("Erreur OpenGL loading",NULL,NULL));
    
    const GLubyte *version = glGetString(GL_VERSION);
    if(!version)return(safe_exit("Erreur Version",window,context));

    printf("OpenGL version: %s\n",version);

    //SDL_GL_SetSwapInterval(-1);

    glClearColor(1.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    while(loop){
        SDL_GL_SwapWindow(window);
        SDL_Event event;
        while(SDL_PollEvent(&event) == 1){
            switch(event.type){
                case SDL_EVENT_QUIT: loop = false; break;
                case SDL_EVENT_KEY_DOWN:
                    switch(event.key.key){
                        case SDLK_ESCAPE: return safe_exit("Test",window,context);break;
                    }break;
            }
        }
    }
    
    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}