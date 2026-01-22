#include "ui_layer.hpp"

#include <algorithm>
#include <cmath>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <optional>

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>

namespace
{
bool isEntityLikelyAlive(const World &world, entity_id entity)
{
    return world.getComponent<TransformComponent>(entity).has_value() ||
           world.getComponent<RenderableComponent>(entity).has_value() ||
           world.getComponent<LightComponent>(entity).has_value() ||
           world.getComponent<PhysicsComponent>(entity).has_value() ||
           world.getComponent<SelectableComponent>(entity).has_value() ||
           world.getComponent<SelectedComponent>(entity).has_value() ||
           world.getComponent<PickBoundsComponent>(entity).has_value() ||
           world.getComponent<CommNodeComponent>(entity).has_value();
}

bool tryProjectToScreen(const glm::vec3 &world_pos,
                        const glm::mat4 &view,
                        const glm::mat4 &projection,
                        int viewport_w,
                        int viewport_h,
                        float &out_x,
                        float &out_y)
{
    if (viewport_w <= 0 || viewport_h <= 0)
        return false;

    const glm::vec4 clip = projection * view * glm::vec4(world_pos, 1.0f);
    if (!std::isfinite(clip.w) || clip.w <= 1e-6f)
        return false;

    const glm::vec3 ndc = glm::vec3(clip) / clip.w; // [-1, 1]
    if (!std::isfinite(ndc.x) || !std::isfinite(ndc.y) || !std::isfinite(ndc.z))
        return false;

    const float x = (ndc.x * 0.5f + 0.5f) * static_cast<float>(viewport_w);
    const float y = (1.0f - (ndc.y * 0.5f + 0.5f)) * static_cast<float>(viewport_h);
    out_x = x;
    out_y = y;
    return true;
}

void drawCommRangeOverlay(const TransformComponent &transform,
                          const CommNodeComponent &comm_node,
                          const glm::mat4 &view,
                          const glm::mat4 &projection,
                          int viewport_w,
                          int viewport_h)
{
    if (!comm_node.enabled || comm_node.range <= 0.0f)
        return;

    const glm::vec3 center{transform.position.x, 0.0f, transform.position.z};

    constexpr int kSegments = 64;
    constexpr float kTwoPi = 6.28318530718f;
    ImVec2 points[kSegments];

    for (int i = 0; i < kSegments; i++)
    {
        const float t = static_cast<float>(i) / static_cast<float>(kSegments);
        const float a = t * kTwoPi;
        const glm::vec3 p = center + glm::vec3(std::cos(a) * comm_node.range, 0.0f, std::sin(a) * comm_node.range);

        float sx = 0.0f;
        float sy = 0.0f;
        if (!::tryProjectToScreen(p, view, projection, viewport_w, viewport_h, sx, sy))
            return;

        points[i] = ImVec2(sx, sy);
    }

    ImDrawList *dl = ImGui::GetBackgroundDrawList();
    const ImU32 stroke = IM_COL32(0, 220, 255, 190);
    const ImU32 fill = IM_COL32(0, 220, 255, 35);
    dl->AddConvexPolyFilled(points, kSegments, fill);
    dl->AddPolyline(points, kSegments, stroke, ImDrawFlags_Closed, 2.0f);
}

void drawTransform(const TransformComponent &transform)
{
    ImGui::Text("position: (%.3f, %.3f, %.3f)", transform.position.x, transform.position.y, transform.position.z);
    ImGui::Text("scale:    (%.3f, %.3f, %.3f)", transform.scale.x, transform.scale.y, transform.scale.z);
    ImGui::Text("rotation: (w=%.3f, x=%.3f, y=%.3f, z=%.3f)",
                transform.rotation.w, transform.rotation.x, transform.rotation.y, transform.rotation.z);
}

void drawRenderable(const RenderableComponent &renderable)
{
    ImGui::Text("mesh_id:  %d", renderable.mesh_id);
    ImGui::Text("color:    (%.3f, %.3f, %.3f)", renderable.color.x, renderable.color.y, renderable.color.z);
    ImGui::Text("use_grid: %s", renderable.use_grid ? "true" : "false");
}

void drawPhysics(const PhysicsComponent &physics)
{
    ImGui::Text("velocity:      (%.3f, %.3f, %.3f)", physics.velocity.x, physics.velocity.y, physics.velocity.z);
    ImGui::Text("acceleration:  (%.3f, %.3f, %.3f)", physics.acceleration.x, physics.acceleration.y, physics.acceleration.z);
    ImGui::Text("friction:      %.3f", physics.friction);
    ImGui::Text("grounded:      %s", physics.grounded ? "true" : "false");
}

void drawLight(const LightComponent &light)
{
    const char *type = "Directional";
    switch (light.type)
    {
    case LightType::Directional:
        type = "Directional";
        break;
    case LightType::Point:
        type = "Point";
        break;
    case LightType::Spot:
        type = "Spot";
        break;
    }

    ImGui::Text("type:      %s", type);
    ImGui::Text("enabled:   %s", light.enabled ? "true" : "false");
    ImGui::Text("color:     (%.3f, %.3f, %.3f)", light.color.x, light.color.y, light.color.z);
    ImGui::Text("intensity: %.3f", light.intensity);
    ImGui::Text("position:  (%.3f, %.3f, %.3f)", light.position.x, light.position.y, light.position.z);
    ImGui::Text("direction: (%.3f, %.3f, %.3f)", light.direction.x, light.direction.y, light.direction.z);
    ImGui::Text("range:     %.3f", light.range);
    ImGui::Text("cone:      inner=%.3f outer=%.3f", light.inner_cone, light.outer_cone);
}

void drawPickBounds(const PickBoundsComponent &bounds)
{
    ImGui::Text("half_extents:  (%.3f, %.3f, %.3f)", bounds.half_extents.x, bounds.half_extents.y, bounds.half_extents.z);
    ImGui::Text("center_offset: (%.3f, %.3f, %.3f)", bounds.center_offset.x, bounds.center_offset.y, bounds.center_offset.z);
}

void drawCommNode(CommNodeComponent &comm_node)
{
    ImGui::Checkbox("enabled", &comm_node.enabled);
    ImGui::Text("range:   %.3f", comm_node.range);
}

void drawEntityInspector(World &world, entity_id entity)
{
    ImGui::Text("Entity: %u", entity);

    if (world.getComponent<SelectedComponent>(entity).has_value())
        ImGui::TextUnformatted("Selected: yes");
    else
        ImGui::TextUnformatted("Selected: no");

    if (auto t = world.getComponent<TransformComponent>(entity))
    {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
            drawTransform(t->get());
    }

    if (auto r = world.getComponent<RenderableComponent>(entity))
    {
        if (ImGui::CollapsingHeader("Renderable", ImGuiTreeNodeFlags_DefaultOpen))
            drawRenderable(r->get());
    }

    if (auto p = world.getComponent<PhysicsComponent>(entity))
    {
        if (ImGui::CollapsingHeader("Physics"))
            drawPhysics(p->get());
    }

    if (auto l = world.getComponent<LightComponent>(entity))
    {
        if (ImGui::CollapsingHeader("Light"))
            drawLight(l->get());
    }

    if (auto b = world.getComponent<PickBoundsComponent>(entity))
    {
        if (ImGui::CollapsingHeader("PickBounds"))
            drawPickBounds(b->get());
    }

    if (auto c = world.getComponent<CommNodeComponent>(entity))
    {
        if (ImGui::CollapsingHeader("CommNode"))
            drawCommNode(c->get());
    }
}

} // namespace

UiLayer::~UiLayer()
{
    if (!initialized_)
        return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    initialized_ = false;
}

bool UiLayer::init(GLFWwindow *window, const char *glsl_version)
{
    window_ = window;
    if (!window_)
        return false;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();

    if (!ImGui_ImplGlfw_InitForOpenGL(window_, false))
        return false;
    if (!ImGui_ImplOpenGL3_Init(glsl_version))
        return false;

    initialized_ = true;
    return true;
}

void UiLayer::onKey(int key, int scancode, int action, int mods)
{
    if (!initialized_)
        return;
    ImGui_ImplGlfw_KeyCallback(window_, key, scancode, action, mods);
}

void UiLayer::onChar(unsigned int c)
{
    if (!initialized_)
        return;
    ImGui_ImplGlfw_CharCallback(window_, c);
}

void UiLayer::onMouseButton(int button, int action, int mods)
{
    if (!initialized_)
        return;
    ImGui_ImplGlfw_MouseButtonCallback(window_, button, action, mods);
}

void UiLayer::onCursorPos(double pos_x, double pos_y)
{
    if (!initialized_)
        return;
    ImGui_ImplGlfw_CursorPosCallback(window_, pos_x, pos_y);
}

void UiLayer::onScroll(double offset_x, double offset_y)
{
    if (!initialized_)
        return;
    ImGui_ImplGlfw_ScrollCallback(window_, offset_x, offset_y);
}

void UiLayer::beginFrame()
{
    if (!initialized_)
        return;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UiLayer::endFrame()
{
    if (!initialized_)
        return;

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool UiLayer::wantsCaptureMouse() const
{
    if (!initialized_)
        return false;
    return ImGui::GetIO().WantCaptureMouse;
}

bool UiLayer::wantsCaptureKeyboard() const
{
    if (!initialized_)
        return false;
    return ImGui::GetIO().WantCaptureKeyboard;
}

void UiLayer::openInspector(entity_id entity)
{
    if (!initialized_)
        return;

    auto it = inspector_states_.find(entity);
    if (it == inspector_states_.end())
    {
        inspector_states_.try_emplace(entity, InspectorWindowState{});
        open_inspectors_.push_back(entity);
        return;
    }

    it->second.open = true;
    auto pos = std::find(open_inspectors_.begin(), open_inspectors_.end(), entity);
    if (pos == open_inspectors_.end())
        open_inspectors_.push_back(entity);
}

void UiLayer::drawInspectors(World &world,
                             const glm::mat4 &view,
                             const glm::mat4 &projection,
                             int viewport_w,
                             int viewport_h)
{
    if (!initialized_)
        return;

    // draw UI
    open_inspectors_.erase(std::remove_if(open_inspectors_.begin(), open_inspectors_.end(), [&](entity_id entity)
                                          {
                                            auto it = inspector_states_.find(entity);
                                            if (it == inspector_states_.end())
                                                return true;

                                            if (!it->second.open)
                                            {
                                                inspector_states_.erase(it);
                                                return true;
                                            }

                                            if (!::isEntityLikelyAlive(world, entity))
                                            {
                                                it->second.open = false;
                                                inspector_states_.erase(it);
                                                return true;
                                            }

                                            return false; }),
                           open_inspectors_.end());

    // draw comm range overlays
    for (entity_id entity : open_inspectors_)
    {
        auto t = world.getComponent<TransformComponent>(entity);
        auto c = world.getComponent<CommNodeComponent>(entity);
        if (t && c)
            ::drawCommRangeOverlay(t->get(), c->get(), view, projection, viewport_w, viewport_h);
    }

    for (entity_id entity : open_inspectors_)
    {
        auto it = inspector_states_.find(entity);
        if (it == inspector_states_.end())
            continue;

        std::string title = "Entity " + std::to_string(entity) + "##inspector_" + std::to_string(entity);

        if (ImGui::Begin(title.c_str(), &it->second.open, ImGuiWindowFlags_AlwaysAutoResize))
        {
            drawEntityInspector(world, entity);
        }
        ImGui::End();
    }
}