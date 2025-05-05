#include "Window.h"

#include <GL/glew.h>


#include <iostream>


int WIDTH = 800, HEIGHT = 1000;

Window mainWindow;

float dirtPlaneVertices[];


void RenderScene() {

}

void RenderPass() {

}

int main() {

	mainWindow = Window(WIDTH, HEIGHT);
	mainWindow.Initialize();

	while (!mainWindow.GetShouldClose()) {


		
		mainWindow.SwapBuffers();
		glfwPollEvents();
	}

	return 0;
}