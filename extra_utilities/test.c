#include "arena.h"
#include <string.h>
#include <stdio.h>

int main(){
    Arena arena;
    if(arena_init(&arena,1024) == 0) return 0;
    
    int *p = (int *) arena_alloc(&arena,100);
    if(p == NULL) return 0;

    memset(p,0,100);
    p[12] = 123;
    for(int i = 0; i < 1; i++){
        printf("%i\n",p[12]);
    }

    arena_reset(&arena);
    arena_free(&arena);
    p = NULL;

}