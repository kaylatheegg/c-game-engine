#version 330 core
in vec2 position;

uniform mat4 transMatrix;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0) * transMatrix;
}