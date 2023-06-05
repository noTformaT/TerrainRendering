#include "Application.h"

#include <stdio.h>
#include <string.h>

// windows dimentions
const GLint WIDTH = 800, HEIGHT = 600;

GLuint VBO, VAO, shader;

// Vertex Shader
static const char* vShader = "                                         \n\
#version 330                                                           \n\
layout (location = 0) in vec3 pos;                                       \n\
                                                                       \n\
void main()                                                            \n\
{                                                                      \n\
    gl_Position = vec4(0.4f * pos.x, 0.4f * pos.y, pos.z, 1.0f);       \n\
}";

// Fragment Shader
static const char* fShader = "                      \n\
#version 330                                        \n\
out vec4 color;                                     \n\
                                                    \n\
void main()                                         \n\
{                                                   \n\
    color =  vec4(1.0f, 0.0f, 0.0f, 1.0f);     \n\
}                                                   \n\
";

void CreateTriangle()
{
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void AddShader(GLuint program, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* code[1];
    code[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, code, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar errorLog[128] = { 0 };

    // compile programm
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(errorLog), nullptr, errorLog);
        printf("Error compiling the %d shader: %s\n", shaderType, errorLog);
        return;
    }

    glAttachShader(program, theShader);
}

void CompileShaders()
{
    // make program
    shader = glCreateProgram();

    if (!shader)
    {
        printf("Error: Creating shader program!\n");
        return;
    }

    // add shader code to programm
    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar errorLog[128] = { 0 };

    // compiling programm
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(errorLog), nullptr, errorLog);
        printf("Error linking program: %s\n", errorLog);
        return;
    }

    // validation programm
    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(errorLog), nullptr, errorLog);
        printf("Error validation program: %s\n", errorLog);
        return;
    }
}

Application::Application()
{
    /*

    // Initialize GLFW
    if (!glfwInit())
    {
        printf("GLFW Initialization FAILED!");
        glfwTerminate();
        //return 1;
    }

    // Setup GLFW window properties

    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Core profile = No Backwards Compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", nullptr, nullptr);
    if (!mainWindow)
    {
        printf("GLFW window creation FAILED!");
        glfwTerminate();
        //return 1;
    }

    // Get buffer size information
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set context FOR GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("GLEW Initialization FAILED!");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        //return 1;
    }

    // Setup viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    // Create triangles and shaders
    CreateTriangle();
    CompileShaders();

    // Loop until window closed
    while (!glfwWindowShouldClose(mainWindow))
    {
        // Get and hanlde user input events
        glfwPollEvents();

        // Clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }
    */
}

Application::~Application()
{

}

void Application::Init()
{
	CreateWindow();
	InitCallbacks();
	InitCamera();
	InitTerrain();
}

void Application::Run()
{
    while (window && !glfwWindowShouldClose(window))
    {
        RenderScene();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::RenderScene()
{
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Clear window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader);

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);
    glUseProgram(0);

}

void Application::PassiveMouseCB(int32_t x, int32_t y)
{
    
}

void Application::KeyboardCB(uint32_t key, int32_t state)
{
    if (state == GLFW_PRESS)
    {

    }
}

void Application::MouseCB()
{
}

void Application::CreateWindow()
{
    int major_ver = 0;
    int minor_ver = 0;
    bool is_full_screen = false;

    if (!glfwInit())
    {
        printf("GLFW Initialization FAILED!");
        glfwTerminate();
    }

    int Major, Minor, Rev;

    glfwGetVersion(&Major, &Minor, &Rev);

    printf("GLFW %d.%d.%d initialized\n", Major, Minor, Rev);

    GLFWmonitor* monitor = is_full_screen ? glfwGetPrimaryMonitor() : NULL;
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    if (major_ver > 0) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_ver);
    }

    if (minor_ver > 0) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_ver);
    }

    window = glfwCreateWindow(WIDTH, HEIGHT, "TerrainRendering", monitor, NULL);

    if (!window) {
        printf("GLFW window creation FAILED!");
        glfwTerminate();
    }

    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

    // Set context FOR GLEW to use
    glfwMakeContextCurrent(window);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("GLEW Initialization FAILED!");
        glfwDestroyWindow(window);
        glfwTerminate();
        //return 1;
    }

    // Setup viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    CreateTriangle();
    CompileShaders();
}

void Application::InitCallbacks()
{
}

void Application::InitCamera()
{
}

void Application::InitTerrain()
{
}
