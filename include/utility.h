#ifndef UTILITY_H
#define UTILITY_H

#define SDL_MAIN_HANDLED
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_opengl.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <inttypes.h>

#define WIDTH 1000
#define HEIGHT 750

#define swap_(x, y, tmp) tmp = x; x = y; y = tmp;

typedef struct{
    Uint64 before;
    Uint64 after;
    Uint64 delta;
}Tick;

int64_t getTime(void);

//Tick
void initTime(Tick *tick);

void deltaTime(Tick *tick);

void fps_counter(int *fps, int *frames, Tick *timer);

bool timeCounter(Tick *tick, Uint64 time);


//GENERAL

int safe_exit(const char *message, SDL_Window *window, SDL_GLContext context);

//BE CAREFUL AND FREE IT IN THE RIGHT MOMENT!!!
int readFile(const char *path, char **content);


//STRING

void number_to_string(int number, char **string);

void concatenate_string(const char *a, const char *b, char **result);

int string_len(const char *string);

/*
//VECTOR ???
typedef struct{
    size_t size;
    size_t capacity;
}Vector;*/

//return a 0
int safe_return(const char *msg);

//random function
int random_(int min, int max);

#endif