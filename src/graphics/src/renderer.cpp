#include "renderer.hpp"
#include "render_data.hpp"
#include "shader.hpp"

#include <GL/glext.h>
#include <algorithm>
#include <array>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace
{
constexpr float kClearColorR = 0.05F;                                                   // 배경색 R (더 어둡게 해 큐브 대비 강조)
constexpr float kClearColorG = 0.05F;                                                   // 배경색 G
constexpr float kClearColorB = 0.08F;                                                   // 배경색 B
constexpr float kClearColorA = 1.0F;                                                    // 배경색 알파
const std::string kVertexShader = std::string(SHADER_ASSET_DIR) + "/shader_vertex";     // 정점 셰이더 경로
const std::string kFragmentShader = std::string(SHADER_ASSET_DIR) + "/shader_fragment"; // 프래그먼트 셰이더 경로
} // namespace

Renderer::~Renderer()
{
    if (shader_program_ != 0)
        glDeleteProgram(shader_program_); // 셰이더 프로그램 제거
    if (cube_vao_ != 0)
        glDeleteVertexArrays(1, &cube_vao_); // 큐브 VAO 제거
    if (cube_vbo_ != 0)
        glDeleteBuffers(1, &cube_vbo_); // 큐브 VBO 제거
    if (window_ptr_)
    {
        glfwDestroyWindow(window_ptr_); // GLFW 창 파괴
        window_ptr_ = nullptr;
    }
    glfwTerminate(); // GLFW 전역 정리
}

bool Renderer::init(int width, int height, const std::string &title)
{
    width_ = width;   // 창 너비 저장
    height_ = height; // 창 높이 저장

    if (!glfwInit()) // GLFW 초기화
    {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ptr_ = glfwCreateWindow(width,
                                   height,
                                   title.c_str(),
                                   nullptr,
                                   nullptr);
    if (!window_ptr_)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window_ptr_); // 현재 스레드에 컨텍스트 바인딩
    glfwSwapInterval(1);                 // vsync 켜기
    std::clog << "[renderer] window created: " << width << "x" << height << " (" << title << ")" << std::endl;

    glViewport(0, 0, width, height); // 렌더링 영역을 창 크기에 맞게 설정

    GLint depth_attachment_type = 0;
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                          GL_DEPTH,
                                          GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                                          &depth_attachment_type);
    std::clog << "[renderer] GL version: " << reinterpret_cast<const char *>(glGetString(GL_VERSION)) << std::endl;

    try
    {
        shader_program_ = loadShaders(kVertexShader, kFragmentShader); // 셰이더 로드 및 컴파일
        std::clog << "[renderer] shaders loaded: " << kVertexShader << ", " << kFragmentShader << std::endl;
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << "\n"; // 셰이더 에러 표시
        return false;                   // 초기화 실패
    }

    createCubeMesh(); // 큐브 메시 준비
    // should_close_ = false; // 초기에는 창이 열려있음
    return true; // 성공
}

void Renderer::draw(const RenderQueue &queue, const glm::mat4 &view, const glm::mat4 &projection)
{
    auto log_error = [](const char *where)
    {
        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            std::clog << "[renderer] gl error at " << where << ": 0x" << std::hex << err << std::dec << std::endl;
        }
    };
    int fb_width = 0;
    int fb_height = 0;
    glfwGetFramebufferSize(window_ptr_, &fb_width, &fb_height);

    glClearColor(kClearColorR, kClearColorG, kClearColorB, kClearColorA); // 배경색 지정
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                   // 컬러/깊이 버퍼 초기화
    glUseProgram(shader_program_);                                        // 셰이더 사용
    log_error("glUseProgram");

    // shader에 행렬 전달
    glUniformMatrix4fv(view_loc_, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projection_loc_, 1, GL_FALSE, glm::value_ptr(projection));
    log_error("set view/projection");

    // -- render queue의 모든 객체 그리기 --
    glBindVertexArray(cube_vao_);
    log_error("bind VAO");

    for (const auto &command : queue)
    {
        if (command.model_id != 0)
            continue; // 현재는 큐브 모델만 렌더링

        // 엔티티의 모델 행렬 전달 후 그리기
        glUniformMatrix4fv(model_loc_, 1, GL_FALSE, glm::value_ptr(command.transform));
        glDrawArrays(GL_TRIANGLES, 0, cube_vertex_count_);
        log_error("glDrawArrays");
    }
    glBindVertexArray(0);
}

void Renderer::swapBuffers()
{
    if (window_ptr_)
        glfwSwapBuffers(window_ptr_);
}

void Renderer::pollEvents()
{
    glfwPollEvents();
    if (window_ptr_ && glfwWindowShouldClose(window_ptr_))
    {
        should_close_ = true;
    }
}

GLuint Renderer::loadShaders(const std::string &vertex_shader_path, const std::string &fragment_shader_path)
{
    const std::string vertex_source = readShaderFile(vertex_shader_path);     // 정점 셰이더 파일 읽기
    const std::string fragment_source = readShaderFile(fragment_shader_path); // 프래그먼트 셰이더 파일 읽기

    GLuint vertex_shader = compileShader(GL_VERTEX_SHADER, vertex_source);       // 정점 셰이더 컴파일
    GLuint fragment_shader = compileShader(GL_FRAGMENT_SHADER, fragment_source); // 프래그먼트 셰이더 컴파일

    GLuint program = glCreateProgram();       // 프로그램 객체 생성
    glAttachShader(program, vertex_shader);   // 정점 셰이더 부착
    glAttachShader(program, fragment_shader); // 프래그먼트 셰이더 부착
    glLinkProgram(program);                   // 프로그램 링크

    GLint linked = 0;                                 // 링크 성공 여부
    glGetProgramiv(program, GL_LINK_STATUS, &linked); // 링크 상태 질의
    if (linked != GL_TRUE)
    {
        char log[1024];                                                       // 로그 버퍼
        glGetProgramInfoLog(program, sizeof(log), nullptr, log);              // 링크 로그 읽기
        glDeleteProgram(program);                                             // 실패 시 프로그램 삭제
        glDeleteShader(vertex_shader);                                        // 셰이더 삭제
        glDeleteShader(fragment_shader);                                      // 셰이더 삭제
        throw std::runtime_error(std::string("Program link failed: ") + log); // 예외 발생
    }

    glDeleteShader(vertex_shader);   // 정점 셰이더 삭제 (링크 후 불필요)
    glDeleteShader(fragment_shader); // 프래그먼트 셰이더 삭제

    model_loc_ = glGetUniformLocation(program, "model");           // 모델 유니폼 위치
    view_loc_ = glGetUniformLocation(program, "view");             // 뷰 유니폼 위치
    projection_loc_ = glGetUniformLocation(program, "projection"); // 프로젝션 유니폼 위치
    if (model_loc_ == -1 || view_loc_ == -1 || projection_loc_ == -1)
    {
        std::clog << "[renderer] warning: uniform location invalid "
                  << "(model=" << model_loc_ << ", view=" << view_loc_ << ", proj=" << projection_loc_ << ")\n";
    }

    return program; // 프로그램 핸들 반환
}

void Renderer::createCubeMesh()
{
    constexpr float cube_vertices[] = {
        -0.5F, -0.5F, -0.5F,
        0.5F, -0.5F, -0.5F,
        0.5F, 0.5F, -0.5F,
        0.5F, 0.5F, -0.5F,
        -0.5F, 0.5F, -0.5F,
        -0.5F, -0.5F, -0.5F,

        -0.5F, -0.5F, 0.5F,
        0.5F, -0.5F, 0.5F,
        0.5F, 0.5F, 0.5F,
        0.5F, 0.5F, 0.5F,
        -0.5F, 0.5F, 0.5F,
        -0.5F, -0.5F, 0.5F,

        -0.5F, 0.5F, 0.5F,
        -0.5F, 0.5F, -0.5F,
        -0.5F, -0.5F, -0.5F,
        -0.5F, -0.5F, -0.5F,
        -0.5F, -0.5F, 0.5F,
        -0.5F, 0.5F, 0.5F,

        0.5F, 0.5F, 0.5F,
        0.5F, 0.5F, -0.5F,
        0.5F, -0.5F, -0.5F,
        0.5F, -0.5F, -0.5F,
        0.5F, -0.5F, 0.5F,
        0.5F, 0.5F, 0.5F,

        -0.5F, -0.5F, -0.5F,
        0.5F, -0.5F, -0.5F,
        0.5F, -0.5F, 0.5F,
        0.5F, -0.5F, 0.5F,
        -0.5F, -0.5F, 0.5F,
        -0.5F, -0.5F, -0.5F,

        -0.5F, 0.5F, -0.5F,
        0.5F, 0.5F, -0.5F,
        0.5F, 0.5F, 0.5F,
        0.5F, 0.5F, 0.5F,
        -0.5F, 0.5F, 0.5F,
        -0.5F, 0.5F, -0.5F}; // 큐브 36개 정점

    cube_vertex_count_ = 36;          // 삼각형 정점 수 저장
    glGenVertexArrays(1, &cube_vao_); // VAO 생성
    glGenBuffers(1, &cube_vbo_);      // VBO 생성

    glBindVertexArray(cube_vao_);                                                        // VAO 바인딩
    glBindBuffer(GL_ARRAY_BUFFER, cube_vbo_);                                            // VBO 바인딩
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW); // 정점 데이터 업로드

    glEnableVertexAttribArray(0);                                                                    // 위치 속성 활성화
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void *>(0)); // 위치 속성 포인터 지정

    glBindVertexArray(0);             // VAO 바인딩 해제
    glBindBuffer(GL_ARRAY_BUFFER, 0); // VBO 해제
    std::clog << "[renderer] cube mesh ready: vao=" << cube_vao_ << " vbo=" << cube_vbo_
              << " vertices=" << cube_vertex_count_ << std::endl;
}