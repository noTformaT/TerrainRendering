#pragma once

#include <stdint.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Terrain.h"

#include "Camera.h"

class Application
{
public:
	Application();
	~Application();

	void Init();
	void Run();
	void RenderScene();
	void RenderUI();
	void PassiveMouseCB(int32_t x, int32_t y);
	void KeyboardCB(uint32_t key, int32_t state);
	void MouseCB(int button, int action, int x, int y);

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
	BaseTerrain m_terrain;
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
};

