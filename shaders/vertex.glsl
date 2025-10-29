#version 330 core
layout (location = 0) in vec3 aPos; // this shit need in main.cpp, in glVetexAttribPointer as first arg

void main()
{
    gl_Position = vec4(aPos, 1.f);
}