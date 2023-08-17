
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstring>

#include "class_display.hpp"


Display::Display(int window_width, int window_height, std::string window_name) :
                _ogl_version_major(4),
                _ogl_version_minor(6),
                _w_width(window_width),
                _w_height(window_height),
                _w_name(window_name),
                _window(nullptr)
{
}


Display::~Display()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}


int Display::open()
{
    if (glfwInit() != GLFW_TRUE)
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return (-1);
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _ogl_version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _ogl_version_minor);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _window = glfwCreateWindow(_w_width, _w_height, _w_name.c_str(), NULL, NULL);
    if (_window == NULL){
        std::cerr << "Failed to open GLFW window." << std::endl;
        return (-1);
    }
    glfwMakeContextCurrent(_window);

    glewExperimental=true;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return (-1);
    }

    glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);
    return (0);
}


int Display::get_window_width(void)
{
    return (_w_width);
}


int Display::get_window_height(void)
{
    return (_w_height);
}


std::string Display::get_window_name(void)
{
    return (_w_name);
}


void Display::set_window_name(std::string name)
{
    glfwSetWindowTitle(_window, name.c_str());
}


bool Display::is_window_open(void)
{
    return (glfwGetKey(_window, GLFW_KEY_ESCAPE ) != GLFW_PRESS
            && glfwWindowShouldClose(_window) == 0);
}


void Display::swap_buffers(void)
{
    glfwSwapBuffers(_window);
}


void Display::poll_events()
{
    glfwPollEvents();
}

void Display::close()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}