#include "Window.h"

Window::Window()
{
	screenWidth = 800;
	screenHeight = 1000;
	currentFrame = 0.0f;
	lastFrame = 0.0f;
	deltaTime = 0.0f;
}

Window::Window(int width, int height, float camSpeed)
{
	screenWidth = width;
	screenHeight = height;
	cameraSpeed = camSpeed;
	currentFrame = 0.0f;
	lastFrame = 0.0f;
	deltaTime = 0.0f;
}

int Window::Initialize()
{
	//Initialize GLFW
	if (!glfwInit()) {
		std::cout << "Initialization has failed." << std::endl;
		glfwTerminate();

		return -1;
	}

	//Setup GLFW window properties
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Core Profile = No backwards compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(screenHeight, screenWidth, "Blind Forest", NULL, NULL);
	if (!mainWindow) {
		std::cout << "GLFW window creation failed." << std::endl;
		glfwTerminate();

		return -1;
	}

	glfwMakeContextCurrent(mainWindow);
	glViewport(0, 0, screenHeight, screenWidth);

	// Initialize GLEW
	glewExperimental = GL_TRUE; // Load OpenGL functions from GPU to use it in c++
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
}

void Window::HandleKeys(glm::vec3& cameraPos, glm::vec3& cameraFront, glm::vec3& cameraRight)
{
	currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	if (glfwGetKey(mainWindow, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += deltaTime * cameraSpeed * cameraFront;
	if (glfwGetKey(mainWindow, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= deltaTime * cameraSpeed * cameraFront;
	if (glfwGetKey(mainWindow, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= deltaTime * cameraRight * cameraSpeed;
	if (glfwGetKey(mainWindow, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += deltaTime * cameraRight * cameraSpeed;
}

Window::~Window()
{
	glfwTerminate();
}
