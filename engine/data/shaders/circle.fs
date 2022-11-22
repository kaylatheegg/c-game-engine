#version 330 core

uniform vec2 circleCenter;
uniform vec3 circleColour;

out vec4 FragColor;

void main() {
   FragColor = vec4(circleColour.r, circleColour.g, circleColour.b, 1.0);
}
