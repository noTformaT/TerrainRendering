#pragma once

#include <stdint.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//#include "Terrain.h"
#include "FaultFormationTerrain.h"
#include "MidpointDisplacementTerrain.h"
#include "LightingData.h"
#include "SkyBox.h"

#include "Camera.h"

class Application
{
public:
	Application();
	~Application();

	void Init();
	void Run();
	void RenderScene(float dt);
	void RenderUI(float dt);
	void PassiveMouseCB(int32_t x, int32_t y);
	void KeyboardCB(uint32_t key, int32_t state);
	void MouseCB(int button, int action, int x, int y);
	void WindowsResize(int width, int height);

	float h0 = 0.218f;
	float h1 = 0.249f;
	float h2 = 0.284f;
	float h3 = 0.386f;
	float h4 = 0.560f;
	float h5 = 0.684;
private:
	void CreateWindow();
	void InitCallbacks();
	void InitCamera(bool isRecalculate);
	void InitSkyBox();
	void InitTerrain();

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void CursorPosCallback(GLFWwindow* window, double x, double y);
	static void MouseButtonCallback(GLFWwindow* window, int Button, int Action, int Mode);
	static void WindowsResizeCallback(GLFWwindow* window, int width, int heigh);

	void UpdateMouse();
	GLfloat GetMouseXChange();
	GLfloat GetMouseYChange();

	void UpdateWireframeState();

private:
	GLFWwindow* window = nullptr;
	FaultFormationTerrain m_terrain0;
	MidpointDisplacementTerrain m_terrain1;
	Camera m_pGameCamera;
	SkyBox m_skyBox;


	GLint bufferWidth;
	GLint bufferHeight;

	GLfloat deltaTime = .0f;
	GLfloat lastTime = .0f;

	LightingData lightingData;

	// KeyBoard
private:
	bool keys[1024];

	GLfloat lastX = .0f;
	GLfloat lastY = .0f;
	GLfloat xChange = .0f;
	GLfloat yChange = .0f;
	bool mouseFirstMoved = true;
	bool m_isWireframe = false;
	bool m_isToogleInput = false;

	int m_terrainIndex = 0;

	bool m_isGeoMappingRender = false;

};

