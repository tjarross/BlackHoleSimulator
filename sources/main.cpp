#include <iostream>
#include <chrono>
#include <thread>

#include "class_display.hpp"

int main(int argc, char **argv)
{
    Display display(1024, 768, std::string("CPPExperiment"));

    display.open();
    
    while (display.is_window_open())
    {
        display.swap_buffers();
        display.poll_events();
    }

    display.close();
}