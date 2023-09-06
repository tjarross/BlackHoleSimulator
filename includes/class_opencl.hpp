#pragma once

#include <string>
#include <vector>

#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>
#include <GLFW/glfw3.h>


class OpenCL
{
    public:
        OpenCL(GLFWwindow *window);
        ~OpenCL(void);

        cl_int  init(std::vector<std::string> kernel_files);

        cl_int  create_display_kernel(GLuint                    display_texture,
                                    cl_int2                     texture_dimensions,
                                    struct World                world,
                                    struct Camera               camera,
                                    std::vector<struct Object>  objects,
                                    cl_int                      objects_len,
                                    std::string                 kernel_name);
        cl_int  execute_display_kernel(size_t cores);
        cl_int  update_kernel(struct Camera             camera,
                            std::vector<struct Object>  objects,
                            cl_int                      objects_len);
    private:
        GLFWwindow      *_window;

        cl_platform_id      _platform_id;
        cl_device_id        _device_id;
        cl_uint             _number_platforms;
        cl_context          _context;
        cl_command_queue    _command_queue;
        cl_program          _program;
        cl_kernel           _kernel;

        cl_mem              _texture_buffer;
        cl_mem              _cl_objects;
};