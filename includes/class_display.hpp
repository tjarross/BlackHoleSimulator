#include <GLFW/glfw3.h>


class Display
{
    public:
        Display(int window_width, int window_height, std::string window_name);
        ~Display(void);

        int         open(void);
        void        close(void);

        int         get_window_width(void);
        int         get_window_height(void);
        std::string get_window_name(void);

        void        set_window_name(std::string name);

        bool        is_window_open(void);
        void        swap_buffers(void);
        void        poll_events(void);

    private:
        int         _ogl_version_major;
        int         _ogl_version_minor;

        int         _w_width;
        int         _w_height;
        std::string _w_name;

        GLFWwindow  *_window;
};