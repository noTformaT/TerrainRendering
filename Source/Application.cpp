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

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
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
        
        if (m_isToogleInput)
        {
            m_pGameCamera.KeyControl(keys, deltaTime);
            m_pGameCamera.MouseControl(GetMouseXChange(), GetMouseYChange(), deltaTime);
        }

        RenderScene(deltaTime);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::RenderScene(float dt)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_terrain.Render(m_pGameCamera);

    RenderUI(dt);
}

void Application::RenderUI(float dt)
{

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    glm::vec3 pos = m_pGameCamera.GetPosition();

    ImGui::Begin("Stats");

    ImGui::Text("RenderTime: %f(ms)", dt);
    ImGui::Text("FPS: %f", 1.0f / dt);

    ImGui::Text("Terrain width: %f", m_terrain.GetTerrainSize());
    ImGui::Text("Terrain height: %f", m_terrain.GetTerrainSize());

    ImGui::Checkbox("Wireframe", &m_isWireframe);
    ImGui::Checkbox("Input enable", &m_isToogleInput);

    ImGui::Text("Camera position:");
    ImGui::Text("%f, %f, %f", pos.x, pos.y, pos.z);

    pos = m_pGameCamera.GetFront();
    ImGui::Text("Camera direction:");
    ImGui::Text("%f, %f, %f", pos.x, pos.y, pos.z);
    ImGui::End();


    ImGui::Begin("Fault formation generator");

    ImGui::SliderInt("Iterations", &m_terrain.iterration, 0, 1000);
    ImGui::SliderFloat("Max Height", &m_terrain.maxHeight, 0.0f, 1000.0f);
    ImGui::SliderFloat("Filter", &m_terrain.filter, 0.0f, 1.0f);

    if (ImGui::Button("Generate"))
    {
        m_terrain.Destroy();
        m_terrain.CreateFaultFormation(m_terrain.size, m_terrain.iterration, m_terrain.minHeight, m_terrain.maxHeight, m_terrain.filter);
    }

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
                break;

            case GLFW_KEY_C:
                m_isWireframe = !m_isWireframe;

                if (m_isWireframe) {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
                else {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
                break;

            case GLFW_KEY_T:
                m_isToogleInput = !m_isToogleInput;
                glfwSetInputMode(window, GLFW_CURSOR, m_isToogleInput ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
                break;

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

    if (m_isToogleInput)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

void Application::InitCamera()
{
    m_pGameCamera = Camera(glm::vec3(18.0f, 228, -170.0f), glm::vec3(.0f, 1.0f, .0f), -0.0f, 0.0f, 50.0f, 0.3f);

    m_pGameCamera.SetProjection(glm::perspective(glm::radians(45.0f), (float)bufferWidth / (float)bufferHeight, 0.1f, 2000.0f));
}

void Application::InitTerrain()
{
    float worldScale = 4.0f;
    m_terrain.InitTerrain(worldScale);
    //m_terrain.LoadFromFile("data/heightmap.save");

    
    m_terrain.CreateFaultFormation(m_terrain.size, m_terrain.iterration, m_terrain.minHeight, m_terrain.maxHeight, m_terrain.filter);
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
