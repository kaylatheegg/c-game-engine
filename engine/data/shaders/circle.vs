#version 330 core
in vec2 position;

void main()
{
    gl_Position = vec4(position.x/800, position.y/800, 0.0, 1.0);
}