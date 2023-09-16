cmake_minimum_required(VERSION 3.0)


project(BlackHoleSimulator)


add_executable(${PROJECT_NAME}
    sources/main.cpp
    sources/class_opengl.cpp
    sources/class_display.cpp
    sources/class_opencl.cpp)


include_directories(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/includes)
include_directories(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/librairies/glm)


find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

find_package(GLEW REQUIRED)

set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)

find_package(OpenCL REQUIRED)


target_link_libraries(${PROJECT_NAME} PRIVATE ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_opengl_LIBRARY} OpenCL::OpenCL)