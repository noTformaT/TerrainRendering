#include "Application.h"

#include <stdio.h>
#include <string>
#include <vector>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Util.h"

// windows dimentions
const GLint WIDTH = 1024, HEIGHT = 768;

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
	InitCamera(false);
    InitSkyBox();
	InitTerrain();

    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
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

    m_skyBox.Render(m_pGameCamera, bufferWidth, bufferHeight);

    m_terrain0.UpdateLayers(h0, h1, h2, h3, h4, h5);
    m_terrain1.UpdateLayers(h0, h1, h2, h3, h4, h5);

    switch (m_terrainIndex)
    {
    case 0:
        m_terrain0.Render(m_isGeoMappingRender, m_pGameCamera, lightingData, bufferWidth, bufferHeight);
        break;
    case 1:
        m_terrain1.Render(m_isGeoMappingRender, m_pGameCamera, lightingData, bufferWidth, bufferHeight);
        break;
    default:
        break;
    }
    

    RenderUI(dt);
}

void Application::RenderUI(float dt)
{

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    glm::vec3 pos = m_pGameCamera.GetPosition();

    bool renderShadowMapView = false;
    GLint width = 0;
    GLint height = 0;
    GLint x = 0;
    GLint y = 0;

    if (ImGui::Begin("Debug ShadowMap"))
    {
        ImGui::SetWindowSize(ImVec2(256, 256));

        renderShadowMapView = true;

        ImVec2 vMin = ImGui::GetWindowContentRegionMin();
        ImVec2 vMax = ImGui::GetWindowContentRegionMax();

        width = GLint(vMax.x - vMin.x);
        height = GLint(vMax.y - vMin.y);

        vMin.x += ImGui::GetWindowPos().x;
        vMin.y += ImGui::GetWindowPos().y;
        vMax.x += ImGui::GetWindowPos().x;
        vMax.y += ImGui::GetWindowPos().y;

        ImGui::GetForegroundDrawList()->AddRect(vMin, vMax, IM_COL32(255, 255, 0, 0));

        x = GLint(vMin.x);
        y = GLint(vMin.y) + height;
        y = bufferHeight - y;
    }
    ImGui::End();

    ImGui::Begin("Options");

    if (ImGui::CollapsingHeader("Stats"))
    {

        ImGui::Text("RenderTime: %f(ms)", dt);
        ImGui::Text("FPS: %f", 1.0f / dt);

        ImGui::Text("Terrain width: %f", 256.0);
        ImGui::Text("Terrain height: %f", 256.0);

        int item;

        const char* fault = "Fault formation";
        const char* midPoint = "Mid point";
        const char* data[] = { fault, midPoint };
        const char* currentItem = data[m_terrainIndex];

        if (ImGui::BeginCombo("Generator", currentItem))
        {
            for (size_t i = 0; i < 2; i++)
            {
                bool selectable = i == m_terrainIndex;
                if (ImGui::Selectable(data[i], selectable))
                {
                    m_terrainIndex = i;
                }

                if (selectable)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndCombo();
        }

        ImGui::Checkbox("Geomapping Render", &m_isGeoMappingRender);

        bool prevWireframeVal = m_isWireframe;
        ImGui::Checkbox("Wireframe", &prevWireframeVal);
        ImGui::Checkbox("Input enable", &m_isToogleInput);

        if (prevWireframeVal != m_isWireframe)
        {
            m_isWireframe = prevWireframeVal;
            UpdateWireframeState();
        }

        ImGui::Text("Camera position:");
        ImGui::Text("%f, %f, %f", pos.x, pos.y, pos.z);

        pos = m_pGameCamera.GetFront();
        ImGui::Text("Camera direction:");
        ImGui::Text("%f, %f, %f", pos.x, pos.y, pos.z);
    }

    if (ImGui::CollapsingHeader("Layers Settings"))
    {
        float hs0 = h0;
        float hs1 = h1;
        float hs2 = h2;
        float hs3 = h3;
        float hs4 = h4;
        float hs5 = h5;


        ImGui::SliderFloat("Layer 0 end", &hs0, 0.0f, 1.0f);
        ImGui::SliderFloat("Layer 1 begin", &hs1, 0.0f, 1.0f);
        ImGui::SliderFloat("Layer 1 end", &hs2, 0.0f, 1.0f);
        ImGui::SliderFloat("Layer 2 begin", &hs3, 0.0f, 1.0f);
        ImGui::SliderFloat("Layer 2 end", &hs4, 0.0f, 1.0f);
        ImGui::SliderFloat("Layer 3 begin", &hs5, 0.0f, 1.0f);

        if (hs0 > hs1)
        {
            if (hs0 != h0)
            {
                hs1 = hs0;
            }
            else
            {
                hs0 = hs1;
            }
        }

        if (hs1 > hs2)
        {
            if (hs1 != h1)
            {
                hs2 = hs1;
            }
            else
            {
                hs1 = hs2;
            }
        }

        if (hs2 > hs3)
        {
            if (hs2 != h2)
            {
                hs3 = hs2;
            }
            else
            {
                hs2 = hs3;
            }
        }

        if (hs3 > hs4)
        {
            if (hs3 != h3)
            {
                hs4 = hs3;
            }
            else
            {
                hs3 = hs4;
            }
        }

        if (hs4 > hs5)
        {
            if (hs4 != h4)
            {
                hs5 = hs4;
            }
            else
            {
                hs4 = hs5;
            }
        }


        h0 = hs0;
        h1 = hs1;
        h2 = hs2;
        h3 = hs3;
        h4 = hs4;
        h5 = hs5;
        
    }

    if (ImGui::CollapsingHeader("Lighting Settings"))
    {
        float direction[3] = { lightingData.SunDirection.x, lightingData.SunDirection.y, lightingData.SunDirection.z };
        ImGui::SliderFloat3("Sun Direction", direction, -1.0f, 1.0f);
        lightingData.SunDirection = glm::vec3(direction[0], direction[1], direction[2]);

        float colors[3] = { lightingData.SunColor.r, lightingData.SunColor.g, lightingData.SunColor.b };
        ImGui::ColorPicker3("Sun Color", colors);
        lightingData.SunColor = glm::vec3(colors[0], colors[1], colors[2]);

        ImGui::SliderFloat("Sun Intencity", &lightingData.SunIntencity, 0.0f, 2.0f);
        ImGui::SliderFloat("Sun Diffuse", &lightingData.SunDiffuse, 0.0f, 1.0f);

        ImGui::Checkbox("Lit material", &lightingData.LitMaterial);
    }

    if (ImGui::CollapsingHeader("Sky sphere"))
    {
        //const char* data[] = { "Cubemap 1", "Cubemap 2", "Cubemap 3", "Cubemap 4", "Cubemap 5", "Cubemap 6", "Cubemap 7", "Cubemap 8", "Cubemap 9", "Cubemap 10", "Cubemap 11" };

        std::string temp = m_skyBox.GetCubemapName(m_skyBox.GetCubemapIndex());
        const char* currentItem = temp.c_str();

        if (ImGui::BeginCombo("Cubemap", currentItem))
        {
            for (size_t i = 0; i < m_skyBox.GetCubemapCount(); i++)
            {
                bool selectable = i == m_skyBox.GetCubemapIndex();
                temp = m_skyBox.GetCubemapName(i);
                if (ImGui::Selectable(temp.c_str(), selectable))
                {
                    m_skyBox.SetCubemapIndex(i);
                }

                if (selectable)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndCombo();
        }
    }

    if (m_terrainIndex == 0)
    {
        m_terrain0.RenderLODSettings();
    }
    else
    {
        m_terrain1.RenderLODSettings();
    }

    ImGui::End();

    if (m_terrainIndex == 0)
    {
        ImGui::Begin("Fault formation generator");

        ImGui::SliderInt("Iterations", &m_terrain0.iterration, 0, 1000);
        ImGui::SliderFloat("Max Height", &m_terrain0.maxHeight, 0.0f, 1000.0f);
        ImGui::SliderFloat("Filter", &m_terrain0.filter, 0.0f, 1.0f);

        if (ImGui::Button("Generate"))
        {
            m_terrain0.Destroy();
            m_terrain0.CreateFaultFormation(m_terrain0.size, m_terrain0.iterration, m_terrain0.minHeight, m_terrain0.maxHeight, m_terrain0.filter);
        }

        ImGui::End();
    }
    else
    {
        if (m_terrainIndex == 1)
        {
            ImGui::Begin("Midpoint displacement generator");

            ImGui::SliderFloat("Roughness", &m_terrain1.roughness, 0.0f, 2.0f);
            ImGui::SliderFloat("Max Height", &m_terrain1.maxHeight, 0.0f, 1000.0f);

            if (ImGui::Button("Generate"))
            {
                m_terrain1.Destroy();
                m_terrain1.CreateMidpoinDisplacement(m_terrain1.size, m_terrain1.roughness, m_terrain1.minHeight, m_terrain1.maxHeight);
            }

            ImGui::End();
        }
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (renderShadowMapView)
    {
        m_terrain0.RenderShadowMapPreview(x, y, width, height, lightingData);
    }
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

                UpdateWireframeState();
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

void Application::WindowsResize(int width, int height)
{
    //int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

    // Setup viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    InitCamera(true);
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

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

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

    glfwSetWindowSizeCallback(window, WindowsResizeCallback);
}

void Application::InitCamera(bool isRecalculate)
{
    if (!isRecalculate)
    {
        m_pGameCamera = Camera(glm::vec3(18.0f, 228, -170.0f), glm::vec3(.0f, 1.0f, .0f), -0.0f, 0.0f, 200.0f, 0.3f);
        //m_pGameCamera = Camera(glm::vec3(0, 0, 0), glm::vec3(.0f, 1.0f, .0f), -0.0f, 0.0f, 50.0f, 0.3f);
    }
    

    m_pGameCamera.SetProjection(glm::perspective(glm::radians(FOV), (float)bufferWidth / (float)bufferHeight, NEAR_PLANE, FAR_PLANE));
    //m_pGameCamera.SetProjection(glm::ortho(-20.0f, 20.0f, -(HEIGHT / 2.0f), HEIGHT / 2.0f, 0.0f, 2000.0f));
}

void Application::InitSkyBox()
{
    m_skyBox.Init();
}

void Application::InitTerrain()
{
    lightingData.Init();

    float worldScale = 4.0f;
    float textureScale = 200.0f;

    std::vector<std::string> TextureFilenames;
    TextureFilenames.push_back("Textures/grass_path_2_diff_1k.jpg");
    TextureFilenames.push_back("Textures/IMGP5525_seamless.jpg");
    TextureFilenames.push_back("Textures/tilable-IMG_0044-verydark.png");
    TextureFilenames.push_back("Textures/water.png");

    /*TextureFilenames.push_back("Textures/R.png");
    TextureFilenames.push_back("Textures/G.png");
    TextureFilenames.push_back("Textures/B.png");
    TextureFilenames.push_back("Textures/A.png");*/
    TextureFilenames.push_back("Textures/checker.png");

    //m_terrain.LoadFromFile("data/heightmap.save");

    m_terrain0.InitTerrain(worldScale, textureScale, TextureFilenames);
    m_terrain0.CreateFaultFormation(m_terrain0.size, m_terrain0.iterration, m_terrain0.minHeight, m_terrain0.maxHeight, m_terrain0.filter);


    m_terrain1.InitTerrain(worldScale, textureScale, TextureFilenames);
    m_terrain1.CreateMidpoinDisplacement(m_terrain1.size, m_terrain1.roughness, m_terrain1.minHeight, m_terrain1.maxHeight);
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

void Application::WindowsResizeCallback(GLFWwindow* window, int width, int heigh)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    app->WindowsResize(width, heigh);
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

void Application::UpdateWireframeState()
{
    if (m_isWireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
