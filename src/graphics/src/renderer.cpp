#include "renderer.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace graphics
{
namespace
{
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

const float kUIPanelVertices[] = {
    // triangle 1
    -0.95f, 0.95f, 0.0f, -0.55f, 0.95f, 0.0f, -0.55f, 0.65f, 0.0f,
    // triangle 2
    -0.95f, 0.95f, 0.0f, -0.55f, 0.65f, 0.0f, -0.95f, 0.65f, 0.0f};

const glm::vec3 kUIPanelColor{0.10f, 0.12f, 0.16f};

const std::string kShaderDirPath = "assets/shader/";

} // namespace

Renderer::Renderer() = default;

Renderer::~Renderer()
{
    if (ui_vbo_)
        glDeleteBuffers(1, &ui_vbo_);
    if (ui_vao_)
        glDeleteVertexArrays(1, &ui_vao_);
    if (cube_vbo_)
        glDeleteBuffers(1, &cube_vbo_);
    if (cube_vao_)
        glDeleteVertexArrays(1, &cube_vao_);
}

bool Renderer::init()
{
    shader_ = std::make_unique<graphics::Shader>(kShaderDirPath + "snake.vs", kShaderDirPath + "snake.fs");

    u_mvp_loc_ = glGetUniformLocation(shader_->getId(), "uMVP");
    u_color_loc_ = glGetUniformLocation(shader_->getId(), "uColor");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    glGenVertexArrays(1, &cube_vao_);
    glGenBuffers(1, &cube_vbo_);
    glBindVertexArray(cube_vao_);
    glBindBuffer(GL_ARRAY_BUFFER, cube_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kCubeVertices), kCubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    glGenVertexArrays(1, &ui_vao_);
    glGenBuffers(1, &ui_vbo_);
    glBindVertexArray(ui_vao_);
    glBindBuffer(GL_ARRAY_BUFFER, ui_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kUIPanelVertices), kUIPanelVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    glBindVertexArray(0);
    return true;
}

void Renderer::draw(const domain::SceneData &scene)
{
    if (!shader_)
        return;

    shader_->use();

    for (const auto &obj : scene.objects)
    {
        if (obj.mesh_type == domain::MeshType::Cube)
        {
            const glm::mat4 mvp = scene.proj * scene.view * obj.transform;
            glUniform3f(u_color_loc_, obj.color.r, obj.color.g, obj.color.b);
            glUniformMatrix4fv(u_mvp_loc_, 1, GL_FALSE, glm::value_ptr(mvp));
            glBindVertexArray(cube_vao_);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    if (scene.show_ui)
    {
        glDisable(GL_DEPTH_TEST);
        glUniform3f(u_color_loc_, kUIPanelColor.r, kUIPanelColor.g, kUIPanelColor.b);
        glUniformMatrix4fv(u_mvp_loc_, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
        glBindVertexArray(ui_vao_);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glEnable(GL_DEPTH_TEST);
    }

    glBindVertexArray(0);
}

} // namespace graphics