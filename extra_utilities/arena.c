//LIB
#include <stdlib.h>
#include <stdio.h>

// INCLUDE
#include "arena.h"


int arena_init(Arena *arena, unsigned long capacity){
    if(capacity >= CAPACITY_LIMIT){
        printf("Memory limit\n");
        return 0;
    }
    arena->buffer = (char*)malloc(sizeof(char) * capacity);
    arena->capacity = capacity;
    arena->counter = 0;

    return 1;
}

void *arena_alloc(Arena *arena, unsigned long size){
     unsigned long offset = (arena->counter + (ALIGNEMENT -1)) & ~(ALIGNEMENT -1);

     if(offset + size > arena->capacity){
        printf("Memory exceed\n");
        return NULL;
     }

     void *ptr = &arena->buffer[offset];
     arena->counter = offset + size;

     return ptr;
}

void arena_reset(Arena *arena){
    arena->counter = 0;
}

void arena_free(Arena *arena){
    free(arena->buffer);
    arena->buffer = NULL;
    arena->capacity = 0;
    arena->counter = 0;
}