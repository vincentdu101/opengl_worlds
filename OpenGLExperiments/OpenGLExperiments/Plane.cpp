#include <GL/glew.h>
#include "Plane.h"

// https://www.youtube.com/watch?v=DJk-aTmrAlQ

GLfloat *generatePlaneVertices(int dimensions) {
	int numVertices = dimensions * dimensions;
	GLfloat almost[1000];
	int half = dimensions / 2;

	// coordinates for top flat surface of cube, use this to move it down in y and then 
	// clone it across a space based on the dimensions provided
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			GLfloat y = 1.0f;
			almost[i * dimensions + j] = j - half; 
			almost[i * dimensions + j + 1] = y; 
			almost[i * dimensions + j + 2] = i - half;
		}
	}
	/*1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f*/

	return almost;
}