#include "Window.h"

Window::Window()
{
	screenWidth = 600;
	screenHeight = 800;
}

Window::Window(int width, int height)
{
	screenWidth = width;
	screenHeight = height;
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
	glViewport(0, 0, screenWidth, screenHeight);

	// Initialize GLEW
	glewExperimental = GL_TRUE; // Load OpenGL functions from GPU to use it in c++
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
}

Window::~Window()
{
	glfwTerminate();
}
