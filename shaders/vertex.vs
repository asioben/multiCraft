#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in mat4 instance;

uniform mat4 VP;
out vec2 TexCoord;

void main(){
    gl_Position = VP * instance * vec4(aPos,1.0);
    TexCoord = aTexCoord;
}