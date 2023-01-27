#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextCoord;

out vec2 textCoord;

uniform mat4 model, view, projection;

void main(){
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    //gl_Position = vec4(aPos, 1.0f);
    textCoord = aTextCoord;
}