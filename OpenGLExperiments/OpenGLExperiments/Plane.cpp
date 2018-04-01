#include <GL/glew.h>
#include "Plane.h"

// https://www.youtube.com/watch?v=DJk-aTmrAlQ

GLfloat *generatePlaneVertices(int dimensions) {
	GLfloat almost[] = { 0.1, 0.1 };

	// coordinates for top flat surface of cube, use this to move it down in y and then 
	// clone it across a space based on the dimensions provided
	1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f

	return almost;
}