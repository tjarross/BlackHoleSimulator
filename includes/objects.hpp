#pragma once

#include <CL/cl.h>


enum ObjectType
{
    NONE = 0,
    CAMERA = 1,
    BLACKHOLE = 2,
    SPHERE = 3
};

struct World
{
    float c;
    float G;

    float ray_step;
};

struct Camera
{
    cl_float3   position;
    cl_float3   direction;
};

struct Object
{
    cl_float3       position;
    cl_float3       direction;
    cl_float3       velocity;
    float           mass;

    float           radius;
    cl_float3       color;

    enum ObjectType object_type;
};