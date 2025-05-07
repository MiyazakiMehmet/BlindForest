#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <iostream>

class Window {
private:
	GLFWwindow* mainWindow;
	int screenWidth, screenHeight;
	float currentFrame, lastFrame, deltaTime, cameraSpeed;
public:
	Window();
	Window(int width, int height, float camSpeed);

	int Initialize();

	bool GetShouldClose() { return glfwWindowShouldClose(mainWindow); }

	void SwapBuffers() { return glfwSwapBuffers(mainWindow); }

	void HandleKeys(glm::vec3& cameraPos, glm::vec3& cameraFront, glm::vec3& cameraRight);

	~Window();
};