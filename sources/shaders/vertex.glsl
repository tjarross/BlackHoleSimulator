#version 460 core

layout(location = 0) in vec3 vertex_coordinates;
layout(location = 1) in vec2 texture_UV;

uniform mat4    MVP;

out vec2        texture_coordinates;

void main(void)
{
    vec4 world_coords = MVP * vec4(vertex_coordinates, 1);
    gl_Position = world_coords;
	texture_coordinates = texture_UV;
}