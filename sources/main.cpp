#include <iostream>
#include <filesystem>

#include <GL/glew.h>

#include "class_display.hpp"
#include "class_opengl.hpp"
#include "class_opencl.hpp"
#include "objects.hpp"


std::vector<std::string> kernel_files{
    "sources/kernels/render_texture.cl"
};
std::vector<std::string> kernels{
    "render_texture"
};


int main(int argc, char **argv)
{
    Display display(1024, 768, std::filesystem::path(argv[0]).filename());

    display.open();
    OpenGL opengl(display.get_window());
    OpenCL opencl(display.get_window());

    opengl.init();
    opengl.load_shaders("sources/shaders/vertex.glsl", "sources/shaders/fragment.glsl");
    opengl.create_texture();
    opengl.create_mvp();

    struct World                world;
    struct Camera               camera;
    std::vector<struct Object>  objects{3};

    world =
    {
        .c = 30,
        .G = 3.54,
        .ray_step = 0.1
    };

    camera =
    {
        .position = {0, 0, 200},
        .direction = {0, 0, -1}
    };

    objects[0] =
    {
        .position = {0, 0, -600},
        .direction = {0, 1, 0},
        .velocity = {0, 0, 0},
        .mass = 10000,

        .radius = 0,
        .color = {0, 0, 0},
        .object_type = BLACKHOLE
    };
    objects[0].radius = (2 * world.G * objects[0].mass) / (world.c * world.c);

    objects[1] =
    {
        .position = {-200, 200, -800},
        .direction = {0, 1, 0},
        .velocity = {30, 0, 0},
        .mass = 0,

        .radius = 50,
        .color = {1, 0, 0},
        .object_type = SPHERE
    };

    objects[2] =
    {
        .position = {200, 0, -600},
        .direction = {0, 1, 0},
        .velocity = {-30, 0, 0},
        .mass = 0,

        .radius = 50,
        .color = {0, 1, 0},
        .object_type = SPHERE
    };

    opencl.init(kernel_files);
    opencl.create_display_kernel(opengl.get_texture(),
                                (cl_int2){opengl.get_texture_width(), opengl.get_texture_height()},
                                world,
                                camera,
                                objects,
                                objects.size(),
                                kernels[0]);
    opencl.execute_display_kernel(opengl.get_texture_width() * opengl.get_texture_height());

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