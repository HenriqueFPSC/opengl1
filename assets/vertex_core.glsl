#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTextCoord;

out vec3 ourColor;
out vec2 textCoord;

uniform mat4 transform;// Set in code

void main(){
    gl_Position = transform * vec4(aPos, 1.0f);
    // gl_Position = vec4(aPos, 1.0f);
    ourColor = aColor;
    textCoord = aTextCoord;
}