#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <random>

namespace
{
constexpr unsigned int kWindowWidth = 800;
constexpr unsigned int kWindowHeight = 600;
} // namespace

int main()
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.attributeFlags = sf::ContextSettings::Core;

    sf::RenderWindow window(sf::VideoMode(kWindowWidth, kWindowHeight),
                            "3D Cube",
                            sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    if (!gladLoadGL(sf::Context::getFunction))
    {
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    const std::string shaderDir = std::string(ASSETS_DIR) + "/shader/";
    Shader shader(shaderDir + "shader.vs", shaderDir + "shader.fs");

    const float vertices[] = {
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f};

    const float planeVertices[] = {
        25.0f, -0.5f, 25.0f, 0.5f, 0.5f, 0.5f,
        -25.0f, -0.5f, 25.0f, 0.5f, 0.5f, 0.5f,
        -25.0f, -0.5f, -25.0f, 0.5f, 0.5f, 0.5f,

        25.0f, -0.5f, 25.0f, 0.5f, 0.5f, 0.5f,
        -25.0f, -0.5f, -25.0f, 0.5f, 0.5f, 0.5f,
        25.0f, -0.5f, -25.0f, 0.5f, 0.5f, 0.5f};

    // cube
    unsigned int VBO, VAO = 0;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // cube - position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);
    // cube - color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // tile
    unsigned int planeVBO, planeVAO = 0;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    // tile - position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);
    // tile - color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // pedestrian
    glm::vec3 pedestrianPosition(5.0f, 0.0f, 5.0f);
    glm::vec3 pedestrianDirection(1.0f, 0.0f, 0.0f);
    float pedestrianMoveSpeed = 1.0f;

    sf::Clock pedestrianDirectionChangeClock;
    float nextDirectionChangeTime = 3.0f;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    glm::vec3 cubePosition(0.0f, 0.0f, 0.0f);
    float cubeYaw = 0.0f;
    float cameraDistance = 7.0f;
    float cameraPitchDeg = 25.0f; // 위/아래
    float cameraYawDeg = -135.0f; // 수평 회전
    float cameraRotateSensitivity = 0.2f;
    float cameraZoomSpeed = 1.0f;
    float moveSpeed = 2.0f;
    float turnSpeed = 80.0f;
    bool isOrbiting = false;
    sf::Vector2i lastMousePos = sf::Mouse::getPosition(window);
    sf::Clock deltaClock;

    while (window.isOpen())
    {
        float deltaTime = deltaClock.restart().asSeconds();

        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
            {
                isOrbiting = true;
                lastMousePos = sf::Mouse::getPosition(window);
            }
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right)
            {
                isOrbiting = false;
            }
            if (event.type == sf::Event::MouseMoved && isOrbiting)
            {
                sf::Vector2i currentPos = sf::Mouse::getPosition(window);
                sf::Vector2i delta = currentPos - lastMousePos;
                lastMousePos = currentPos;

                cameraYawDeg -= static_cast<float>(delta.x) * cameraRotateSensitivity;
                cameraPitchDeg -= static_cast<float>(delta.y) * cameraRotateSensitivity;
                cameraPitchDeg = std::clamp(cameraPitchDeg, -89.0f, 89.0f);
            }
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                cameraDistance -= event.mouseWheelScroll.delta * cameraZoomSpeed;
                cameraDistance = std::clamp(cameraDistance, 2.0f, 30.0f);
            }
        }

        // keyboard
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            cubePosition.x += -sin(glm::radians(cubeYaw)) * moveSpeed * deltaTime;
            cubePosition.z += -cos(glm::radians(cubeYaw)) * moveSpeed * deltaTime;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            cubePosition.x -= -sin(glm::radians(cubeYaw)) * moveSpeed * deltaTime;
            cubePosition.z -= -cos(glm::radians(cubeYaw)) * moveSpeed * deltaTime;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            cubeYaw += turnSpeed * deltaTime;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            cubeYaw -= turnSpeed * deltaTime;
        }

        // pedestrian update
        if (pedestrianDirectionChangeClock.getElapsedTime().asSeconds() >= nextDirectionChangeTime)
        {
            pedestrianDirection.x = dis(gen);
            pedestrianDirection.z = dis(gen);
            pedestrianDirection.y = 0.0f;
            pedestrianDirection = glm::normalize(pedestrianDirection);

            std::uniform_real_distribution<> timeDis(2.0, 5.0);
            nextDirectionChangeTime = timeDis(gen);
            pedestrianDirectionChangeClock.restart();

            std::cout << "Pedestrian new direction: (" << pedestrianDirection.x << ", " << pedestrianDirection.z << ")" << std::endl;
        }
        pedestrianPosition += pedestrianDirection * pedestrianMoveSpeed * deltaTime;
        float mapHalfSize = 24.0f;
        if (pedestrianPosition.x > mapHalfSize)
            pedestrianPosition.x = mapHalfSize;
        if (pedestrianPosition.x < -mapHalfSize)
            pedestrianPosition.x = -mapHalfSize;
        if (pedestrianPosition.z > mapHalfSize)
            pedestrianPosition.z = mapHalfSize;
        if (pedestrianPosition.z < -mapHalfSize)
            pedestrianPosition.z = -mapHalfSize;

        // Rendering
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();

        // Rendering - tile
        glm::mat4 planeModel = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(planeModel));
        glBindVertexArray(planeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Rendering - cube
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePosition);
        model = glm::rotate(model, glm::radians(cubeYaw), glm::vec3(0.0f, 1.0f, 0.0f));

        const float yawRad = glm::radians(cameraYawDeg);
        const float pitchRad = glm::radians(cameraPitchDeg);
        const float cosPitch = cos(pitchRad);
        glm::vec3 cameraPos = cubePosition +
                              glm::vec3(cosPitch * cos(yawRad),
                                        sin(pitchRad),
                                        cosPitch * sin(yawRad)) *
                                  cameraDistance;
        glm::mat4 view = glm::lookAt(cameraPos, cubePosition, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(kWindowWidth) / static_cast<float>(kWindowHeight), 0.1f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Rendering - pedestrian
        glm::mat4 pedestrianModel = glm::mat4(1.0f);
        pedestrianModel = glm::translate(pedestrianModel, pedestrianPosition);
        pedestrianModel = glm::scale(pedestrianModel, glm::vec3(0.3f, 1.0f, 0.3f));
        glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(pedestrianModel));
        glBindVertexArray(VAO); // cube와 동일한 정점 데이터(VAO) 사용
        glDrawArrays(GL_TRIANGLES, 0, 36);

        window.display();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &planeVBO);

    return 0;
}
