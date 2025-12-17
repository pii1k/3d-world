#pragma once

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif

#include "render_data.hpp"
#include <GL/gl.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <string>

class Renderer
{
public:
    ~Renderer();

    bool init(int width, int height, const std::string &title);
    void draw(const RenderQueue &queue, const glm::mat4 &view, const glm::mat4 &projection);
    bool windowShouldClose() const { return should_close_; };

    void swapBuffers();
    void pollEvents();

    GLFWwindow *getWindowPtr() { return window_ptr_; };

private:
    GLuint loadShaders(const std::string &vertex_shader_path, const std::string &fragment_shader_path);
    void createCubeMesh();

    GLFWwindow *window_ptr_ = nullptr;
    bool should_close_ = false; // 윈도 종료 여부
    int width_ = 0;             // 창 너비
    int height_ = 0;            // 창 높이

    GLuint shader_program_ = 0;   // GPU 셰이더 프로그램 핸들
    GLint model_loc_ = -1;        // 모델 행렬 유니폼 위치
    GLint view_loc_ = -1;         // 뷰 행렬 유니폼 위치
    GLint projection_loc_ = -1;   // 프로젝션 행렬 유니폼 위치
    GLuint cube_vao_ = 0;         // 큐브 VAO
    GLuint cube_vbo_ = 0;         // 큐브 VBO
    GLsizei cube_vertex_count_{}; // 큐브 정점 수
};