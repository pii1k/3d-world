#pragma once

#include "render_defs.hpp"
#include "resource_manager.hpp"
#include "shader.hpp"

#include "glad/gl.h"
#include <glm/glm.hpp>
#include <memory>

namespace graphics
{
class Renderer
{
public:
    explicit Renderer(util::ResourceManager &resource_manager);
    ~Renderer();

    bool init();

    void draw(const domain::SceneData &scene);

private:
    std::unique_ptr<graphics::Shader> shader_;
    util::ResourceManager &resource_manager_;

    GLint u_mvp_loc_ = -1;
    GLint u_color_loc_ = -1;
    GLuint cube_vao_ = 0;
    GLuint cube_vbo_ = 0;
    GLuint ui_vao_ = 0;
    GLuint ui_vbo_ = 0;
};

} // namespace graphics