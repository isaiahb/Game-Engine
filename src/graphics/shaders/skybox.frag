#version 330 core
in vec3 TexCoords;
out vec4 color;

uniform samplerCube skybox;

void main()
{
    float v = 1;
    float o = v;
    float m = v;
    float g = v;
    color = texture(skybox, TexCoords) * vec4(o, m, g, 1);
}