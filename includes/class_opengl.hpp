#include "glm/glm.hpp"

class OpenGL
{
    public:
        OpenGL();
        ~OpenGL();

        void init(float window_ratio);

        int  load_shaders(std::string vertex_shader_filepath, std::string fragment_shader_filepath);
        void create_triangle(void);
        void create_mvp(void);
        void load_mvp(void);
        void draw(void);

    private:
        GLuint      _vao;
        GLuint      _program_id;

        float       _window_ratio;
        float       _near_plane;
        float       _far_plane;
        glm::mat4   _mvp;
        GLuint      _matrix_id;
};