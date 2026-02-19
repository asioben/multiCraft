#ifndef UTILITY_H
#define UTILITY_H

#define SDL_MAIN_HANDLED
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_opengl.h>

#include <stdlib.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600

int safe_exit(const char *message, SDL_Window *window, SDL_GLContext context);

//BE CAREFUL AND FREE IT IN THE RIGHT MOMENT!!!
int readFile(const char *path, char **content);

#endif