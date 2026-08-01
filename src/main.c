#include "../include/graphic.h"
#include "../include/input.h"
#include "../include/camera.h"
#include "../include/texture.h"
#include "../include/block.h"
#include "../include/chunk.h"
#include "../include/chunkManager.h"
#include "../include/physic.h"
#include "../include/scene.h"

#include <time.h>

#define WIDTH 1000
#define HEIGHT 750

#define IMPL true

static void silent_failure(bool *loop, const char *msg){
    if(msg != NULL) printf(msg);
    loop = false;
}

int main(){

    srand(time(NULL));

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

    //backface culling
    /*glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);*/
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    unsigned short indices[] = {
        0, 1, 2,   2, 3, 0,        // back
        4, 5, 6,   6, 7, 4,        // front
        8, 9,10,  10,11, 8,        // left
        12,13,14,  14,15,12,        // right
        16,17,18,  18,19,16,        // bottom
        20,21,22,  22,23,20         // top
    };

    //in case again
    glEnable(GL_DEPTH_TEST);
    glViewport(0,0,WIDTH,HEIGHT);

    //in case
    //glDepthMask(GL_TRUE);

    int size = (int)sqrt(CHUNKS_LIMIT);
    int area = CHUNKS_LIMIT;
    //printf("size:%d\n",size);

    //int test_number = 0;

    //Chunk chunks[size];
    ChunkManager *chunkManager = NULL; 
    BIDS *bid = NULL;
    if(generateChunks(&chunkManager,&bid,size) == 0)silent_failure(&loop,"Chunks failed\n");

    /*Arena arena;
    arena_init(&arena,16000000);*/
    Mesh *meshes = NULL;
    if(loadChunks(chunkManager,&bid,&meshes,indices) == 0)silent_failure(&loop,"Loading chunks failed\n");

    unsigned int handles[3] = {0,0,0};

    //paths
    const char *vs = "/Users/user/Developer/multiCraft/shaders/2dvertex.vs";
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
    vec3s position = { size * 4.0f,32.0f,size * 4.0f};
    vec3s look = {0.0f,2.0f,0.0f};
    initCamera(&camera,position,look);

    //tick
    Tick tick;
    initTime(&tick);

    //texture 
    unsigned int texture;
    //if(initTexture("/Users/user/Developer/multiCraft/sprites/title.png",&texture)==1) return safe_exit("Error Texture",window,context);

    unsigned int titleVAO;
    unsigned int titleVBO;
    unsigned int titleTexture;
    unsigned int titleEBO;

    if(initTitle(&titleVAO,&titleVBO,&titleTexture) == 0) return safe_exit("Error Title",window,context);

    //fps counter
    int fps = 0;
    int frames = 0;
    Tick fps_timer;
    initTime(&fps_timer);
    char *fps_string = NULL;
    char *string_fps = "FPS: ";
    char *final_fps_string = NULL;

    BlockID current_block = GRASS;

    //cycle (Tick)
    Tick cycle;
    initTime(&cycle);
    int cycle_per_second = 20;
    Uint64 cycle_time = (int)(1000000 / cycle_per_second);
    int use_cycle = 0;

    //basic movement
    vec3 acceleration = {0.0f,-0.005f,0.0f};
    vec3 speed_position = {0.0f,0.0f,0.0f};
    vec3 speed_look = {0.0f,0.0f,0.0f};

    //bool collision = false;
    bool on_cube = false;
    Box cube;
    Box hitbox;

    cube.padding[0] = 1.0f;
    cube.padding[1] = 1.0f;
    cube.padding[2] = 1.0f;

    hitbox.padding[0] = 1.0f;
    hitbox.padding[1] = -3.0f;
    hitbox.padding[2] = 1.0f;

    long loop_counter = 0;

    vec3 direction = {0.0f,0.0f,0.0f};

    //Scene
    Scene scene = TITLE;

    //MAIN LOOP
    while(loop){ 
      if(scene == GAME){
        loop_counter ++;
        //printf("loop\n");
        //loadChunks(chunkManager);
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
        //printf("%d\n",tick.delta);
        if(timeCounter(&cycle,cycle_time)){
            use_cycle = 0;
        }
        //printf("%i\n",tick.delta);
        if(use_cycle == 0 && IMPL == true){
            bool collision = false;
            if(on_cube == false){
                accelerate(acceleration,speed_position,camera.position.raw,((float)cycle.delta / 1000000));
                accelerate(acceleration,speed_look,camera.look.raw,((float)cycle.delta / 1000000));
                //printf("Lookat: %f,%f,%f\n",camera.look.x,camera.look.y,camera.look.z);
                //printf("Position: %f,%f,%f\n",camera.position.x,camera.position.y,camera.position.z);
                //if(on_cube) loop = false;
            }
            on_cube = false;
            for(int v = 0; v < chunkManager->chunks[chunkManager->currentChunk].meshesSize; v++){
                for(int c = 0; c < chunkManager->chunks[chunkManager->currentChunk].meshSize[v]; c++){

                    glm_vec3_copy(chunkManager->chunks[chunkManager->currentChunk].blocks[chunkManager->chunks[chunkManager->currentChunk].models[v][c]].model[3],cube.position);
                    glm_vec3_copy(camera.position.raw,hitbox.position);
                    collision = boxToBoxCollision(cube,hitbox);

                    if(collision){
                        vec3 diff;

                        glm_vec3_sub(cube.position,camera.position.raw,diff);

                        if(diff[1] <= -1.0f){
                            camera.position.y = cube.position[1] + 2.0f;
                            glm_vec3_zero(speed_look);
                            glm_vec3_zero(speed_position);
                        //camera.look.y = cube.position[1] + 2.0f;
                        }

                        if(diff[0] >= 1.0f || diff[0] <= -1.0f){
                            camera.position.x -= direction[0];
                        }

                        if(diff[2] >= 1.0f || diff[2] <= -1.0f){
                            camera.position.z -= direction[2];
                        }

                        printf("Direction: %f,%f,%f\n",direction[0],direction[1],direction[2]);
                        printf("diff: %f,%f,%f\n",diff[0],diff[1],diff[2]);

                        printf("Block: %f,%f,%f\n",cube.position[0],cube.position[1],cube.position[2]);
                        printf("Player: %f,%f,%f\n",camera.position.x,camera.position.y,camera.position.z);
                        printf("Look: %f,%f,%f\n",camera.look.x,camera.look.y,camera.look.z);
                        printf("loop:%d\n",loop_counter);
                        printf("/////////////\n");


                        on_cube = true;
                        //loop = false;
                        //break;
                    }
                    //if(collision) break;
                }
            }
        }
        camera.View = glms_lookat(camera.position,camera.look,camera.up);
        matrix_init(camera.View,camera.Projection,handles[0],&matrix,&counter);
        //render(meshes,BLOCKS_LIMIT,handles[0],texture);
    }else{
        //
        //sceneManagement(&scene,);
        //texture = titleTexture;
    }
        sceneManagement(&scene,meshes,handles[0],texture,titleVAO);
        SDL_GL_SwapWindow(window);
        SDL_Event event;
        while(SDL_PollEvent(&event) == 1){
            switch(event.type){
                case SDL_EVENT_QUIT: loop = false; break;
                default: {
                    if(use_cycle == 0) keys = getKeys();
                    mouse = getMouse(event);
                    if(mouse.left == 1 || mouse.right == 4 && use_cycle == 0){
                        vec2 m_ = {mouse.position.x,mouse.position.y};
                        vec2 s_ = {WIDTH,HEIGHT};
                        vec3 ray;

                        screenToWorld(m_,s_,camera.View.raw,camera.Projection.raw,ray);
                        if(mouse.left == 1 && use_cycle == 0){
                             //EVENT
                             //REMOVE = 0
                             //ADD = 1
                             updateBlock(chunkManager,&camera,&meshes,bid,ray,indices,0,current_block);
                        }else if(mouse.right == 4 && use_cycle == 0){
                             updateBlock(chunkManager,&camera,&meshes,bid,ray,indices,1,current_block);
                        }
                    }
                    if(cameraMovement(keys,mouse,&camera,tick.delta,direction) == 1){ 
                        if(getCurrentChunk(chunkManager,camera.position))
                        if(loadChunks(chunkManager,&bid,&meshes,indices) == 0)silent_failure(&loop,"Loading chunks failed.\n");
                    }
                    pickBlock(keys,&current_block);
                    use_cycle ++;
                }
            }
        }
    }
    
    destroyBIDS(&bid);
    destroyChunkManager(&chunkManager);
    destroyMeshes(&meshes,1);
    shaders_destroy(handles[1],handles[2],handles[0]);
    destroyTexture(&texture);
    //arena_free(&arena);
    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    vao_destroy(&titleVAO);
    vbo_ebo_destroy(&titleVBO,&titleEBO);

    return 0;
}

/** GIANT TODO LIST FOR COMMIT ?
 * For the next commit 
 * Work on some physics 
 * like gravity
 * continue improving 3D world selection algo
 * maybe add chunk frustrum culling
 * work and improve the word generation
 * maybe big terrain 
 * and also try to cull
 * block at the border of a chunk
 **/
