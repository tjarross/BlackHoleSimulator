#version 460 core

in vec2             texture_coordinates;
uniform	sampler2D	texture_data;

out vec4            pixel_color;

void main(void)
{
    pixel_color = texture(texture_data, texture_coordinates);
}