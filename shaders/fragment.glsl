#version 330 core
out vec4 FragColor;
in vec3 fCol;
in vec2 TexCoord;

uniform float time;
uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord) * vec4(fCol, 1.f);
} 