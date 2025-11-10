#version 330 core
layout (location = 0) in vec3 aPos; // this shit need in main.cpp, in glVetexAttribPointer as first arg
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aTexCoord;

out vec3 fCol;
out vec2 TexCoord;

uniform float time;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.f);
    fCol = aCol;
    TexCoord = aTexCoord;
}