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
    Renderer() = default; // 기본 생성자는 즉시 아무것도 하지 않음
    ~Renderer();          // 소멸 시 OpenGL/윈도 리소스를 정리

    bool init(int width, int height, const std::string &title);                              // GLFW 창+컨텍스트와 셰이더, 메시를 초기화
    void draw(const RenderQueue &queue, const glm::mat4 &view, const glm::mat4 &projection); // RenderQueue의 draw command를 실제로 그리기
    bool windowShouldClose() const { return should_close_; };

    void swapBuffers(); // 프론트/백 버퍼를 교환
    void pollEvents();  // 이벤트를 읽어 창 상태 업데이트

    GLFWwindow *getWindowPtr() { return window_ptr_; };

private:
    GLuint loadShaders(const std::string &vertex_shader_path, const std::string &fragment_shader_path); // 셰이더 파일을 읽고 GPU 프로그램 생성
    void createCubeMesh();                                                                              // 간단한 큐브 VAO/VBO 구성

    GLFWwindow *window_ptr_ = nullptr; // GLFW 윈도우/컨텍스트
    bool should_close_ = false;        // 윈도 종료 여부
    int width_ = 0;                    // 창 너비
    int height_ = 0;                   // 창 높이

    GLuint shader_program_ = 0;   // GPU 셰이더 프로그램 핸들
    GLint model_loc_ = -1;        // 모델 행렬 유니폼 위치
    GLint view_loc_ = -1;         // 뷰 행렬 유니폼 위치
    GLint projection_loc_ = -1;   // 프로젝션 행렬 유니폼 위치
    GLuint cube_vao_ = 0;         // 큐브 VAO
    GLuint cube_vbo_ = 0;         // 큐브 VBO
    GLsizei cube_vertex_count_{}; // 큐브 정점 수
};