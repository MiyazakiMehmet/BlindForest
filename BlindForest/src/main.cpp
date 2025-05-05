#include <GL/glew.h>

#include "Window.h"
#include "Mesh.h"

#include <iostream>

int WIDTH = 800, HEIGHT = 1000;

Window mainWindow;
Mesh planeMesh;

float dirtPlaneVertices[];

void CreateObjects() {
	float planeVertices[] = {
		-0.5f, -0.5f, 0.0f,
		-0.5f,  5.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f
	};

	unsigned int planeIndices[] = {
		0, 1, 2,
		2, 3, 0
	};

	planeMesh.CompileMesh(planeVertices, planeIndices, 6);
}

void RenderScene() {
	planeMesh.RenderMesh();
}

void RenderPass() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	RenderScene();
}

int main() {

	mainWindow = Window(WIDTH, HEIGHT);
	mainWindow.Initialize();

	CreateObjects();
	while (!mainWindow.GetShouldClose()) {

		RenderPass();
		
		mainWindow.SwapBuffers();
		glfwPollEvents();
	}

	return 0;
}