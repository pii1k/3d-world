#include <cstddef>
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

namespace
{
constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEGITH = 600;
constexpr unsigned int LOG_BUF_SIZE = 512;

const std::string vertexShaderSource = R"(
  #version 330 core
  layout (location = 0) in vec3 aPos;
  layout (location = 1) in vec3 aColor;
  out vec3 ourColor;
  void main()
  {
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
  }
  )";

const std::string fragmentShaderSource = R"(
  #version 330 core
  out vec4 FragColor;
  in vec3 ourColor;
  void main()
  {
    FragColor = vec4(ourColor, 1.0f);
  }
)";

enum class TargetType
{
    SHADER,
    SHADER_PROGRAM
};

void checkError(unsigned int target, TargetType target_type, unsigned int symbolic_constant, std::string label)
{
    int result;
    std::string infoLog;
    infoLog.resize(LOG_BUF_SIZE);

    switch (target_type)
    {
    case TargetType::SHADER:
        glGetShaderiv(target, symbolic_constant, &result);
        if (!result)
        {
            glGetShaderInfoLog(target, LOG_BUF_SIZE, NULL, infoLog.data());
            std::cout << "ERROR::" << label << '\n'
                      << infoLog << std::endl;
        }
        break;
    case TargetType::SHADER_PROGRAM:
        glGetProgramiv(target, symbolic_constant, &result);
        if (!result)
        {
            glGetProgramInfoLog(target, LOG_BUF_SIZE, NULL, infoLog.data());
            std::cout << "ERROR::" << label << '\n'
                      << infoLog << std::endl;
        }
        break;
    }
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow * /* window */, int width, int height)
{
    glViewport(0, 0, width, height);
}
} // namespace

int main()
{
    // glfw: init and config
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw: window creation
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEGITH, "Learn OpenGL", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    {
        std::cout << "Failed to init GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *vertexShaderCPtr = vertexShaderSource.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderCPtr, NULL);
    glCompileShader(vertexShader);
    checkError(vertexShader, TargetType::SHADER, GL_COMPILE_STATUS, "SHADER::VERTEX");

    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragmentShaderCPtr = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderCPtr, NULL);
    glCompileShader(fragmentShader);
    checkError(fragmentShader, TargetType::SHADER, GL_COMPILE_STATUS, "SHADER::FRAGMENT");

    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkError(shaderProgram, TargetType::SHADER_PROGRAM, GL_LINK_STATUS, "SHADER::PROGRAM");
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
        // positions         // colors
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // top
    };
    // unsigned int indices[] = {
    //     0, 1, 3, // first triangle
    //     1, 2, 3  // second triangle
    // };
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // unsigned int EBO;
    // glGenBuffers(1, &EBO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);
    // glBindVertexArray(VAO);

    glUseProgram(shaderProgram);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render the triagnle
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: swap buffers and poll IO events (key pressed/released, mouse ..)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}
