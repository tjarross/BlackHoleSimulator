class OpenGL
{
    public:
        OpenGL();
        ~OpenGL();

        void init(void);

        int  load_shaders(std::string vertex_shader_filepath, std::string fragment_shader_filepath);
        void create_triangle(void);
        void draw(void);

    private:
        GLuint  _vao;
        GLuint  _program_id;
};