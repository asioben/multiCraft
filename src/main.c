#include "../include/graphic.h"
#include "../include/input.h"
#include "../include/camera.h"
#include "../include/texture.h"
#include "../include/block.h"

#define WIDTH 800
#define HEIGHT 600

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

    Mesh meshes[3];

    glEnable(GL_DEPTH_TEST);
    glViewport(0,0,WIDTH,HEIGHT);
    //init  some objects
    for(int i = 0; i < 3; i++){
        for (int j = 0; j < 36; j++) meshes[i].indices[j] = indices[j];
        generateCube(meshes[i].vertices,i);
        /**vao_init(&meshes[i].VAO);
        vbo_init(&meshes[i].VBO,meshes[i].vertices,sizeof(meshes[i].vertices));
        ebo_init(&meshes[i].EBO,meshes[i].indices,sizeof(meshes[i].indices));**/
        vertex_init();
    }
   
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
    vec3s position = {0.0f,0.0f,5.0f};
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

    //model matrix
    float x = 0.0f;
    float z = 0.0f;
    for(int j = 0; j < 3; j++){
        vec3s positions[100];
        for(int i = 0; i < 100; i++){
          if(i % 10 == 0){
            x = 0.0f;
            z -= 1.0f;
          }
          positions[i].x = x;
          positions[i].y = 0.0f;
          positions[i].z = z;
          x += 1;
          glm_mat4_identity(meshes[j].model[i]);
          vec3 position_ = {positions[i].x,positions[i].y,positions[i].z};
          glm_translate(meshes[j].model[i],position_);
      }
      instance_init(meshes[j].VAO,&meshes[j].instance,meshes[j].model,sizeof(mat4) * 100);
    }



    //MAIN LOOP
    while(loop){ 
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
        render(meshes,handles[0],texture);
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
    
    for(int i = 0; i < 3; i++){
        vbo_ebo_destroy(&meshes[i].VBO,&meshes[i].EBO);
        vbo_ebo_destroy(&meshes[i].instance,NULL);
        vao_destroy(&meshes[i].VAO);
    }
    shaders_destroy(handles[1],handles[2],handles[0]);
    destroyTexture(&texture);
    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

/** GIANT TODO_LIST FOR COMMIT 10
 * Make cube instances with world matrix
 * Rewrite a bit the graphic layer (new buffers for instance and multiple textures (vertex (uv)))
 **/
