#include "../include/graphic.h"

#define WIDTH 800
#define HEIGHT 600

int main(){
    //printf("Hello World !\n");

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

    //SDL_GL_SetSwapInterval(-1);

    glClearColor(1.0f,0.0f,0.0f,1.0f);

    float vertices[] = {
      // Positions (x, y, z)
      -0.5f, -0.5f, -0.5f,  // 0
       0.5f, -0.5f, -0.5f,  // 1
       0.5f,  0.5f, -0.5f,  // 2
      -0.5f,  0.5f, -0.5f,  // 3
      -0.5f, -0.5f,  0.5f,  // 4
       0.5f, -0.5f,  0.5f,  // 5
       0.5f,  0.5f,  0.5f,  // 6
      -0.5f,  0.5f,  0.5f   // 7
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

    unsigned int VBO, VAO, EBO;
    vbo_init(&VBO,vertices,sizeof(vertices));
    vao_init(&VAO);
    ebo_init(&EBO,indices,sizeof(indices));

    int handles[3] = {0,0,0};
    /*const char *test0 = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main(){\n"
    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

    const char *test1 = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main(){\n"
    "FragColor = vec4(1.0f,0.5f,0.2f,1.0f);\n"
    "}\0";*/

    const char *vs = "/Users/user/Developer/multiCraft/shaders/vertex.vs";
    const char *fs = "/Users/user/Developer/multiCraft/shaders/color.fs";

    if(shaders_init(vs,fs,handles) == 1) return safe_exit("Erreur shaders",window,context);
    //if(shaders_init(test0,test1,handles) == 1) return safe_exit("Erreur shaders",window,context);


    while(loop){
        render(VAO,handles[0]);
        SDL_GL_SwapWindow(window);
        SDL_Event event;
        while(SDL_PollEvent(&event) == 1){
            switch(event.type){
                case SDL_EVENT_QUIT: loop = false; break;
                case SDL_EVENT_KEY_DOWN:
                    switch(event.key.key){
                        case SDLK_ESCAPE: return safe_exit("Test",window,context);break;
                    }break;
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