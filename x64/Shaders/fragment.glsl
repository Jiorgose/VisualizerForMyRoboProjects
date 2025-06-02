#version 460

uniform float time;
uniform vec3 sColor;
uniform float aspectRatio;

uniform vec3 cameraPosition;

in vec2 UV;
out vec4 color;

void main() {
  //col = 15.0 / 255.0;
  color = vec4(sColor, 1.0);
}
