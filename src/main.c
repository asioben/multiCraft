#include "../include/graphic.h"
#include "../include/input.h"
#include "../include/camera.h"
#include "../include/texture.h"
#include "../include/block.h"
#include "../include/chunk.h"

#define WIDTH 800
#define HEIGHT 600

static void silent_failure(bool *loop, const char *msg){
    if(msg != NULL) printf(msg);
    loop = false;
}

int main(){

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

    glClearColor(0.529f,0.807f,0.922f,1.0f);

    unsigned short indices[] = {
        0, 1, 2,   2, 3, 0,        // back
        4, 5, 6,   6, 7, 4,        // front
        8, 9,10,  10,11, 8,        // left
        12,13,14,  14,15,12,        // right
        16,17,18,  18,19,16,        // bottom
        20,21,22,  22,23,20         // top
    };

    glEnable(GL_DEPTH_TEST);
    glViewport(0,0,WIDTH,HEIGHT);

    Chunk chunks[9];
    BIDS *bid = NULL;
    if(generateChunks(chunks,&bid) == 0)silent_failure(&loop,"Chunks failed\n");

    Mesh *meshes = NULL;
   
    for(int y = 0; y < 9; y++){
        if(generateMeshes(&chunks[y],bid) == 0)silent_failure(&loop,"Meshes failed\n");   
    }
    
    if(concatenateMeshes(chunks,&meshes,bid,9,indices) == 0) silent_failure(&loop,"Concatenation of meshes failed\n");
   
    

    int handles[3] = {0,0,0};

    //paths
    const char *vs = "/Users/user/Developer/multiCraft/shaders/vertex.vs";
    const char *fs = "/Users/user/Developer/multiCraft/shaders/fragment.fs";

    if(shaders_init(vs,fs,handles) == 1) return safe_exit("Erreur shaders",window,context);
   
    //event
    Uint8 *keys;
    Mouse mouse;

    //some general int
    unsigned int matrix = 0;
    int counter = 0;

    //camera portion
    Camera camera;
    vec3s position = {24.0f,16.0f,24.0f};
    vec3s look = {0.0f,2.0f,0.0f};
    initCamera(&camera,position,look);

    //tick
    Tick tick;
    initTime(&tick);

    //texture 
    unsigned int texture;
    if(initTexture("/Users/user/Developer/multiCraft/sprites/block.png",&texture)==1) return safe_exit("Erreor Texture",window,context);

    //fps counter
    int fps = 0;
    int frames = 0;
    Tick fps_timer;
    initTime(&fps_timer);
    char *fps_string = NULL;
    char *string_fps = "FPS: ";
    char *final_fps_string = NULL;

    //MAIN LOOP
    while(loop){ 
        //printf("loop\n");
        fps_counter(&fps,&frames,&fps_timer);
        if(fps > 0 && frames == 0){ 
           number_to_string(fps,&fps_string);
           size_t test = string_len(string_fps);
           concatenate_string(string_fps,fps_string,&final_fps_string);
           SDL_SetWindowTitle(window,final_fps_string);
           free(fps_string);
           free(final_fps_string);
        }
        deltaTime(&tick);
        camera.View = glms_lookat(camera.position,camera.look,camera.up);
        matrix_init(camera.View,handles[0],&matrix,&counter);
        render(meshes,3,handles[0],texture);
        SDL_GL_SwapWindow(window);
        SDL_Event event;
        while(SDL_PollEvent(&event) == 1){
            switch(event.type){
                case SDL_EVENT_QUIT: loop = false; break;
                default: {
                    keys = getKeys();
                    mouse = getMouse(event);
                    cameraMovement(keys,mouse,&camera,tick.delta);
                }
            }
        }
    }
    
    destroyBIDS(&bid);
    for(int z = 0; z < 9; z++){destroyChunks(&chunks[z]);}
    destroyMeshes(&meshes,1);
    shaders_destroy(handles[1],handles[2],handles[0]);
    destroyTexture(&texture);
    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

/** GIANT TODO LIST FOR COMMIT ?
 * For the next commit 
 * I'd need to work on my perlin noise generation
 * or world generation in general
 * And test my generateVisibleBlocks optimisation (DONE partially)
 * I think that would be it
 * For what's done:
 * the mesh logic for the generateVisibleBlocks is done
 * I'd need to maybe modified generateVisibleBlocks
 **/
