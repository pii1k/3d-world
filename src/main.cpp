#include "glad/gl.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "shader.hpp"

#include <algorithm>
#include <deque>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace
{
constexpr int kGridW = 24;
constexpr int kGridH = 18;

constexpr int kWindowW = 960;
constexpr int kWindowH = 720;

constexpr float kTickSeconds = 0.12f;

int g_framebuffer_w = kWindowW;
int g_framebuffer_h = kWindowH;

struct IVec2
{
    int x = 0;
    int y = 0;
};

bool operator==(const IVec2 &a, const IVec2 &b)
{
    return a.x == b.x && a.y == b.y;
}

int toIndex(const IVec2 &p)
{
    return p.y * kGridW + p.x;
}

enum class Direction
{
    Up,
    Down,
    Left,
    Right,
};

bool isOpposite(Direction a, Direction b)
{
    return (a == Direction::Up && b == Direction::Down) || (a == Direction::Down && b == Direction::Up) ||
           (a == Direction::Left && b == Direction::Right) || (a == Direction::Right && b == Direction::Left);
}

IVec2 directionDt(Direction d)
{
    switch (d)
    {
    case Direction::Up:
        return {0, 1};
    case Direction::Down:
        return {0, -1};
    case Direction::Left:
        return {-1, 0};
    case Direction::Right:
        return {1, 0};
    }
    return {0, 0};
}

struct SnakeGame
{
    std::deque<IVec2> snake_;
    std::vector<uint8_t> occupied;
    IVec2 food{};

    Direction direct_{Direction::Right};
    Direction pending_direct_{Direction::Right};

    bool game_over{false};
    int score{0};

    std::mt19937 rng{std::random_device{}()};

    SnakeGame()
    {
        reset();
    }

    void reset()
    {
        occupied.assign(kGridW * kGridH, 0);
        snake_.clear();

        const IVec2 start{kGridW / 2, kGridH / 2};
        snake_.push_front(start);
        occupied[toIndex(start)] = 1;

        direct_ = Direction::Right;
        pending_direct_ = Direction::Right;
        game_over = false;
        score = 0;

        spawnFood();
    }

    void spawnFood()
    {
        std::uniform_int_distribution<int> dist_x(0, kGridW - 1);
        std::uniform_int_distribution<int> dist_y(0, kGridH - 1);

        for (int tries = 0; tries < 10'000; ++tries)
        {
            IVec2 p{dist_x(rng), dist_y(rng)};
            if (!occupied[toIndex(p)])
            {
                food = p;
                return;
            }
        }

        food = {-1, -1};
    }

    void setPendingDir(Direction next)
    {
        if (isOpposite(direct_, next))
            return;
        pending_direct_ = next;
    }

    void step()
    {
        if (game_over)
            return;

        direct_ = pending_direct_;

        const IVec2 d = directionDt(direct_);
        const IVec2 head = snake_.front();
        const IVec2 next{head.x + d.x, head.y + d.y};

        if (next.x < 0 || next.x >= kGridW || next.y < 0 || next.y >= kGridH)
        {
            game_over = true;
            return;
        }

        const bool will_eat = (next == food);

        // If we won't grow, the tail cell will be freed; allow moving into it.
        if (!will_eat)
        {
            const IVec2 tail = snake_.back();
            occupied[toIndex(tail)] = 0;
        }

        if (occupied[toIndex(next)])
        {
            game_over = true;
            return;
        }

        snake_.push_front(next);
        occupied[toIndex(next)] = 1;

        if (will_eat)
        {
            score += 1;
            spawnFood();
        }
        else
        {
            snake_.pop_back();
        }
    }
};

void framebuffer_size_callback(GLFWwindow * /* window */, int width, int height)
{
    g_framebuffer_w = width;
    g_framebuffer_h = height;
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow *window, int key, int /* scancode */, int action, int /* mods */)
{
    if (action != GLFW_PRESS)
        return;

    auto *game = static_cast<SnakeGame *>(glfwGetWindowUserPointer(window));
    if (!game)
        return;

    if (key == GLFW_KEY_ESCAPE)
    {
        glfwSetWindowShouldClose(window, true);
        return;
    }
    if (key == GLFW_KEY_R)
    {
        game->reset();
        return;
    }

    if (key == GLFW_KEY_UP || key == GLFW_KEY_W)
        game->setPendingDir(Direction::Up);
    else if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S)
        game->setPendingDir(Direction::Down);
    else if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A)
        game->setPendingDir(Direction::Left);
    else if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D)
        game->setPendingDir(Direction::Right);
}

void drawCell(GLuint vao,
              GLint u_mvp_loc,
              GLint u_color_loc,
              const glm::mat4 &projection,
              const IVec2 &cell,
              const glm::vec3 &color)
{
    const glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(static_cast<float>(cell.x), static_cast<float>(cell.y), 0.0f));
    const glm::mat4 mvp = projection * model;
    glUniformMatrix4fv(u_mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniform3f(u_color_loc, color.r, color.g, color.b);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void drawGame(GLuint vao,
              GLint u_mvp_loc,
              GLint u_color_loc,
              const glm::mat4 &projection,
              const SnakeGame &game)
{
    if (game.food.x >= 0)
        drawCell(vao, u_mvp_loc, u_color_loc, projection, game.food, {0.95f, 0.25f, 0.25f});

    bool first = true;
    for (const auto &cell : game.snake_)
    {
        if (first)
        {
            drawCell(vao, u_mvp_loc, u_color_loc, projection, cell, {0.2f, 0.95f, 0.4f});
            first = false;
        }
        else
        {
            drawCell(vao, u_mvp_loc, u_color_loc, projection, cell, {0.15f, 0.65f, 0.4f});
        }
    }
}
} // namespace

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(kWindowW, kWindowH, "Snake (GLFW/OpenGL)", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwGetFramebufferSize(window, &g_framebuffer_w, &g_framebuffer_h);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        glfwTerminate();
        return -1;
    }

    glDisable(GL_DEPTH_TEST);

    SnakeGame game{};
    glfwSetWindowUserPointer(window, &game);
    glfwSetKeyCallback(window, key_callback);

    const std::string shader_dir = std::string(ASSETS_DIR) + "/shader/";
    Shader shader(shader_dir + "snake.vs", shader_dir + "snake.fs");
    shader.use();

    const GLint u_mvp_loc = glGetUniformLocation(shader.getId(), "uMVP");
    const GLint u_color_loc = glGetUniformLocation(shader.getId(), "uColor");

    const float quad_vertices[] = {
        // unit quad in [0,1]x[0,1]
        0.0f,
        0.0f,
        1.0f,
        0.0f,
        1.0f,
        1.0f,
        0.0f,
        1.0f,
    };
    const unsigned int quad_indices[] = {0, 1, 2, 0, 2, 3};

    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_indices), quad_indices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glBindVertexArray(0);

    const glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(kGridW), 0.0f, static_cast<float>(kGridH));

    float last_time = static_cast<float>(glfwGetTime());
    float accumulator = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        const float now = static_cast<float>(glfwGetTime());
        float dt = now - last_time;
        last_time = now;
        if (dt > 0.25f)
            dt = 0.25f;

        glfwPollEvents();

        accumulator += dt;
        while (accumulator >= kTickSeconds)
        {
            game.step();
            accumulator -= kTickSeconds;
        }

        glClearColor(0.06f, 0.07f, 0.09f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        // Main view (full window)
        glViewport(0, 0, g_framebuffer_w, g_framebuffer_h);
        drawGame(vao, u_mvp_loc, u_color_loc, projection, game);

        // // Mini-map (top-right)
        // const int margin = 12;
        // int mini_w = std::max(1, g_framebuffer_w / 4);
        // int mini_h = std::max(1, (mini_w * kGridH) / kGridW);
        // const int mini_x = std::max(margin, g_framebuffer_w - margin - mini_w);
        // const int mini_y = std::max(margin, g_framebuffer_h - margin - mini_h);
        // glViewport(mini_x, mini_y, mini_w, mini_h);
        // drawGame(vao, u_mvp_loc, u_color_loc, projection, game);

        glfwSwapBuffers(window);
    }

    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    glfwTerminate();
    return 0;
}