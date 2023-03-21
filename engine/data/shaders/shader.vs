#version 330 core
in vec2 position;

in vec2 texcoord;

out vec2 Texcoord;

uniform mat4 transMatrix;

void main()
{

    gl_Position = transMatrix * vec4(position, 0.0, 1.0);

    Texcoord = texcoord;
}