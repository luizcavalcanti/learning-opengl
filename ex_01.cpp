#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // vertex shader
    const char *vertexShaderSource = "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "void main() {\n"
      "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
      "}\0";

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);


    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (success) {
      std::cout << "Shader compiled successfuly" << std::endl;
    } else {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    const char *fragmentShaderSource = "#version 330 core\n"
      "out vec4 FragColor;\n"
      "void main() {\n"
      "    FragColor = vec4(1.0f, 0.5f, 0.2f, 0.3f);\n"
      "}\0";

    const char *fragmentShaderSourceB = "#version 330 core\n"
      "out vec4 FragColor;\n"
      "void main() {\n"
      "    FragColor = vec4(0.1f, 0.3f, 0.9f, 0.4f);\n"
      "}\0";

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    if (success) {
      std::cout << "Shader compiled successfuly" << std::endl;
    } else {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShaderB;
    fragmentShaderB = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderB, 1, &fragmentShaderSourceB, NULL);
    glCompileShader(fragmentShaderB);

    if (success) {
      std::cout << "Shader compiled successfuly" << std::endl;
    } else {
      glGetShaderInfoLog(fragmentShaderB, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // shader program
    unsigned int shaderProgram, shaderProgramB;
    shaderProgram = glCreateProgram();
    shaderProgramB = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      std::cout << "ERROR::SHADERPROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glAttachShader(shaderProgramB, vertexShader);
    glAttachShader(shaderProgramB, fragmentShaderB);
    glLinkProgram(shaderProgramB);

    glGetProgramiv(shaderProgramB, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shaderProgramB, 512, NULL, infoLog);
      std::cout << "ERROR::SHADERPROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // vertices data, VBO and VAO
    float verticesA[] = {
      -0.5f, -0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
      0.0f,  0.5f, 0.0f,
    };

    float verticesB[] = {
      // segundo triangulo
      -0.9f, -0.9f, -0.7f,
      -0.5f, -0.9f, -0.7f,
      -0.7f, -0.5f, -0.7f
    };

    // Create vertex buffer and array
    unsigned int aVAO, bVAO, aVBO, bVBO;
    glGenVertexArrays(1, &aVAO);
    glGenBuffers(1, &aVBO);

    // Bind array first and then buffer
    glBindVertexArray(aVAO);
    glBindBuffer(GL_ARRAY_BUFFER, aVBO);

    // Send data to buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesA), verticesA, GL_STATIC_DRAW);

    // Set attibutes pointers/config
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenVertexArrays(1, &bVAO);
    glGenBuffers(1, &bVBO);

    // Bind array first and then buffer
    glBindVertexArray(bVAO);
    glBindBuffer(GL_ARRAY_BUFFER, bVBO);

    // Send data to buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesB), verticesB, GL_STATIC_DRAW);

    // Set attibutes pointers/config
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(aVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glUseProgram(shaderProgramB);
        glBindVertexArray(bVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &aVAO);
    glDeleteVertexArrays(1, &bVAO);
    glDeleteBuffers(1, &aVBO);
    glDeleteBuffers(1, &bVBO);
    glDeleteProgram(shaderProgram);
    glDeleteProgram(shaderProgramB);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
