// __constant float c = 30; // 299792458; // m/s
// __constant float G = 3.54; // 6.67 * pow(10.f, -11);

// __constant float ray_step = 0.1;

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
    float3  position;
    float3  direction;
};

struct Object
{
    float3          position;
    float3          direction;
    float3          velocity;
    float           mass;

    float           radius;
    float3          color;

    enum ObjectType object_type;
};


void new_magnitude(float3 *vector, float current_magnitude, float new_magnitude)
{
    *vector *= new_magnitude / current_magnitude;
}


void apply_forces(struct Object *object, struct Object *ray, float G)
{
    float3 force = object->position - ray->position;
    float r = distance(object->position, ray->position);
    float gravity_force = (G * object->mass) / (r * r);

    new_magnitude(&force, r, gravity_force);

    ray->velocity += force;
}


bool intersection(struct Object ray, struct Object object, int objects_len)
{
    if (object.object_type == SPHERE
     || object.object_type == BLACKHOLE)
        if (distance(ray.position, object.position) < object.radius)
            return (true);
    
    return (false);
}


float3 apply_fog(float3 color, float3 fog_color, float3 fog_factor)
{
    return mix(color, fog_color, fog_factor);
}


float3 cast_ray(struct World world, struct Object ray, __global struct Object *objects, int objects_len)
{
    int     i = 0;
    int     j;
    int     max_iterations = 500;
    float3  fog_color = {1, 1, 0.8};
    float   fog_mix_factor = 0.f;

    while (i < max_iterations)
    {
        if (i / (float)max_iterations > 0.6f)
            fog_mix_factor = clamp((i - (max_iterations * 0.8f)) / (float)(max_iterations - i), 0.f, 1.f);
        j = 0;
        while (j < objects_len)
        {
            if (intersection(ray, objects[j], objects_len) == true)
                return (apply_fog(objects[j].color, fog_color, fog_mix_factor));
            j++;
        }
        ray.position += ray.velocity * world.ray_step;

        j = 0;
        while (j < objects_len)
        {
            apply_forces(objects + j, &ray, world.G);
            j++;
        }
        i++;
    }

    return (fog_color);
}


__kernel void render_texture(__write_only image2d_t img,
                            int2                    texture_dimensions,
                            struct World            world,
                            struct Camera           camera,
                            __global struct Object  *objects,
                            int                     objects_len)
{
	size_t	gid = get_global_id(0);

    int     width = texture_dimensions.x;
    int     height = texture_dimensions.y;
	int2	pixel_position = (int2){gid % width, gid / width};

    struct Object ray;

    ray.position = (float3)
    {
        camera.position.x - (width / 2.f) + pixel_position.x,
        camera.position.y - (height / 2.f) + pixel_position.y,
        camera.position.z
    };
    ray.velocity = camera.direction * world.c;

    float3 color = cast_ray(world, ray, objects, objects_len);
    write_imagef(img, pixel_position, (float4){color, 1});
}
