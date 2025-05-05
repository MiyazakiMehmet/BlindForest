#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

class Window {
private:
	GLFWwindow* mainWindow;
	int screenWidth, screenHeight;
public:
	Window();
	Window(int width, int height);

	int Initialize();

	bool GetShouldClose() { return glfwWindowShouldClose(mainWindow); }

	void SwapBuffers() { return glfwSwapBuffers(mainWindow); }

	~Window();
};