#include "class_opencl.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <CL/cl_gl.h>
#include <stdio.h>

#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#include "GLFW/glfw3native.h"

#include "objects.hpp"


OpenCL::OpenCL(GLFWwindow *window):
    _window(window)
{
}


OpenCL::~OpenCL(void)
{
}


cl_int OpenCL::init(std::vector<std::string> kernel_files)
{
    cl_int     ret;
    FILE    *fp;
	std::vector<char> error_message(0x100000);

	if ((ret = clGetPlatformIDs(1, &_platform_id, &_number_platforms)) != CL_SUCCESS)
		return (ret);

	cl_context_properties props[] =
	{
		CL_GL_CONTEXT_KHR, (cl_context_properties)glfwGetGLXContext(_window),
		CL_GLX_DISPLAY_KHR, (cl_context_properties)glfwGetX11Display(),
		CL_CONTEXT_PLATFORM, (cl_context_properties)_platform_id,
		0
	};

	if ((ret = clGetGLContextInfoKHR(props, CL_CURRENT_DEVICE_FOR_GL_CONTEXT_KHR,
					sizeof(cl_device_id), &_device_id, NULL)) != CL_SUCCESS)
		return (ret);
	_context = clCreateContext(props, 1, &_device_id, NULL, NULL, &ret);
	if (ret != CL_SUCCESS)
		return (ret);

	_command_queue = clCreateCommandQueueWithProperties(_context, _device_id, 0, &ret);
	if (ret != CL_SUCCESS)
		return (ret);

    std::string source;
    for (std::string& file: kernel_files)
    {
        std::ifstream source_file_stream(file, std::ios::in);
        if(source_file_stream.is_open())
        {
            std::stringstream sstr;
            sstr << source_file_stream.rdbuf();
            source += sstr.str();
            source_file_stream.close();
        }
        else
            return (1);
    }

    size_t source_len = source.length();
	_program = clCreateProgramWithSource(_context, 1,
			(const char **)&source, (const size_t *)&source_len, &ret);
	if (ret != CL_SUCCESS)
		return (ret);

	if ((ret = clBuildProgram(_program, 1, &_device_id, "-cl-fast-relaxed-math", NULL, NULL)) != CL_SUCCESS)
	{
		if ((ret = clGetProgramBuildInfo(_program, _device_id,
				CL_PROGRAM_BUILD_LOG, 0x100000, &error_message[0], NULL)) != CL_SUCCESS)
			return (ret);
		std::cerr << &error_message[0] << std::endl;
		return (ret);
	}

    return (CL_SUCCESS);
}

cl_int OpenCL::create_display_kernel(GLuint                     display_texture,
                                    cl_int2                     texture_dimensions,
                                    struct World                world,
                                    struct Camera               camera,
                                    std::vector<struct Object>  objects,
                                    cl_int                      objects_len,
                                    std::string                 kernel_name)
{
    cl_int  ret;

    _kernel = clCreateKernel(_program, kernel_name.c_str(), &ret);
    if (ret != CL_SUCCESS)
		return (ret);

    _texture_buffer = clCreateFromGLTexture(_context, CL_MEM_WRITE_ONLY & CL_MEM_COPY_HOST_PTR, GL_TEXTURE_2D, 0, display_texture, &ret);
    if (ret != CL_SUCCESS)
        return (ret);

    _cl_objects = clCreateBuffer(_context, CL_MEM_COPY_HOST_PTR, sizeof(struct Object) * objects_len, &objects[0], &ret);
    if (ret != CL_SUCCESS)
        return (ret);

    if ((ret = clSetKernelArg(_kernel, 0, sizeof(cl_mem), &_texture_buffer)) != CL_SUCCESS)
        return (ret);

    if ((ret = clSetKernelArg(_kernel, 1, sizeof(cl_int2), &texture_dimensions)) != CL_SUCCESS)
        return (ret);

    if ((ret = clSetKernelArg(_kernel, 2, sizeof(struct World), &world)) != CL_SUCCESS)
        return (ret);

    if ((ret = clSetKernelArg(_kernel, 3, sizeof(struct Camera), &camera)) != CL_SUCCESS)
        return (ret);

    if ((ret = clSetKernelArg(_kernel, 4, sizeof(cl_mem), &_cl_objects)) != CL_SUCCESS)
        return (ret);

    if ((ret = clSetKernelArg(_kernel, 5, sizeof(cl_int), &objects_len)) != CL_SUCCESS)
        return (ret);
    
    return (CL_SUCCESS);
}


cl_int OpenCL::update_kernel(struct Camera              camera,
                            std::vector<struct Object>  objects,
                            cl_int                      objects_len)
{
    cl_int  ret;

    if ((ret = clSetKernelArg(_kernel, 3, sizeof(struct Camera), &camera)) != CL_SUCCESS)
        return (ret);

    if ((ret = clSetKernelArg(_kernel, 4, sizeof(cl_mem), &_cl_objects)) != CL_SUCCESS)
        return (ret);

    if ((ret = clSetKernelArg(_kernel, 5, sizeof(cl_int), &objects_len)) != CL_SUCCESS)
        return (ret);
    
    return (CL_SUCCESS);
}


cl_int OpenCL::execute_display_kernel(size_t cores)
{
    cl_int ret;

    glFinish();
    if ((ret = clEnqueueAcquireGLObjects(_command_queue, 1, &_texture_buffer, 0, NULL, NULL)) != CL_SUCCESS)
		return (ret);
    if ((ret = clEnqueueNDRangeKernel(_command_queue, _kernel, 1, NULL, &cores, NULL, 0, NULL, NULL)) != CL_SUCCESS)
		return (ret);
	if ((ret = clEnqueueReleaseGLObjects(_command_queue, 1, &_texture_buffer, 0, NULL, NULL)) != CL_SUCCESS)
        return (ret);
	if ((ret = clFinish(_command_queue)) != CL_SUCCESS)
        return (ret);

    return (CL_SUCCESS);
}