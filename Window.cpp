#include "Window.h"

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "GLFW Starter Project";

bool Window::activated = false;
bool Window::actionSkybox = true;
bool Window::actionLightSource = false;
glm::vec3 Window::lastPoint;

bool Window::rotateCarousel = false;
bool Window::rotatePole = false;
bool Window::rotateCar = false;

Material* carouselMaterial;
Material* poleMaterial;
Material* carMaterial;

PointLight* Window::pointLight;
LightSource* Window::lightSource;

Geometry* Window::carousel;
Geometry* Window::pole[6];
Geometry* Window::car[6];

Transform* Window::carouselTransform;
Transform* Window::poleTransform[6];
Transform* Window::carTransform[6];

Cube* Window::skybox;
Sphere* Window::discoball;

GLfloat Window::normalColoring = 0.0;
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

// Skybox Shader Program ID
GLuint Window::skyboxShaderProgram;

bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

	// Check the shader program.
	if (!shaderProgram)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	// Create a skybox shader program with a vertex shader and a fragment shader.
	skyboxShaderProgram = LoadShaders("shaders/skybox.vert", "shaders/skybox.frag");

	// Check the shader program.
	if (!skyboxShaderProgram)
	{
		std::cerr << "Failed to initialize skybox shader program" << std::endl;
		return false;
	}

	return true;
}

bool Window::initializeObjects()
{
	pointSize = 30.0;

	carouselMaterial = new Material(glm::vec3(0.329412, 0.223529, 0.027451), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.992157, 0.941176, 0.807843), 0.21794872);
	poleMaterial = new Material(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 0.829, 0.829), glm::vec3(0.0, 0.0, 0.0), 0.088);
	carMaterial = new Material(glm::vec3(0.1, 0.18725, 0.1745), glm::vec3(0.396, 0.74151, 0.69102), glm::vec3(0.297254, 0.30829, 0.306678), 0.1);

	pointLight = new PointLight(glm::vec3(-14.5, -16.0, 0.0), glm::vec3(0.9, 0.9, 0.9), glm::vec3(-0.05, 0.9, 0.0));
	lightSource = new LightSource("sphere.obj", pointLight);

	// Set up carousel.
	carouselTransform = new Transform();
	carouselTransform->translate(glm::vec3(0.0, -4.0, 0.0));
	carousel = new Geometry("cone.obj", 3.0f, pointSize, normalColoring, carouselMaterial);
	carouselTransform->addChild(carousel);

	// Set up rides.
	for (unsigned i = 0; i < 6; i++) {
		poleTransform[i] = new Transform();
		glm::vec3 offset = glm::vec3(0, -2.0, 3.0);
		float radians = glm::radians(360.0f * ((i + 1.0f) / 6));
		glm::vec3 position = glm::vec3(offset.x * cos(radians) + offset.z * sin(radians), offset.y, offset.x * sin(radians) - offset.z * cos(radians));
		poleTransform[i]->translate(position);
		poleTransform[i]->rotate(90.0f, glm::vec3(1.0, 0.0, 0.0));
		poleTransform[i]->rotate(360.0f * ((i + 1.0f) / 6), glm::vec3(0.0, 1.0, 0.0));
		pole[i] = new Geometry("cylinder.obj", 2.0, pointSize, normalColoring, poleMaterial);
		poleTransform[i]->addChild(pole[i]);

		carTransform[i] = new Transform();
		car[i] = new Geometry("cube.obj", 2.0, pointSize, normalColoring, carMaterial);
		carTransform[i]->addChild(car[i]);

		poleTransform[i]->addChild(carTransform[i]);
		carouselTransform->addChild(poleTransform[i]);
	}

	skybox = new Cube(1000);
	discoball = new Sphere(eyePos);

	return true;
}

void Window::cleanUp()
{
	delete carouselMaterial;
	delete poleMaterial;
	delete carMaterial;

	delete pointLight;
	delete lightSource;

	delete carousel;
	for (unsigned i = 0; i < 6; i++) {
		delete pole[i];
		delete car[i];
	}

	delete carouselTransform;
	for (unsigned i = 0; i < 6; i++) {
		delete poleTransform[i];
		delete carTransform[i];
	}

	delete skybox;
	delete discoball;

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
	discoball->update();
	if (rotateCarousel)
		carouselTransform->rotate(0.0002f, glm::vec3(0.0, 1.0, 0.0));
	if (rotatePole) {
		for (unsigned i = 0; i < 6; i++) {
			poleTransform[i]->rotate(0.0002f, glm::vec3(0.0, 1.0, 0.0));
		}
	}
	if (rotateCar) {
		for (unsigned i = 0; i < 6; i++) {
			carTransform[i]->rotate(0.0002f, glm::vec3(0.0, 1.0, 0.0));
		}
	}
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	skybox->draw(view, projection, skyboxShaderProgram);
	glDisable(GL_CULL_FACE);

	discoball->draw(shaderProgram, projection, view, glm::mat4(1.0));
	lightSource->draw(shaderProgram, projection, view, glm::mat4(1.0));
	carouselTransform->draw(shaderProgram, projection, view, glm::mat4(1.0));

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

		// switch between the cube and the cube Geometry
		case GLFW_KEY_F1:
			actionSkybox = true;
			actionLightSource = false;
			break;
		case GLFW_KEY_F2:
			actionSkybox = false;
			actionLightSource = true;
			break;
		case GLFW_KEY_F3:
			actionSkybox = true;
			actionLightSource = true;
			break;
		case GLFW_KEY_1:
			rotateCarousel = !rotateCarousel;
			break;
		case GLFW_KEY_2:
			rotatePole = !rotatePole;
			break;
		case GLFW_KEY_3:
			rotateCar = !rotateCar;
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
			if (actionSkybox)
			{
				eyePos.y += direction.y;
				eyePos.z += direction.x;
				lookAtPoint.y += direction.y;
				lookAtPoint.z += direction.x;
				view = glm::lookAt(eyePos, lookAtPoint, upVector);
			}
			if (actionLightSource)
			{
				float rotAngle = velocity * 0.05;
				glm::vec3 rotAxis = glm::cross(lastPoint, currPoint);
				lightSource->orbit(direction, rotAngle, rotAxis);
			}
		}
		lastPoint = currPoint;
	}
}

void Window::scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	glMatrixMode(GL_PROJECTION);
	if (actionSkybox)
	{
		glm::mat4 mT = glm::translate(glm::mat4(1.0), -eyePos);
		glm::mat4 mR = glm::rotate(glm::mat4(1.0), glm::degrees(GLfloat(yOffset * 0.001)), glm::vec3(0.0, 1.0, 0.0));
		lookAtPoint = glm::vec3(inverse(mT) * mR * mT * glm::vec4(lookAtPoint, 1.0));
		view = glm::lookAt(eyePos, lookAtPoint, upVector);
	}
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
