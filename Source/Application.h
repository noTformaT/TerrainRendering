#pragma once

#include <stdint.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//#include "Terrain.h"
#include "FaultFormationTerrain.h"
#include "MidpointDisplacementTerrain.h"

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

	float h0 = 0.112f;
	float h1 = 0.183f;
	float h2 = 0.418f;
	float h3 = 0.536f;
	float h4 = 0.841f;
	float h5 = 0.902f;
private:
	void CreateWindow();
	void InitCallbacks();
	void InitCamera();
	void InitTerrain();

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void CursorPosCallback(GLFWwindow* window, double x, double y);
	static void MouseButtonCallback(GLFWwindow* window, int Button, int Action, int Mode);

	void UpdateMouse();
	GLfloat GetMouseXChange();
	GLfloat GetMouseYChange();

private:
	GLFWwindow* window = nullptr;
	FaultFormationTerrain m_terrain0;
	MidpointDisplacementTerrain m_terrain1;
	Camera m_pGameCamera;

	GLint bufferWidth;
	GLint bufferHeight;

	GLfloat deltaTime = .0f;
	GLfloat lastTime = .0f;

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

};

