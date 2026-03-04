#include "../include/utility.h"

int safe_exit(const char *message, SDL_Window *window, SDL_GLContext context){
    printf("%s: %s\n",message,SDL_GetError());
    if(context != NULL) SDL_GL_DestroyContext(context);
    if(window != NULL) SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

int readFile(const char *path,char **content){
    FILE * fptr = NULL;
    int size = 0;

    //open file
    fptr = fopen(path, "r");
    //check for failure
    if(fptr == NULL) {
        printf("%s failed to open\n",path);
        //it should be a good idea to check what the function
        //returns before freeing it 
        fclose(fptr);
        return 1;
    }
    //go at the end of the file
    fseek(fptr,0,SEEK_END);
    //get the position of the end of the file to know its size
    size = ftell(fptr);
    //check if the file is empty
    if(size == 0) {
        printf("%s is empty\n",path);
        fclose(fptr);
        return 1;
    }
    //come back at the begining
    fseek(fptr,0,SEEK_SET);
    //create a buffer of memory
    *content = malloc(size + 1);
    if(*content == NULL){
        printf("empty content\n");
        fclose(fptr);
        return 1;
    }
    //read the file
    fread(*content,1,size,fptr);
    (*content)[size] = '\0';
    //close the file
    fclose(fptr);

    return 0;
    //be careful while freeing
}

int64_t getTime(void){
    struct timespec tms;

    if(!timespec_get(&tms,TIME_UTC)) return -1;

    int64_t micros = (tms.tv_sec * 1000000ULL) + (tms.tv_nsec/1000);

    return micros;
}

void initTime(Tick *tick){
    tick->before = getTime();
}

void deltaTime(Tick *tick){
    tick->after = getTime();
    tick->delta = tick->after - tick->before;
    tick->before = tick->after;
}