#version 330 core

uniform vec2 circleCenter;
uniform vec3 circleColour;
uniform float radius;

out vec4 FragColor;

in vec4 gl_FragCoord;

void main() {
   if (distance(gl_FragCoord.xy - vec2(0.5, 0.5), circleCenter) >= radius - 2 && distance(gl_FragCoord.xy - vec2(0.5, 0.5), circleCenter) <= radius) {
      FragColor = vec4(circleColour.r/255, circleColour.g/255, circleColour.b/255, 1.0);
   } else {
      discard;
   }
}


//notes about the circle:
//its bugged because the circleCenter isnt being transformed right
//causing the radius calculation to break
//no i dont know how to fix this