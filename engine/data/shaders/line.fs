#version 330 core

uniform vec3 lineColour;
out vec4 FragColor;

void main() {
    FragColor = vec4(lineColour.r/255, lineColour.g/255, lineColour.b/255, 1.0);
}