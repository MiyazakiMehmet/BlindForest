#include <GL/glew.h>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"

#include <iostream>

int WIDTH = 800, HEIGHT = 1000;

Window mainWindow;
Mesh planeMesh;
Shader planeShader;

float dirtPlaneVertices[];

void CreateObjects() {
	float planeVertices[] = {
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f
	};

	unsigned int planeIndices[] = {
		0, 1, 2,
		2, 3, 0
	};

	planeMesh.CompileMesh(planeVertices, planeIndices, 6);
}

void CreateShaders() {
	planeShader = Shader();
	planeShader.CompileShader("src/shaders/Plane.vert", "src/shaders/Plane.frag");
}

void RenderScene() {
	planeMesh.RenderMesh();
}

void RenderPass() {
	glClearColor(0.4f, 0.6f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);


	planeShader.UseShader();

	RenderScene();
}

int main() {

	mainWindow = Window(WIDTH, HEIGHT);
	mainWindow.Initialize();

	CreateObjects();

	CreateShaders();


	while (!mainWindow.GetShouldClose()) {

		RenderPass();
		
		mainWindow.SwapBuffers();
		glfwPollEvents();
	}

	return 0;
}