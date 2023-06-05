#pragma once

#include <stdint.h>

class Application
{
public:
	Application();
	~Application();

	void Init();
	void Run();
	void RenderScene();
	void PassiveMouseCB(int32_t x, int32_t y);
	void KeyboardCB(uint32_t key, int32_t state);
	void MouseCB();

private:
	void CreateWindow();
	void InitCallbacks();
	void InitCamera();
	void InitTerrain();
	//static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	//static void CursorPosCallback(GLFWwindow* window, double x, double y);
	//static void MouseButtonCallback(GLFWwindow* window, int Button, int Action, int Mode)


};

