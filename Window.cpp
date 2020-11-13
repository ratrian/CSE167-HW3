#include "Window.h"

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "GLFW Starter Project";

bool Window::activated = false;
bool Window::actionObject = true;
bool Window::actionLightSource = false;
glm::vec3 Window::lastPoint;

Material* bunnyPointsMaterial;
Material* sandalPointsMaterial;
Material* bearPointsMaterial;

PointLight* Window::pointLight;
LightSource* Window::lightSource;

// Objects to Render
PointCloud* Window::bunnyPoints;
PointCloud* Window::sandalPoints;
PointCloud* Window::bearPoints;
PointCloud* currPointCloud;

GLfloat Window::normalColoring = 1.0;
GLfloat pointSize;

// Camera Matrices
// Projection matrix:
glm::mat4 Window::projection; 

// View Matrix:
glm::vec3 Window::eyePos(0, 0, 20);			// Camera position.
glm::vec3 Window::lookAtPoint(0, 0, 0);		// The point we are looking at.
glm::vec3 Window::upVector(0, 1, 0);		// The up direction of the camera.
glm::mat4 Window::view = glm::lookAt(Window::eyePos, Window::lookAtPoint, Window::upVector);

// Shader Program ID
GLuint Window::shaderProgram;

bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

	// Check the shader program.
	if (!shaderProgram)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	return true;
}

bool Window::initializeObjects()
{
	pointSize = 30.0;

	bunnyPointsMaterial = new Material(glm::vec3(0.329412, 0.223529, 0.027451), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.992157, 0.941176, 0.807843), 0.21794872);
	sandalPointsMaterial = new Material(glm::vec3(0.25, 0.20725, 0.20725), glm::vec3(1.0, 0.829, 0.829), glm::vec3(0.0, 0.0, 0.0), 0.088);
	bearPointsMaterial = new Material(glm::vec3(0.19225, 0.19225, 0.19225), glm::vec3(0.50754, 0.50754, 0.50754), glm::vec3(0.508273, 0.508273, 0.508273), 0.4);

	pointLight = new PointLight(glm::vec3(6.0, 6.0, 6.0), glm::vec3(0.7, 0.7, 0.7), glm::vec3(-0.05, 0.9, 0.0));
	lightSource = new LightSource("sphere.obj", pointLight);

	// Create point clouds consisting of objects vertices.
	bunnyPoints = new PointCloud("bunny.obj", pointSize, normalColoring, bunnyPointsMaterial);
	sandalPoints = new PointCloud("sandal.obj", pointSize, normalColoring, sandalPointsMaterial);
	bearPoints = new PointCloud("bear.obj", pointSize, normalColoring, bearPointsMaterial);

	// Set bunny to be the first to display
	currPointCloud = bunnyPoints;

	return true;
}

void Window::cleanUp()
{
	delete bunnyPointsMaterial;
	delete sandalPointsMaterial;
	delete bearPointsMaterial;

	delete pointLight;
	delete lightSource;

	// Deallcoate the objects.
	delete bunnyPoints;
	delete sandalPoints;
	delete bearPoints;

	// Delete the shader program.
	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(60.0), 
								double(width) / (double)height, 1.0, 1000.0);
}

void Window::idleCallback()
{
	// Perform any necessary updates here 
	currPointCloud->update();
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// Render the objects
	currPointCloud->draw(view, projection, shaderProgram);

	lightSource->draw(view, projection, shaderProgram);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Modify below to add your key callbacks.
	 */
	
	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);				
			break;

		// switch between the cube and the cube pointCloud
		case GLFW_KEY_F1:
			currPointCloud = bunnyPoints;
			break;
		case GLFW_KEY_F2:
			currPointCloud = sandalPoints;
			break;
		case GLFW_KEY_F3:
			currPointCloud = bearPoints;
			break;
		case GLFW_KEY_S:
			pointSize = pointSize / 2;
			currPointCloud->updatePointSize(pointSize);
			break;
		case GLFW_KEY_L:
			pointSize = pointSize * 2;
			currPointCloud->updatePointSize(pointSize);
			break;
		case GLFW_KEY_N:
			if (normalColoring == 0.0) {
				normalColoring = 1.0;
			}
			else if (normalColoring == 1.0) {
				normalColoring = 0.0;
			}
			currPointCloud->updateNormalColoring(normalColoring);
			break;
		case GLFW_KEY_1:
			actionObject = true;
			actionLightSource = false;
			break;
		case GLFW_KEY_2:
			actionObject = false;
			actionLightSource = true;
			break;
		case GLFW_KEY_3:
			actionObject = true;
			actionLightSource = true;
			break;

		default:
			break;
		}
	}
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		activated = true;

		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		lastPoint = trackBallMapping(xPos, yPos);

		glMatrixMode(GL_MODELVIEW);
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		activated = false;
}

void Window::cursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (activated)
	{
		glm::vec3 currPoint = trackBallMapping(xPos, yPos);
		glm::vec3 direction = currPoint - lastPoint;
		float velocity = glm::length(direction);
		if (velocity > 0.0001)
		{
			float rotAngle = velocity * 0.05;
			glm::vec3 rotAxis = glm::cross(lastPoint, currPoint);
			if (actionObject)
				currPointCloud->spin(rotAngle, rotAxis);
			if (actionLightSource)
				lightSource->orbit(direction, rotAngle, rotAxis);
		}
		lastPoint = currPoint;
	}
}

void Window::scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	glMatrixMode(GL_PROJECTION);
	if (actionObject)
		currPointCloud->zoom(glm::vec3(1.0 + yOffset * 0.01));
	if (actionLightSource)
		lightSource->move(glm::vec3(yOffset * 0.01));
}

glm::vec3 Window::trackBallMapping(double xPos, double yPos)
{
	glm::vec3 v;
	float d;
	v.x = (2.0 * xPos - height) / height;
	v.y = (width - 2.0 * yPos) / width;
	v.z = 0.0;
	d = glm::length(v);
	d = (d < 1.0) ? d : 1.0;
	v.z = sqrtf(1.001 - d * d);
	glm::normalize(v);
	return v;
}
