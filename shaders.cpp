#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
// #include <cmath>

#include "shader.h"

void initGLFW();
bool initGLAD();
GLFWwindow* createWindow(unsigned int width, unsigned int height, const char* title, bool fullscreen);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
    initGLFW();

    GLFWwindow* window = createWindow(SCR_WIDTH, SCR_HEIGHT, "Teste", true);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        return -1;
    }

    if (!initGLAD()) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader myShader("shaders/invert.vs", "shaders/shader.fs");

    // vertices data, VBO and VAO
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
    };

    // Create vertex buffer and array
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind array first and then buffer
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send data to buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set attibutes pointers/config
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);


    while (!glfwWindowShouldClose(window))
        {
            processInput(window);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            myShader.use();

            // int vertexColorLocation = glGetUniformLocation(shaderProgram, "myColor");
            // float timeValue = glfwGetTime();
            // float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
            // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void initGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif  
}

bool initGLAD() {
    // load all OpenGL function pointers
    return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) != 0;
}

GLFWwindow* createWindow(unsigned int width, unsigned int height, const char* title, bool fullscreen) {
    GLFWwindow* window = glfwCreateWindow(width, height, title,
                                          fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}


void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
