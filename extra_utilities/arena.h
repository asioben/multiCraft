#ifndef ARENA_H
#define ARENA_H

#define CAPACITY_LIMIT 8589934592
#define ALIGNEMENT 8

typedef struct{
    char *buffer;
    unsigned long capacity;
    unsigned long counter;
} Arena;

int arena_init(Arena *arena, unsigned long capacity);

void *arena_alloc(Arena *arena, unsigned long size);

void arena_reset(Arena *arena);

void arena_free(Arena *arena);

#endif