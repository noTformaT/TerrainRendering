#pragma once

#include <stdint.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

	void RenderTest();

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void CursorPosCallback(GLFWwindow* window, double x, double y);
	static void MouseButtonCallback(GLFWwindow* window, int Button, int Action, int Mode);

private:
	GLFWwindow* window = nullptr;
	// TODO: Camera
};

