#include "glad/gl.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/trigonometric.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "shader.hpp"

#include <algorithm>
#include <iostream>
#include <string>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace
{
constexpr int kGridW = 24;
constexpr int kGridH = 18;
constexpr int kWindowW = 960;
constexpr int kWindowH = 720;
constexpr glm::mat4 kIMat = glm::mat4(1.0f);

int g_framebuffer_w = kWindowW;
int g_framebuffer_h = kWindowH;
void framebuffer_size_callback(GLFWwindow * /* window */, int width, int height)
{
    g_framebuffer_w = width;
    g_framebuffer_h = height;
    glViewport(0, 0, width, height);
}

const float kCubeVertices[] = {
    // front (z = +0.5)
    -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
    // back (z = -0.5)
    -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f,
    // left (x = -0.5)
    -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f,
    // right (x = +0.5)
    0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f,
    // top (y = +0.5)
    -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f,
    // bottom (y = -0.5)
    -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f};

} // namespace

GLFWwindow *init()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow *window = glfwCreateWindow(kWindowW, kWindowH, "3d Snake (GLFW/OpenGL)", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwGetFramebufferSize(window, &g_framebuffer_w, &g_framebuffer_h);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        glfwTerminate();
        return nullptr;
    }

    glEnable(GL_MULTISAMPLE);

    return window;
}

struct CameraConfig
{
    glm::vec3 position{};
    glm::vec3 target{};
    glm::vec3 up{};
    float fov_deg = 45.0f;
    float near_plane = 0.1f;
    float far_plane = 200.0f;
    bool use_ortho = false;
    float ortho_size = 10.0f;
};

class Camera
{
public:
    explicit Camera(const CameraConfig &cfg)
        : position_(cfg.position),
          target_(cfg.target),
          up_(cfg.up),
          fov_deg_(cfg.fov_deg),
          near_plane_(cfg.near_plane),
          far_plane_(cfg.far_plane),
          use_ortho_(cfg.use_ortho),
          ortho_size_(cfg.ortho_size) {}

    glm::mat4 view() const { return glm::lookAt(position_, target_, up_); }
    glm::mat4 proj(int w, int h) const
    {
        const float aspect_ratio = static_cast<float>(w) / static_cast<float>(std::max(1, h));
        return use_ortho_ ? glm::ortho(-ortho_size_ * aspect_ratio, ortho_size_ * aspect_ratio,
                                       -ortho_size_, ortho_size_, near_plane_, far_plane_)
                          : glm::perspective(glm::radians(fov_deg_), aspect_ratio, near_plane_, far_plane_);
    }

    void setPosition(const glm::vec3 &pos) { position_ = pos; }
    void setTarget(const glm::vec3 &target) { target_ = target; }
    void setUp(const glm::vec3 &up) { up_ = up; }
    void setPerspective(float fov_deg, float near_plane, float far_plane)
    {
        use_ortho_ = false;
        fov_deg_ = fov_deg;
        near_plane_ = near_plane;
        far_plane_ = far_plane;
    }
    void setOrtho(float size, float near_plane, float far_plane)
    {
        use_ortho_ = true;
        ortho_size_ = size;
        near_plane_ = near_plane;
        far_plane_ = far_plane;
    }

private:
    glm::vec3 position_{};
    glm::vec3 target_{};
    glm::vec3 up_{};
    float fov_deg_ = 0.f;
    float near_plane_, far_plane_ = 0.f;
    bool use_ortho_ = false;
    float ortho_size_ = 0.f;
};

struct OrbitController
{
public:
    explicit OrbitController(Camera camera)
        : camera_(std::move(camera)) {}

    Camera &getCamera() { return camera_; }
    const Camera &getCamera() const { return camera_; }

    void update()
    {
        const float yaw = glm::radians(yaw_deg_);
        const float pitch = glm::radians(pitch_deg_);

        const float cosin_pitch = glm::cos(pitch);
        const glm::vec3 direction{cosin_pitch * glm::sin(yaw),
                                  glm::sin(pitch),
                                  cosin_pitch * glm::cos(yaw)};

        camera_.setTarget(target_);
        camera_.setPosition(target_ + direction * distance_);
        camera_.setUp(glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void onScroll(double yoffset)
    {
        distance_ = std::clamp(distance_ - static_cast<float>(yoffset) * zoom_speed_,
                               min_distance_,
                               max_distance_);
        this->update();
    }

private:
    Camera camera_;
    glm::vec3 target_{0.0f, 0.0f, 0.0f};
    float yaw_deg_ = 0.0f;
    float pitch_deg_ = 35.0f;
    float distance_ = 50.0f;
    float zoom_speed_ = 1.0f;
    float min_distance_ = 2.0f;
    float max_distance_ = 200.0f;
};

void scroll_callback(GLFWwindow *window, double /* xoffset */, double yoffset)
{
    auto *controller = static_cast<OrbitController *>(glfwGetWindowUserPointer(window));
    if (!controller)
        return;

    controller->onScroll(yoffset);
}

int main()
{
    GLFWwindow *window = init();
    if (!window)
        return -1;

    CameraConfig cam_cfg{};
    cam_cfg.position = glm::vec3(0.0f, 2.0f, 12.0f);
    cam_cfg.target = glm::vec3(0.0f, 0.0f, 0.0f);
    cam_cfg.up = glm::vec3(0.0f, 1.0f, 0.0f);
    cam_cfg.fov_deg = 45.0f;
    cam_cfg.near_plane = 0.1f;
    cam_cfg.far_plane = 200.0f;

    OrbitController controller{Camera(cam_cfg)};
    controller.update(); // initialize
    glfwSetWindowUserPointer(window, &controller);
    glfwSetScrollCallback(window, scroll_callback);

    const std::string shader_dir = std::string(ASSETS_DIR) + "/shader/";
    Shader shader(shader_dir + "snake.vs", shader_dir + "snake.fs");
    shader.use();

    const GLint u_mvp_loc = glGetUniformLocation(shader.getId(), "uMVP");
    const GLint u_color_loc = glGetUniformLocation(shader.getId(), "uColor");

    glEnable(GL_DEPTH_TEST);
    glUniform3f(u_color_loc, 0.75f, 0.75f, 0.85f);

    // set plane VAO, VBO
    GLuint plane_vao, plane_vbo = 0;
    glGenVertexArrays(1, &plane_vao);
    glGenBuffers(1, &plane_vbo);
    glBindVertexArray(plane_vao);
    glBindBuffer(GL_ARRAY_BUFFER, plane_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kCubeVertices), kCubeVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glBindVertexArray(0);

    // loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glViewport(0, 0, g_framebuffer_w, g_framebuffer_h);
        glClearColor(0.06f, 0.07f, 0.09f, 3.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        constexpr float kGroundThickness = 1.0f;
        const float ground_width = static_cast<float>(kGridW);
        const float ground_length = static_cast<float>(kGridH) * 2.0f;

        const glm::mat4 model = glm::translate(kIMat, glm::vec3(0.0f, -kGroundThickness * 0.5f, 0.0f)) *
                                glm::scale(kIMat, glm::vec3(ground_width, kGroundThickness, ground_length));

        auto &camera = controller.getCamera();
        const glm::mat4 view = camera.view();
        const glm::mat4 proj = camera.proj(g_framebuffer_w, g_framebuffer_h);
        const glm::mat4 mvp = proj * view * model;
        glUniformMatrix4fv(u_mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp));

        glBindVertexArray(plane_vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteBuffers(1, &plane_vbo);
    glDeleteVertexArrays(1, &plane_vao);

    glfwTerminate();
    return 0;
}