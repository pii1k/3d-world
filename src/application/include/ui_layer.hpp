#pragma once

#include "component.hpp"
#include "world.hpp"

#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>

struct GLFWwindow;

class UiLayer
{
public:
    struct InspectorWindowState
    {
        bool open = true;
        bool pin_to_entity = false;
        bool initial_pos_set = false;
    };

    UiLayer() = default;
    ~UiLayer();

    UiLayer(const UiLayer &) = delete;
    UiLayer &operator=(const UiLayer &) = delete;

    bool init(GLFWwindow *window, const char *glsl_version);

    void onKey(int key, int scancode, int action, int mods);
    void onChar(unsigned int c);
    void onMouseButton(int button, int action, int mods);
    void onCursorPos(double pos_x, double pos_y);
    void onScroll(double offset_x, double offset_y);

    void beginFrame();
    void endFrame();

    bool wantsCaptureMouse() const;
    bool wantsCaptureKeyboard() const;

    void openInspector(entity_id entity);
    void drawInspectors(const World &world,
                        const glm::mat4 &view,
                        const glm::mat4 &projection,
                        int viewport_w,
                        int viewport_h);

private:
    GLFWwindow *window_ = nullptr;
    bool initialized_ = false;

    std::vector<entity_id> open_inspectors_;
    std::unordered_map<entity_id, InspectorWindowState> inspector_states_;
};