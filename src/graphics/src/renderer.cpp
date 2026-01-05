#include "renderer.hpp"
#include "primitives.hpp"
#include "render_data.hpp"
#include "shader.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace
{
constexpr float kClearColorR = 0.05F;
constexpr float kClearColorG = 0.05F;
constexpr float kClearColorB = 0.08F;
constexpr float kClearColorA = 1.0F;
const std::string kVertexShader = std::string(SHADER_ASSET_DIR) + "/shader_vertex";
const std::string kFragmentShader = std::string(SHADER_ASSET_DIR) + "/shader_fragment";
} // namespace

Renderer::~Renderer()
{
    if (shader_program_ != 0)
        glDeleteProgram(shader_program_);
    if (window_ptr_)
    {
        glfwDestroyWindow(window_ptr_);
        window_ptr_ = nullptr;
    }
    glfwTerminate();
}

bool Renderer::init(int width, int height, const std::string &title)
{
    width_ = width;
    height_ = height;

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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
    glfwMakeContextCurrent(window_ptr_);
    glfwSwapInterval(1);
    std::clog << "[renderer] window created: " << width << "x" << height << " (" << title << ")" << std::endl;

    int framebuffer_width = width;
    int framebuffer_height = height;
    glfwGetFramebufferSize(window_ptr_, &framebuffer_width, &framebuffer_height);
    glViewport(0, 0, framebuffer_width, framebuffer_height);
    glEnable(GL_DEPTH_TEST);

    std::clog << "[renderer] GL version: " << reinterpret_cast<const char *>(glGetString(GL_VERSION)) << std::endl;

    try
    {
        shader_program_ = loadShaders(kVertexShader, kFragmentShader);
        std::clog << "[renderer] shaders loaded: " << kVertexShader << ", " << kFragmentShader << std::endl;
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << "\n";
        return false;
    }

    registerBuiltinMeshes();
    return true;
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

    glClearColor(kClearColorR, kClearColorG, kClearColorB, kClearColorA);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader_program_);
    log_error("glUseProgram");

    glUniformMatrix4fv(view_loc_, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projection_loc_, 1, GL_FALSE, glm::value_ptr(projection));
    log_error("set view/projection");

    auto draw_item = [&](const RenderItem &item)
    {
        Mesh *mesh = getMeshFromId(static_cast<int>(item.mesh_handle));
        if (!mesh)
            return;

        glUniformMatrix4fv(model_loc_, 1, GL_FALSE, glm::value_ptr(item.model));
        glUniform3f(color_loc_, item.color.x, item.color.y, item.color.z);
        glUniform1i(use_grid_loc_, item.use_grid ? 1 : 0);

        glBindVertexArray(mesh->getVAO());
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->getIndexCount()), GL_UNSIGNED_INT, nullptr);
        log_error("glDrawElements");
    };

    for (const auto &item : queue.opaque)
    {
        draw_item(item);
    }
    for (const auto &item : queue.transparent)
    {
        draw_item(item);
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
    const std::string vertex_source = readShaderFile(vertex_shader_path);
    const std::string fragment_source = readShaderFile(fragment_shader_path);

    GLuint vertex_shader = compileShader(GL_VERTEX_SHADER, vertex_source);
    GLuint fragment_shader = compileShader(GL_FRAGMENT_SHADER, fragment_source);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    GLint linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked != GL_TRUE)
    {
        char log[1024];
        glGetProgramInfoLog(program, sizeof(log), nullptr, log);
        glDeleteProgram(program);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        throw std::runtime_error(std::string("Program link failed: ") + log);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    model_loc_ = glGetUniformLocation(program, "model");
    view_loc_ = glGetUniformLocation(program, "view");
    projection_loc_ = glGetUniformLocation(program, "projection");
    color_loc_ = glGetUniformLocation(program, "uColor");
    use_grid_loc_ = glGetUniformLocation(program, "uUseGrid");
    if (model_loc_ == -1 || view_loc_ == -1 || projection_loc_ == -1 || color_loc_ == -1 || use_grid_loc_ == -1)
    {
        std::clog << "[renderer] warning: uniform location invalid "
                  << "(model=" << model_loc_ << ", view=" << view_loc_ << ", proj=" << projection_loc_ << ", color=" << color_loc_
                  << ", grid=" << use_grid_loc_ << ")\n";
    }

    return program;
}

void Renderer::registerBuiltinMeshes()
{
    registerMesh(Primitives::createCube(), static_cast<int>(MeshId::Cube));
    registerMesh(Primitives::createPlane(1.0f, 1.0f), static_cast<int>(MeshId::Plane));
    std::clog << "[renderer] builtin meshes registered (cube=" << static_cast<int>(MeshId::Cube)
              << ", plane=" << static_cast<int>(MeshId::Plane) << ")\n";
}

int Renderer::registerMesh(std::unique_ptr<Mesh> mesh, int preferred_id)
{
    if (!mesh)
        return -1;

    // TODO(jyan): createCube, cretePlane에 맞춰서 작성된거라 나중에 수정 필요
    if (preferred_id >= 0)
    {
        if (static_cast<size_t>(preferred_id) >= meshes_.size())
            meshes_.resize(static_cast<size_t>(preferred_id) + 1);
        meshes_[static_cast<size_t>(preferred_id)] = std::move(mesh);
        return preferred_id;
    }

    meshes_.push_back(std::move(mesh));
    return static_cast<int>(meshes_.size() - 1);
}

Mesh *Renderer::getMeshFromId(int mesh_id)
{
    if (mesh_id < 0)
        return nullptr;
    const size_t idx = static_cast<size_t>(mesh_id);
    if (idx >= meshes_.size())
        return nullptr;
    return meshes_[idx].get();
}
