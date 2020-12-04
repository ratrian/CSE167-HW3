#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "main.h"
#include "shader.h"
#include "LightSource.h"
#include "Transform.h"
#include "Geometry.h"
#include "Cube.h"
#include "Sphere.h"

class Window
{
public:

	// Window Properties
	static int width;
	static int height;
	static const char* windowTitle;

	static bool activated, actionSkybox, actionLightSource, carouselView, rotateCarousel, rotatePole, rotateCar;
	static glm::vec3 lastPoint;

	static PointLight* pointLight;
	static LightSource* lightSource;

	static Geometry* carousel;
	static Geometry* ground;
	static Geometry* pole[6];
	static Geometry* car[6];

	static Transform* carouselTransform;
	static Transform* groundTransform;
	static Transform* poleTransform[6];
	static Transform* carTransform[6];
	
	static Cube* skybox;
	static Sphere* discoball;

	static GLfloat normalColoring;

	// Camera Matrices
	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::vec3 eyePos, lookAtPoint, carouselPos, carouselLookAtPoint, upVector;

	// Shader Program ID
	static GLuint shaderProgram;
	static GLuint skyboxShaderProgram;

	// Constructors and Destructors
	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();

	// Window functions
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);

	// Draw and Update functions
	static void idleCallback();
	static void displayCallback(GLFWwindow*);

	// Callbacks
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);
	static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

	static glm::vec3 trackBallMapping(double xPos, double yPos);
};

#endif
