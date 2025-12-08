#include "gl_render_adapter.hpp"

#include <algorithm>
#include <array>
#include <cmath>

#include "pedestrian.hpp"
#include "vehicle.hpp"
#include "world.hpp"

namespace
{
constexpr float kPi = 3.1415926535F;
constexpr float kDegToRad = kPi / 180.0F;
constexpr float kGroundSize = 200.0F;
constexpr float kGridStep = 5.0F;

ads::adapter::Vec3 operator+(const ads::adapter::Vec3 &lhs, const ads::adapter::Vec3 &rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

ads::adapter::Vec3 operator-(const ads::adapter::Vec3 &lhs, const ads::adapter::Vec3 &rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

ads::adapter::Vec3 operator*(const ads::adapter::Vec3 &vec, float scalar)
{
    return {vec.x * scalar, vec.y * scalar, vec.z * scalar};
}

float dot(const ads::adapter::Vec3 &lhs, const ads::adapter::Vec3 &rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

ads::adapter::Vec3 cross(const ads::adapter::Vec3 &lhs, const ads::adapter::Vec3 &rhs)
{
    return {
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x};
}

ads::adapter::Vec3 normalize(const ads::adapter::Vec3 &vec)
{
    const float len = std::sqrt(dot(vec, vec));
    if (len <= 0.0001F)
    {
        return {0.0F, 0.0F, 0.0F};
    }
    return {vec.x / len, vec.y / len, vec.z / len};
}
} // namespace

namespace ads::adapter
{
GlRenderAdapter::GlRenderAdapter(unsigned int width, unsigned int height)
    : width_(width), height_(height)
{
}

void GlRenderAdapter::ensureWindow()
{
    if (window_)
    {
        return;
    }

    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.antialiasingLevel = 4;

    window_ = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(width_, height_),
        "Autonomous Driving Simulator",
        sf::Style::Default,
        settings);
    window_->setVerticalSyncEnabled(true);
    window_->setActive(true);
    window_->setMouseCursorVisible(true);
    last_mouse_position_ = sf::Mouse::getPosition(*window_);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.05F, 0.05F, 0.08F, 1.0F);
    setupProjection();
    frame_clock_.restart();
}

void GlRenderAdapter::processEvents()
{
    sf::Event event{};
    while (window_->pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            window_active_ = false;
            window_->close();
            break;
        case sf::Event::Resized:
            width_ = event.size.width;
            height_ = event.size.height;
            glViewport(0, 0, static_cast<GLsizei>(width_), static_cast<GLsizei>(height_));
            setupProjection();
            break;
        case sf::Event::MouseWheelScrolled:
            camera_distance_ -= event.mouseWheelScroll.delta * 2.5F;
            camera_distance_ = std::clamp(camera_distance_, 5.0F, 150.0F);
            break;
        case sf::Event::MouseButtonPressed:
            last_mouse_position_ = {event.mouseButton.x, event.mouseButton.y};
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                orbiting_ = true;
            }
            if (event.mouseButton.button == sf::Mouse::Middle)
            {
                panning_ = true;
            }
            break;
        case sf::Event::MouseButtonReleased:
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                orbiting_ = false;
            }
            if (event.mouseButton.button == sf::Mouse::Middle)
            {
                panning_ = false;
            }
            break;
        case sf::Event::MouseMoved:
        {
            const sf::Vector2i current(event.mouseMove.x, event.mouseMove.y);
            const sf::Vector2i delta = current - last_mouse_position_;
            last_mouse_position_ = current;
            if (orbiting_)
            {
                camera_yaw_deg_ -= static_cast<float>(delta.x) * 0.25F;
                camera_pitch_deg_ -= static_cast<float>(delta.y) * 0.25F;
                camera_pitch_deg_ = std::clamp(camera_pitch_deg_, -5.0F, 85.0F);
            }
            else if (panning_)
            {
                const float pan_scale = camera_distance_ * 0.005F;
                camera_target_offset_.x -= static_cast<float>(delta.x) * pan_scale;
                camera_target_offset_.z += static_cast<float>(delta.y) * pan_scale;
            }
            break;
        }
        default:
            break;
        }
    }
}

void GlRenderAdapter::setupProjection()
{
    glViewport(0, 0, static_cast<GLsizei>(width_), static_cast<GLsizei>(height_));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    const float aspect = static_cast<float>(width_) / static_cast<float>(std::max(1u, height_));
    const float fov = 45.0F * kDegToRad;
    const float near_plane = 0.1F;
    const float far_plane = 500.0F;
    const float top = std::tan(fov * 0.5F) * near_plane;
    const float bottom = -top;
    const float right = top * aspect;
    const float left = -right;
    glFrustum(left, right, bottom, top, near_plane, far_plane);
    glMatrixMode(GL_MODELVIEW);
}

void GlRenderAdapter::updateCameraTarget(const domain::World &world, float delta_seconds)
{
    if (delta_seconds <= 0.0F)
    {
        delta_seconds = 0.016F;
    }

    const float yaw_rad = camera_yaw_deg_ * kDegToRad;
    Vec3 forward{std::cos(yaw_rad), 0.0F, std::sin(yaw_rad)};
    Vec3 right{-forward.z, 0.0F, forward.x};
    Vec3 movement{};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        movement = movement + forward;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        movement = movement - forward;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        movement = movement + right;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        movement = movement - right;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
        camera_target_offset_.y -= keyboard_pan_speed_ * delta_seconds * 0.5F;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        camera_target_offset_.y += keyboard_pan_speed_ * delta_seconds * 0.5F;
    }

    if (movement.x != 0.0F || movement.y != 0.0F || movement.z != 0.0F)
    {
        movement = normalize(movement);
        camera_target_offset_.x += movement.x * keyboard_pan_speed_ * delta_seconds;
        camera_target_offset_.z += movement.z * keyboard_pan_speed_ * delta_seconds;
    }

    (void)world;
}

Vec3 GlRenderAdapter::computeFocusPoint(const domain::World &world) const
{
    const auto &ego = world.egoVehicle();
    const auto pos = ego.position();
    Vec3 focus{pos.x, 1.5F, pos.y};
    return focus + camera_target_offset_;
}

Vec3 GlRenderAdapter::computeCameraPosition(const Vec3 &focus) const
{
    const float yaw_rad = camera_yaw_deg_ * kDegToRad;
    const float pitch_rad = camera_pitch_deg_ * kDegToRad;
    const float cos_pitch = std::cos(pitch_rad);
    Vec3 offset{
        cos_pitch * std::cos(yaw_rad),
        std::sin(pitch_rad),
        cos_pitch * std::sin(yaw_rad)};
    offset = offset * camera_distance_;
    return focus + offset;
}

std::array<float, 16> GlRenderAdapter::makeLookAtMatrix(const Vec3 &eye, const Vec3 &target)
{
    const Vec3 up{0.0F, 1.0F, 0.0F};
    const Vec3 f = normalize(target - eye);
    const Vec3 s = normalize(cross(f, up));
    const Vec3 u = cross(s, f);

    return {s.x,
            u.x,
            -f.x,
            0.0F,
            s.y,
            u.y,
            -f.y,
            0.0F,
            s.z,
            u.z,
            -f.z,
            0.0F,
            -dot(s, eye),
            -dot(u, eye),
            dot(f, eye),
            1.0F};
}

void GlRenderAdapter::applyViewMatrix(const Vec3 &eye, const Vec3 &target)
{
    const auto matrix = makeLookAtMatrix(eye, target);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(matrix.data());
}

void GlRenderAdapter::drawScene(const domain::World &world)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawGround();
    drawRoadSurface(world.roadHalfWidth());
    drawVehicle(world.egoVehicle());
    drawPedestrians(world.pedestrians());
}

void GlRenderAdapter::render(const domain::World &world)
{
    ensureWindow();
    if (!window_active_)
    {
        return;
    }

    const float delta_seconds = frame_clock_.restart().asSeconds();
    processEvents();
    if (!window_active_)
    {
        return;
    }

    updateCameraTarget(world, delta_seconds);
    const Vec3 focus = computeFocusPoint(world);
    const Vec3 camera_position = computeCameraPosition(focus);

    window_->setActive(true);
    setupProjection();
    applyViewMatrix(camera_position, focus);
    drawScene(world);
    window_->display();
}

void GlRenderAdapter::drawGround() const
{
    glDisable(GL_CULL_FACE);
    glColor3f(0.08F, 0.10F, 0.12F);
    glBegin(GL_QUADS);
    glVertex3f(-kGroundSize, -0.05F, -kGroundSize);
    glVertex3f(kGroundSize, -0.05F, -kGroundSize);
    glVertex3f(kGroundSize, -0.05F, kGroundSize);
    glVertex3f(-kGroundSize, -0.05F, kGroundSize);
    glEnd();

    glColor3f(0.18F, 0.18F, 0.2F);
    glBegin(GL_LINES);
    for (float i = -kGroundSize; i <= kGroundSize; i += kGridStep)
    {
        glVertex3f(i, 0.0F, -kGroundSize);
        glVertex3f(i, 0.0F, kGroundSize);
        glVertex3f(-kGroundSize, 0.0F, i);
        glVertex3f(kGroundSize, 0.0F, i);
    }
    glEnd();
}

void GlRenderAdapter::drawRoadSurface(float half_width) const
{
    const float length = 300.0F;
    glColor3f(0.1F, 0.1F, 0.12F);
    glBegin(GL_QUADS);
    glVertex3f(-length, 0.01F, -half_width);
    glVertex3f(length, 0.01F, -half_width);
    glVertex3f(length, 0.01F, half_width);
    glVertex3f(-length, 0.01F, half_width);
    glEnd();

    glColor3f(0.95F, 0.95F, 0.1F);
    glLineWidth(2.0F);
    glBegin(GL_LINES);
    glVertex3f(-length, 0.02F, 0.0F);
    glVertex3f(length, 0.02F, 0.0F);
    glEnd();

    glColor3f(0.5F, 0.5F, 0.5F);
    glBegin(GL_LINES);
    glVertex3f(-length, 0.015F, -half_width * 0.5F);
    glVertex3f(length, 0.015F, -half_width * 0.5F);
    glVertex3f(-length, 0.015F, half_width * 0.5F);
    glVertex3f(length, 0.015F, half_width * 0.5F);
    glEnd();
}

void GlRenderAdapter::drawUnitCube() const
{
    glBegin(GL_QUADS);
    // Front face (+Z)
    glVertex3f(-0.5F, -0.5F, 0.5F);
    glVertex3f(0.5F, -0.5F, 0.5F);
    glVertex3f(0.5F, 0.5F, 0.5F);
    glVertex3f(-0.5F, 0.5F, 0.5F);
    // Back face (-Z)
    glVertex3f(-0.5F, -0.5F, -0.5F);
    glVertex3f(-0.5F, 0.5F, -0.5F);
    glVertex3f(0.5F, 0.5F, -0.5F);
    glVertex3f(0.5F, -0.5F, -0.5F);
    // Left face (-X)
    glVertex3f(-0.5F, -0.5F, -0.5F);
    glVertex3f(-0.5F, -0.5F, 0.5F);
    glVertex3f(-0.5F, 0.5F, 0.5F);
    glVertex3f(-0.5F, 0.5F, -0.5F);
    // Right face (+X)
    glVertex3f(0.5F, -0.5F, -0.5F);
    glVertex3f(0.5F, 0.5F, -0.5F);
    glVertex3f(0.5F, 0.5F, 0.5F);
    glVertex3f(0.5F, -0.5F, 0.5F);
    // Top face (+Y)
    glVertex3f(-0.5F, 0.5F, -0.5F);
    glVertex3f(-0.5F, 0.5F, 0.5F);
    glVertex3f(0.5F, 0.5F, 0.5F);
    glVertex3f(0.5F, 0.5F, -0.5F);
    // Bottom face (-Y)
    glVertex3f(-0.5F, -0.5F, -0.5F);
    glVertex3f(0.5F, -0.5F, -0.5F);
    glVertex3f(0.5F, -0.5F, 0.5F);
    glVertex3f(-0.5F, -0.5F, 0.5F);
    glEnd();
}

void GlRenderAdapter::drawPrism(float width, float height, float depth) const
{
    glPushMatrix();
    glScalef(width, height, depth);
    drawUnitCube();
    glPopMatrix();
}

void GlRenderAdapter::drawVehicle(const domain::Vehicle &vehicle) const
{
    const auto pos = vehicle.position();
    glPushMatrix();
    glTranslatef(pos.x, 0.9F, pos.y);
    const float heading_deg = -vehicle.headingRad() * 180.0F / kPi;
    glRotatef(heading_deg, 0.0F, 1.0F, 0.0F);
    glColor3f(0.08F, 0.6F, 0.9F);
    drawPrism(4.0F, 1.6F, 2.0F);
    glTranslatef(0.4F, 0.9F, 0.0F);
    glColor3f(0.1F, 0.2F, 0.3F);
    drawPrism(2.0F, 1.0F, 1.6F);
    glPopMatrix();
}

void GlRenderAdapter::drawPedestrians(std::span<const domain::Pedestrian> pedestrians) const
{
    glColor3f(0.95F, 0.3F, 0.2F);
    for (const auto &ped : pedestrians)
    {
        const auto pos = ped.position();
        glPushMatrix();
        glTranslatef(pos.x, 0.9F, pos.y);
        drawPrism(0.6F, 1.8F, 0.6F);
        glPopMatrix();
    }
}
} // namespace ads::adapter
