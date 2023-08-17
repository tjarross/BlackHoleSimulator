#version 460 core

layout(location = 0) in vec3 vertex_coordinates;
layout(location = 1) in vec3 vertex_color;

uniform mat4 MVP;

out vec3 color;

void main(void)
{
    color = vertex_color;
    gl_Position = MVP * vec4(vertex_coordinates, 1);
}