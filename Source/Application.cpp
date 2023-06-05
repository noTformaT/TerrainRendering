#include "Application.h"

#include <stdio.h>
#include <string.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// windows dimentions
const GLint WIDTH = 1027, HEIGHT = 768;

Application::Application()
{
    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = false;
    }
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
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    while (window && !glfwWindowShouldClose(window))
    {
        GLfloat now = (GLfloat)glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        UpdateMouse();
        m_pGameCamera.KeyControl(keys, deltaTime);
        m_pGameCamera.MouseControl(GetMouseXChange(), GetMouseYChange(), deltaTime);

        RenderScene();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT);

    m_terrain.Render(m_pGameCamera);

    RenderUI();
}

void Application::RenderUI()
{

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Params");
    ImGui::Text("Hello");
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::PassiveMouseCB(int32_t x, int32_t y)
{
    
}

void Application::KeyboardCB(uint32_t key, int32_t state)
{
    if (key >= 0 && key < 1024)
    {
        if (state == GLFW_PRESS)
        {
            keys[key] = true;
            //printf("Pressed: %d\n", key);
        }
        else
        {
            if (state == GLFW_RELEASE)
            {
                keys[key] = false;
                //printf("Released: %d\n", key);
            }
        }
    }


    if (state == GLFW_PRESS)
    {
        switch (key) {

            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);

            case GLFW_KEY_C:
                //m_pGameCamera->Print();
                break;

            case GLFW_KEY_W:
                //m_isWireframe = !m_isWireframe;

                /*if (m_isWireframe) {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
                else {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }*/

            break;
        }
    }
}

void Application::MouseCB(int button, int action, int x, int y)
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

    // Setup GLFW window properties

    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Core profile = No Backwards Compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "TerrainRendering", monitor, NULL);

    if (!window) {
        printf("GLFW window creation FAILED!");
        glfwTerminate();
    }

    //int bufferWidth, bufferHeight;
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

    glfwSetWindowUserPointer(window, this);
}

void Application::InitCallbacks()
{
    glfwSetKeyCallback(window, KeyCallback);
    //glfwSetCursorPosCallback(window, CursorPosCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Application::InitCamera()
{
    m_pGameCamera = Camera(glm::vec3(10.0f, 12.0f, .0f), glm::vec3(.0f, 1.0f, .0f), -0.0f, 0.0f, 2.0f, 0.3f);

    m_pGameCamera.SetProjection(glm::perspective(glm::radians(45.0f), (float)bufferWidth / (float)bufferHeight, 0.1f, 2000.0f));
}

void Application::InitTerrain()
{
    m_terrain.InitTerrain();
    m_terrain.LoadFromFile("data/heightmap.save");
}

void Application::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));

    app->KeyboardCB(key, action);
}

void Application::CursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app->mouseFirstMoved)
    {
        app->lastX = (float)xPos;
        app->lastY = (float)yPos;
        app->mouseFirstMoved = false;
    }

    app->xChange = (float)xPos - app->lastX;
    app->yChange = app->lastY - (float)yPos;

    app->lastX = (float)xPos;
    app->lastY = (float)yPos;
}

void Application::MouseButtonCallback(GLFWwindow* window, int Button, int Action, int Mode)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    double x, y;

    glfwGetCursorPos(window, &x, &y);

    app->MouseCB(Button, Action, (int)x, (int)y);
}

void Application::UpdateMouse()
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    Application::CursorPosCallback(window, xpos, ypos);
}

GLfloat Application::GetMouseXChange()
{
    GLfloat result = xChange;
    xChange = .0f;
    return result;
}

GLfloat Application::GetMouseYChange()
{
    GLfloat result = yChange;
    yChange = .0f;
    return result;
}
