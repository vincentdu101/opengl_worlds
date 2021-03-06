#include <GLFW/glfw3.h>
// extern keyword here is to access the variable window declared in tutorial cpp
// this is a hack to keep the tutorials simple. Please avoid this. 
extern GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.h"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix() {
	return ViewMatrix;
}

glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}

// initial position: on +z
glm::vec3 position = glm::vec3(0, 0, 5);

// initial horizontal angle: toward -z
float horizontalAngle = 3.14f;

// initial vertical angle: none
float verticalAngle = 0.0f;

// initial field of view 
float initialFoV = 45.0f;

// 3 units/second
float speed = 3.0f;

float mouseSpeed = 0.0005f;

void computeMatricesFromInputs(int width, int height) {

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// compute the time difference between the current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// reset mouse position for next frame
	glfwSetCursorPos(window, width / 2, height / 2);

	// compute new orientation
	horizontalAngle += mouseSpeed * float(width / 2 - xpos);
	verticalAngle += mouseSpeed * float(height / 2 - ypos);

	// direction: spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f),
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);

	// up vector
	glm::vec3 up = glm::cross(right, direction);

	// move forward
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		position += direction * deltaTime * speed;
	}

	// move backward
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		position -= direction * deltaTime * speed;
	}

	// strafe right
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		position += right * deltaTime * speed;
	}

	// strafe left
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		position -= right * deltaTime * speed;
	}

	float FoV = initialFoV;

	// projection matrix: 45 degree field of view, 4:3 ratio, display range: 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);

	// camera matrix
	ViewMatrix = glm::lookAt(
		position,				// camera is here
		position + direction,	// and looks here, at the same position plus direction
		up						// head is up (set to 0, -1, 0 to look upside-down
	);

	// for the next frame, the last time will be now
	lastTime = currentTime;
}