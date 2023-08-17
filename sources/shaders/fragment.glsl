#version 460 core

out vec3 pixel_color;
in vec3 color;

void main()
{
    pixel_color = color;
}