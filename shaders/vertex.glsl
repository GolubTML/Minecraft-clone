#version 330 core
layout (location = 0) in vec3 aPos; // this shit need in main.cpp, in glVetexAttribPointer as first arg
layout (location = 1) in vec3 aCol;

out vec3 fCol;

void main()
{
    gl_Position = vec4(aPos, 1.f);
    fCol = aCol;
}