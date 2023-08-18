#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "class_opengl.hpp"


OpenGL::OpenGL(GLFWwindow *window):
	_near_plane(0.1f),
	_far_plane(100.f),
	_position(glm::vec3(1, 0, 3)),
	_direction(glm::vec3(0, 0, 0)),
	_up(glm::vec3(0, 1, 0)),
	_delta_time(0.f),
	_speed(3.f),
	_mouse_speed(0.05f),
	_horizontal_angle(3.14f),
	_vertical_angle(0.f),
	_w_width(0),
	_w_height(0)
{
	_window = window;
}


OpenGL::~OpenGL()
{
}


void OpenGL::init(void)
{
	glfwGetWindowSize(_window, &_w_width, &_w_height);
	_window_ratio = static_cast<float>(_w_width) / static_cast<float>(_w_height);

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}


int OpenGL::load_shaders(std::string vertex_shader_filepath, std::string fragment_shader_filepath)
{
	GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertex_shader_code;
	std::ifstream vertex_file_stream(vertex_shader_filepath, std::ios::in);
	if(vertex_file_stream.is_open())
    {
		std::stringstream sstr;
		sstr << vertex_file_stream.rdbuf();
		vertex_shader_code = sstr.str();
		vertex_file_stream.close();
	}
    else
    {
        std::cerr << "Cannot open" << vertex_shader_filepath << std::endl;
		return 1;
	}

	std::string fragment_shader_code;
	std::ifstream fragment_file_stream(fragment_shader_filepath, std::ios::in);
	if(fragment_file_stream.is_open()){
		std::stringstream sstr;
		sstr << fragment_file_stream.rdbuf();
		fragment_shader_code = sstr.str();
		fragment_file_stream.close();
	}
    else
    {
        std::cerr << "Cannot open" << fragment_shader_filepath << std::endl;
		return 1;
	}

	GLint result = GL_FALSE;
	int log_length;

	const char *vertex_source_ptr = vertex_shader_code.c_str();
	glShaderSource(vertex_shader_id, 1, &vertex_source_ptr , NULL);
	glCompileShader(vertex_shader_id);

	glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &log_length);
	if (log_length > 0)
    {
		std::vector<char> error_message(log_length + 1);
		glGetShaderInfoLog(vertex_shader_id, log_length, NULL, &error_message[0]);
		printf("%s\n", &error_message[0]);
        return (1);
	}

	const char *fragment_source_ptr = fragment_shader_code.c_str();
	glShaderSource(fragment_shader_id, 1, &fragment_source_ptr , NULL);
	glCompileShader(fragment_shader_id);

	glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &log_length);
	if (log_length > 0)
    {
		std::vector<char> error_message(log_length+1);
		glGetShaderInfoLog(fragment_shader_id, log_length, NULL, &error_message[0]);
		printf("%s\n", &error_message[0]);
        return (1);
	}

	_program_id = glCreateProgram();
	glAttachShader(_program_id, vertex_shader_id);
	glAttachShader(_program_id, fragment_shader_id);
	glLinkProgram(_program_id);

	glGetProgramiv(_program_id, GL_LINK_STATUS, &result);
	glGetProgramiv(_program_id, GL_INFO_LOG_LENGTH, &log_length);
	if (log_length > 0)
    {
		std::vector<char> error_message(log_length + 1);
		glGetProgramInfoLog(_program_id, log_length, NULL, &error_message[0]);
		printf("%s\n", &error_message[0]);
        return (1);
	}

	glDetachShader(_program_id, vertex_shader_id);
	glDetachShader(_program_id, fragment_shader_id);
	
	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

    glUseProgram(_program_id);

    return (0);
}


void OpenGL::create_mvp(void)
{
	_projection = glm::perspective(glm::radians(45.0f), _window_ratio, _near_plane, _far_plane);

	_view = glm::lookAt(
				glm::vec3(1,0,3), // Camera is at (4,3,3), in World Space
				glm::vec3(0,0,0), // and looks at the origin
				glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
				);

	_model = glm::mat4(1.0f);
	_mvp = _projection * _view * _model;
	
	_matrix_id = glGetUniformLocation(_program_id, "MVP");
}


void OpenGL::load_mvp(void)
{
	glUniformMatrix4fv(_matrix_id, 1, GL_FALSE, &_mvp[0][0]);
}


void OpenGL::update_mvp(void)
{
	_view = glm::lookAt(_position, _position + _direction, _up);
	_mvp = _projection * _view * _model;
}


void OpenGL::create_triangle(void)
{
    static const GLfloat coords[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
    };
    static const GLfloat colors[] = {
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
    };

    GLuint coordsbuffer;
    glGenBuffers(1, &coordsbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, coordsbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, coordsbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glDisableVertexAttribArray(0);

    GLuint colorsbuffer;
    glGenBuffers(1, &colorsbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorsbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorsbuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glDisableVertexAttribArray(1);
}


void OpenGL::draw(void)
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

void OpenGL::compute_delta_time(void)
{
	static float last_time = glfwGetTime();
    double current_time = glfwGetTime();

    _delta_time = static_cast<float>(current_time - last_time);
    last_time = current_time;
}

void OpenGL::process_inputs(void)
{
    double xpos, ypos;
    glfwGetCursorPos(_window, &xpos, &ypos);
	glfwSetCursorPos(_window, _w_width / 2, _w_height / 2);
	compute_delta_time();

    _horizontal_angle += _mouse_speed * _delta_time * static_cast<float>(_w_width / 2.f - xpos);
    _vertical_angle += _mouse_speed * _delta_time * static_cast<float>(_w_height / 2.f - ypos);


    _direction = glm::vec3(
        cos(_vertical_angle) * sin(_horizontal_angle),
        sin(_vertical_angle),
        cos(_vertical_angle) * cos(_horizontal_angle)
    );
    glm::vec3 right = glm::vec3(
        sin(_horizontal_angle - 3.14f / 2.0f),
        0,
        cos(_horizontal_angle - 3.14f / 2.0f)
    );
    _up = glm::cross( right, _direction );

    if (glfwGetKey(_window, GLFW_KEY_UP) == GLFW_PRESS){
        _position += _direction * _delta_time * _speed;
    }
    if (glfwGetKey(_window, GLFW_KEY_DOWN) == GLFW_PRESS){
        _position -= _direction * _delta_time * _speed;
    }
    if (glfwGetKey(_window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        _position += right * _delta_time * _speed;
    }
    if (glfwGetKey(_window, GLFW_KEY_LEFT) == GLFW_PRESS){
        _position -= right * _delta_time * _speed;
    }
}