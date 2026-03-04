#include "../include/graphic.h"
#include "../include/input.h"
#include "../include/camera.h"
#include "../include/texture.h"

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

    glClearColor(1.0f,0.0f,0.0f,1.0f);

    float vertices[] = {
      // Positions (x, y, z)
      -0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  // 0
       0.5f, -0.5f, -0.5f, 1.0f, 0.0f,// 1
       0.5f,  0.5f, -0.5f, 0.0f, 0.0f// 2
      -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // 3
      -0.5f, -0.5f,  0.5f, 0.0f, 1.0f,// 4
       0.5f, -0.5f,  0.5f, 0.0f, 1.0f,// 5
       0.5f,  0.5f,  0.5f, 0.0f, 1.0f,// 6
      -0.5f,  0.5f,  0.5f, 0.0f, 1.0f// 7
    };

    unsigned short indices[] = {
    // Back face
    0, 1, 2,
    2, 3, 0,

    // Front face
    4, 5, 6,
    6, 7, 4,

    // Left face
    4, 0, 3,
    3, 7, 4,

    // Right face
    1, 5, 6,
    6, 2, 1,

    // Bottom face
    4, 5, 1,
    1, 0, 4,

    // Top face
    3, 2, 6,
    6, 7, 3
};

    glEnable(GL_DEPTH_TEST);
    glViewport(0,0,WIDTH,HEIGHT);
    //init  some objects
    unsigned int VBO, VAO, EBO;
    vao_init(&VAO);
    vbo_init(&VBO,vertices,sizeof(vertices));
    ebo_init(&EBO,indices,sizeof(indices));
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
    vec3s look = {0.0f,0.0f,0.0f};
    initCamera(&camera,position,look);

    //tick
    Tick tick;
    initTime(&tick);

    //texture 
    unsigned int texture;
    if(initTexture("/Users/user/Developer/multiCraft/sprites/block.png",&texture)==1) return safe_exit("Erreor Texture",window,context);
    while(loop){
        deltaTime(&tick);
        camera.View = glms_lookat(camera.position,camera.look,camera.up);
        matrix_init(camera.View,handles[0],&matrix,&counter);
        render(VAO,handles[0],texture);
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
    
    vbo_ebo_destroy(&VBO,&EBO);
    vao_destroy(&VAO);
    shaders_destroy(handles[1],handles[2],handles[0]);
    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}