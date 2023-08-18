#include <GL/glew.h>
#include <iostream>
#include <chrono>
#include <thread>

#include "class_display.hpp"
#include "class_opengl.hpp"


int main(int argc, char **argv)
{
    Display display(1024, 768, std::string("CPPExperiment"));

    display.open();
    OpenGL opengl(display.get_window());

    opengl.init();
    opengl.load_shaders("sources/shaders/vertex.glsl", "sources/shaders/fragment.glsl");
    opengl.create_triangle();
    opengl.create_mvp();

    display.set_background_color(0.6f, 0.6f, 0.8f, 0.0f);
    while (display.is_window_open())
    {
        display.clear();
        
        opengl.process_inputs();
        opengl.update_mvp();
        opengl.load_mvp();
        opengl.draw();

        display.swap_buffers();
        display.poll_events();
    }

    display.close();
}