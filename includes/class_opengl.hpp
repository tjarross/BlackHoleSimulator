#pragma once

#include "glm/glm.hpp"

class OpenGL
{
    public:
        OpenGL(GLFWwindow *window);
        ~OpenGL();

        void init(void);

        int  load_shaders(std::string vertex_shader_filepath, std::string fragment_shader_filepath);
        void create_triangle(void);
        void create_mvp(void);
        void load_mvp(void);
        void update_mvp(void);
        void draw(void);
        void process_inputs(void);

    private:
        void compute_delta_time(void);

        GLuint      _vao;
        GLuint      _program_id;

        float       _window_ratio;
        float       _near_plane;
        float       _far_plane;
        glm::mat4   _model;
        glm::mat4   _view;
        glm::mat4   _projection;
        glm::mat4   _mvp;
        GLuint      _matrix_id;

        GLFWwindow *_window;
        int         _w_width;
        int         _w_height;

        glm::vec3   _position;
        glm::vec3   _direction;
        glm::vec3   _up;
        float       _delta_time;
        float       _speed;
        float       _mouse_speed;
        float       _horizontal_angle;
        float       _vertical_angle;
};