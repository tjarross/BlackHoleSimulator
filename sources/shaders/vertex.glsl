#version 460 core

layout(location = 0) in vec3 vertex_coordinates;
layout(location = 1) in vec3 vertex_color;

out vec3 color;

void main()
{
    color = vertex_color;
    gl_Position.xyz = vertex_coordinates;
    gl_Position.w = 1.0;
}