#include <GL/glew.h>
#include <iostream>
#include <chrono>
#include <thread>

#include "class_display.hpp"
#include "class_opengl.hpp"

int main(int argc, char **argv)
{
    Display display(1024, 768, std::string("CPPExperiment"));
    OpenGL opengl;

    display.open();
    opengl.init();

    opengl.load_shaders("sources/shaders/vertex.glsl", "sources/shaders/fragment.glsl");
    opengl.create_triangle();
    
    display.set_background_color(0.6f, 0.6f, 0.8f, 0.0f);
    while (display.is_window_open())
    {
        display.clear();
        opengl.draw();
        display.swap_buffers();
        display.poll_events();
    }

    display.close();
}