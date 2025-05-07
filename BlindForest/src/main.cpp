#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"

#include <iostream>
#include <vector>

int WIDTH = 800, HEIGHT = 1000;

Window mainWindow;
Mesh planeMesh;
Shader planeShader;

std::vector<float> planeVertices;
std::vector<unsigned int> planeIndices;
int gridSize = 50;
float size = 10.0f;

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

glm::vec3 cameraPos = glm::vec3(0.0f, 3.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); //Where camera lookss
glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRight, cameraUp;
float cameraSpeed = 2.5f;

bool firstMouse = true;
double previousX = WIDTH / 2;
double previousY = HEIGHT / 2;
float yaw = -90.0f;
float pitch = 0;


void HandleMouse(GLFWwindow* window, double xPos, double yPos) {
	if (firstMouse) {
		previousX = xPos;
		previousY = yPos;
		firstMouse = false;
	}

	float xOffset = xPos - previousX;
	float yOffset = previousY - yPos;

	previousX = xPos;
	previousY = yPos;

	float sensivity = 0.1f;
	xOffset *= sensivity;
	yOffset *= sensivity;

	yaw += xOffset;
	pitch += yOffset;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	else if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	glm::vec3 direction;
	direction.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	direction.y = glm::sin(glm::radians(pitch));
	direction.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));

	cameraFront = glm::normalize(direction);
	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}

void CreateObjects() {
	//Create indices for plane
	for (int y = 0; y <= gridSize; y++) {
		for (int x = 0; x <= gridSize; x++) {
			float xPos = ((float)x / gridSize) * size - size / 2.0f;
			float yPos = 0.0f;
			float zPos = ((float)y / gridSize) * size - size / 2.0f;

			planeVertices.push_back(xPos);
			planeVertices.push_back(yPos);
			planeVertices.push_back(zPos);
		}
	}

	//Create indices for plane
	for (int y = 0; y < gridSize; y++) {
		for (int x = 0; x < gridSize; x++) {
			unsigned int i0 = y * (gridSize + 1) + x;
			unsigned int i1 = i0 + 1;
			unsigned int i2 = i0 + (gridSize + 1);
			unsigned int i3 = i2 + 1;

			//First Triangle
			planeIndices.push_back(i0);
			planeIndices.push_back(i2);
			planeIndices.push_back(i1);
			//Second Triangle
			planeIndices.push_back(i2);
			planeIndices.push_back(i3);
			planeIndices.push_back(i1);
		}
	}

	planeMesh.CompileMesh(planeVertices, planeIndices);
}

void CreateShaders() {
	planeShader = Shader();
	planeShader.CompileShader("src/shaders/Plane.vert", "src/shaders/Plane.frag");
}

void SetTransformations() {
	//Plane transformation
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
	//model = glm::rotate(model, glm::radians(15.0f), glm::vec3(2.0f, 2.0f, 2.0f));
	//model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));

	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));

	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	projection = glm::perspective(glm::radians(45.0f), (float)1400 / (float)1000, 0.1f, 100.0f);

	glUniformMatrix4fv(planeShader.GetModelLoc(), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(planeShader.GetViewLoc(), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(planeShader.GetProjectionLoc(), 1, GL_FALSE, glm::value_ptr(projection));

}

void RenderScene() {
	
	SetTransformations();

	planeMesh.RenderMesh();
}

void RenderPass() {
	glClearColor(0.4f, 0.6f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);


	planeShader.UseShader();

	RenderScene();
}

int main() {

	mainWindow = Window(WIDTH, HEIGHT, cameraSpeed);
	mainWindow.Initialize();

	//Handling yaw/pitch
	glfwSetInputMode(mainWindow.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(mainWindow.GetWindow(), HandleMouse);

	CreateObjects();

	CreateShaders();

	while (!mainWindow.GetShouldClose()) {

		mainWindow.HandleKeys(cameraPos, cameraFront, cameraRight);
		
		RenderPass();


		
		mainWindow.SwapBuffers();
		glfwPollEvents();
	}

	return 0;
}