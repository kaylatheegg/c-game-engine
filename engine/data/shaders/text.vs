#version 330 core


in vec2 texcoord;

in vec2 position;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(position.x/800, position.y/800, 0.0, 1.0);
    TexCoords = texcoord;
}  