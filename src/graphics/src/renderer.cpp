#include "renderer.hpp"
#include "render_data.hpp"
#include "shader.hpp"

#include <GL/glext.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace
{
constexpr float kClearColorR = 0.1F;                                                    // 배경색 R
constexpr float kClearColorG = 0.1F;                                                    // 배경색 G
constexpr float kClearColorB = 0.12F;                                                   // 배경색 B
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
    if (window_)
    {
        glfwDestroyWindow(window_); // GLFW 창 파괴
        window_ = nullptr;
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

    window_ = glfwCreateWindow(width,
                               height,
                               title.c_str(),
                               nullptr,
                               nullptr);
    if (!window_)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window_); // 현재 스레드에 컨텍스트 바인딩
    glfwSwapInterval(1);             // vsync 켜기

    glViewport(0, 0, width, height); // 렌더링 영역을 창 크기에 맞게 설정
    glEnable(GL_DEPTH_TEST);         // 깊이 테스트 활성화

    try
    {
        shader_program_ = loadShaders(kVertexShader, kFragmentShader); // 셰이더 로드 및 컴파일
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

void Renderer::draw(const RenderQueue &queue)
{
    if (shader_program_ == 0 || cube_vao_ == 0)
        return;                                                           // 초기화 실패 시 그리지 않음
    glClearColor(kClearColorR, kClearColorG, kClearColorB, kClearColorA); // 배경색 지정
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                   // 컬러/깊이 버퍼 초기화
    glUseProgram(shader_program_);                                        // 셰이더 사용

    // glm을 사용한 카메라 행렬 설정
    // view matrix: 카메라를 (0,0,3) 위치에 두고 원점을 바라보게함
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f));

    // projection matrix: 45도 시야각(FOV)의 원근 투영 설정 (실제 창 비율 사용)
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width_) / static_cast<float>(height_), 0.1f, 100.0f);

    // shader에 행렬 전달
    glUniformMatrix4fv(glGetUniformLocation(shader_program_, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader_program_, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // -- render queue의 모든 객체 그리기 --
    glBindVertexArray(cube_vao_);
    for (const auto &command : queue)
    {
        if (command.model_id != 0)
            continue; // 현재는 큐브 모델만 렌더링

        // 엔티티의 모델 행렬 전달 후 그리기
        glUniformMatrix4fv(glGetUniformLocation(shader_program_, "model"), 1, GL_FALSE, glm::value_ptr(command.transform));
        glDrawArrays(GL_TRIANGLES, 0, cube_vertex_count_);
    }
    glBindVertexArray(0);
}

void Renderer::swapBuffers()
{
    if (window_)
        glfwSwapBuffers(window_);
}

void Renderer::pollEvents()
{
    glfwPollEvents();
    if (window_ && glfwWindowShouldClose(window_))
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
}