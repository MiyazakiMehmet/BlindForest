#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "GlobalValues.h"

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "PerlinNoise2D.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Model.h"

#include <iostream>
#include <vector>

int WIDTH = 1000, HEIGHT = 1200;

Window mainWindow;
Mesh planeMesh;
Shader planeShader;
DirectionalLight directionalLight;
PointLight pointLight[MAX_POINT_LIGHT];
SpotLight spotLight[MAX_SPOT_LIGHT];
Model treeModel;
Shader treeShader;
Material material;
Texture texture;

std::vector<float> planeVertices;
std::vector<unsigned int> planeIndices;
int gridSize = 50;
float size = 100.0f;


glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

glm::vec3 cameraPos = glm::vec3(0.0f, 3.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); //Where camera looks
glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRight, cameraUp;
float cameraSpeed = 2.5f;

bool firstMouse = true;
double previousX = WIDTH / 2;
double previousY = HEIGHT / 2;
float yaw = -90.0f;
float pitch = 0;

unsigned int perlinTex;
int texSize = 512;
std::vector<glm::vec3> treePositions;

glm::vec3 lightColor;
float ambientIntensity;
float diffuseIntensity;
glm::vec3 lightDirection;
float specularIntensity;
float shininess;
glm::vec3 pointLightPosition;
int pointLightCount = 0;
float constant;
float linear;
float exponent;
glm::vec3 spotLightPosition;
int spotLightCount = 0;

glm::vec3 treePos;

unsigned char* PerlinTexture(int& texSizeOut) {
	
	
	unsigned char* data = new unsigned char[texSize * texSize];

	for (int y = 0; y < texSize; ++y) {
		for (int x = 0; x < texSize; ++x) {
			float fx = (float)x / texSize * 10.0f;  // scale
			float fy = (float)y / texSize * 10.0f;

			float value = perlin(fx, fy); // -1 to 1
			value = (value + 1.0f) * 0.5f; // Normalize to 0..1

			data[y * texSize + x] = (unsigned char)(value * 255);
		}
	}

	glGenTextures(1, &perlinTex);
	glBindTexture(GL_TEXTURE_2D, perlinTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, texSize, texSize, 0, GL_RED, GL_UNSIGNED_BYTE, data);
	return data;
}

void TreeScatteringNoise() {

	unsigned char* noiseData = PerlinTexture(texSize);

	for (int x = 0; x < texSize; x += 4) {
		for (int y = 0; y < texSize; y += 4) {
			float noiseValue = (float)noiseData[y * texSize + x] / 255.0f;

			if (noiseValue > 0.6f) {
				float worldX = ((float)x / texSize - 0.5f) * 100.0f;
				float worldZ = ((float)y / texSize - 0.5f) * 100.0f;
				treePositions.push_back(glm::vec3(worldX, 0.0f, worldZ));
			}
		}
	}

	// Shuffle
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(treePositions.begin(), treePositions.end(), g);

	// Trim to 100
	if (treePositions.size() > 200){
		treePositions.resize(200);
	}

	std::cout << "Tree count: " << treePositions.size() << std::endl;

	delete[] noiseData; // Safe to delete after extracting positions
}

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
//Will be set just once
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

			// Texture coords (u, v)
			planeVertices.push_back((float)x / gridSize);
			planeVertices.push_back((float)y / gridSize);

			// Normal (facing up)
			planeVertices.push_back(0.0f);
			planeVertices.push_back(1.0f);
			planeVertices.push_back(0.0f);
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

	//Tree

	treeModel = Model();
	std::string treeModelPath = "src/models/Tree.obj";

	treeModel.LoadModel(treeModelPath);
}

void CreateShaders() {
	planeShader = Shader();
	planeShader.CompileShader("src/shaders/Plane.vert", "src/shaders/Plane.frag");

	treeShader = Shader();
	treeShader.CompileShader("src/shaders/Tree.vert", "src/shaders/Tree.frag");
}

void SetTransformations() {
	//Plane transformation
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
	//model = glm::rotate(model, glm::radians(15.0f), glm::vec3(2.0f, 0.0f, 0.0f));
	//model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));

	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));

	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	projection = glm::perspective(glm::radians(45.0f), (float)1400 / (float)1000, 0.1f, 100.0f);

	glUniformMatrix4fv(planeShader.GetModelLoc(), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(planeShader.GetViewLoc(), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(planeShader.GetProjectionLoc(), 1, GL_FALSE, glm::value_ptr(projection));

}

void LightningSetup() {
	//Directional Light
	lightColor = glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f));
	ambientIntensity = 0.2f;
	diffuseIntensity = 0.9f;
	lightDirection = glm::vec3(0.0f, -0.1f, -1.0f);
	specularIntensity = 1.0f;
	shininess = 12.0f;

	glm::vec3 pointLightColor = glm::vec3(0.0f, 1.0f, 1.0f);
	pointLightPosition = glm::vec3(5.0f, 4.0f, 0.0f);
	float pointLightAmbientIntensity = 0.2f;
	float pointLightDiffuseIntensity = 6.9f;
	constant = 1.0f;
	linear = 0.1f;
	exponent = 0.1f;
	pointLightCount++;

	glm::vec3 spotLightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	spotLightPosition = cameraPos;
	float spotLightAmbientIntensity = 0.6f;
	float spotLightDiffuseIntensity = 1.9f;
	constant = 1.3f;
	linear = 0.1f;
	exponent = 1.0f;
	glm::vec3 spotLightDirection = cameraFront;
	float edge = 20.0f;
	spotLightCount++;

	directionalLight = DirectionalLight(lightColor, ambientIntensity, diffuseIntensity, lightDirection);
	pointLight[0] = PointLight(pointLightColor, pointLightAmbientIntensity, pointLightDiffuseIntensity, pointLightPosition, constant, linear, exponent);
	spotLight[0] = SpotLight(spotLightColor, spotLightAmbientIntensity, spotLightDiffuseIntensity, spotLightPosition, constant, linear, exponent, spotLightDirection, edge);
	material = Material(specularIntensity, shininess);
}

void RenderScene() {
	
	//Update SpotLight
	spotLight[0].UpdateSpotLightPosition(cameraPos + glm::vec3(0.0f, 0.2f, 0.0f));
	spotLight[0].UpdateSpotLightDirection(cameraFront);

	planeShader.UseShader();

	//Get eye position every frame
	glUniform3f(planeShader.GetEyePosLoc(), cameraPos.x, cameraPos.y, cameraPos.z);

	//Perlin Texture
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, perlinTex);
	glUniform1i(glGetUniformLocation(planeShader.GetShaderID(), "perlinNoise"), 1);
	//Transform
	SetTransformations();
	//Lightning
	//planeShader.SetDirectionalLight(directionalLight);
	//planeShader.SetPointLight(pointLight, pointLightCount);
	planeShader.SetSpotLight(spotLight, spotLightCount);
	planeShader.SetMaterial(material);
	//Draw
	planeMesh.RenderMesh();

	//Tree
	treeShader.UseShader();

	glUniform1i(glGetUniformLocation(treeShader.GetShaderID(), "diffuseMap"), 0);
	//Get eye position every frame
	glUniform3f(treeShader.GetEyePosLoc(), cameraPos.x, cameraPos.y, cameraPos.z);

	//Lightning
	//treeShader.SetDirectionalLight(directionalLight);
	//treeShader.SetPointLight(pointLight, pointLightCount);
	treeShader.SetSpotLight(spotLight, spotLightCount);
	treeShader.SetMaterial(material);

	glActiveTexture(GL_TEXTURE0);

	for (const glm::vec3& pos : treePositions) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, pos);
		//model = glm::scale(model, glm::vec3(1.0f)); // optional scale to shrink trees
		glUniformMatrix4fv(treeShader.GetModelLoc(), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(treeShader.GetViewLoc(), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(treeShader.GetProjectionLoc(), 1, GL_FALSE, glm::value_ptr(projection));

		treeModel.RenderModel();
	}
}
//Will be set per frame
void RenderPass() {
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);  
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	RenderScene();
}

int main() {

	mainWindow = Window(WIDTH, HEIGHT, cameraSpeed);
	mainWindow.Initialize();

	//Handling yaw/pitch
	glfwSetInputMode(mainWindow.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(mainWindow.GetWindow(), HandleMouse);

	//Will be set just once
	CreateObjects();

	CreateShaders();

	PerlinTexture(texSize);

	TreeScatteringNoise();

	LightningSetup();

	while (!mainWindow.GetShouldClose()) {

		mainWindow.HandleKeys(cameraPos, cameraFront, cameraRight);
		
		RenderPass();
		
		mainWindow.SwapBuffers();
		glfwPollEvents();
	}

	return 0;
}