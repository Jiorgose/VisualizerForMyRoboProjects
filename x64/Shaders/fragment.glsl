#version 460

in vec2 UV;

out vec4 color;

void main()
{
    color = vec4(UV, 0.0, 1.0);
}