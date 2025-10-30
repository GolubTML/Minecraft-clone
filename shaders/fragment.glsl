#version 330 core
out vec4 FragColor;
in vec3 fCol;

uniform float time;

void main()
{
    FragColor = vec4(fCol * sin(time), 1.0f);
} 