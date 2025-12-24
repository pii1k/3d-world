#pragma once

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif

#include "mesh.hpp"
#include "render_data.hpp"
#include <GL/gl.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <memory>
#include <string>
#include <vector>

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
    void registerBuiltinMeshes();
    int registerMesh(std::unique_ptr<Mesh> mesh, int preferred_id = -1);
    Mesh *getMeshFromId(int mesh_id);

    GLFWwindow *window_ptr_ = nullptr;
    bool should_close_ = false;
    int width_ = 0;
    int height_ = 0;

    GLuint shader_program_ = 0;
    GLint model_loc_ = -1;
    GLint view_loc_ = -1;
    GLint projection_loc_ = -1;
    GLint color_loc_ = -1;
    GLint use_grid_loc_ = -1;

    std::vector<std::unique_ptr<Mesh>> meshes_;
};