#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>

static const GLfloat vertexBufferData[] = {
	0.5, -0.5, -0.5, // Start Red cube
	0.5, -0.5, 0.5,
	-0.5, -0.5, 0.5,
	0.5, -0.5, -0.5,
	-0.5, -0.5, 0.5,
	-0.5, -0.5, -0.5,
	0.5, 0.5, -0.5,
	-0.5, 0.5, -0.5,
	-0.5, 0.5, 0.5,
	0.5, 0.5, -0.5,
	-0.5, 0.5, 0.5,
	0.5, 0.5, 0.5,
	0.5, -0.5, -0.5,
	0.5, 0.5, -0.5,
	0.5, 0.5, 0.5,
	0.5, -0.5, -0.5,
	0.5, 0.5, 0.5,
	0.5, -0.5, 0.5,
	0.5, -0.5, 0.5,
	0.5, 0.5, 0.5,
	-0.5, 0.5, 0.5,
	0.5, -0.5, 0.5,
	-0.5, 0.5, 0.5,
	-0.5, -0.5, 0.5,
	-0.5, -0.5, 0.5,
	-0.5, 0.5, 0.5,
	-0.5, 0.5, -0.5,
	-0.5, -0.5, 0.5,
	-0.5, 0.5, -0.5,
	-0.5, -0.5, -0.5,
	0.5, 0.5, -0.5,
	0.5, -0.5, -0.5,
	-0.5, -0.5, -0.5,
	0.5, 0.5, -0.5,
	-0.5, -0.5, -0.5,
	-0.5, 0.5, -0.5, // End Red cube
	0.5, -0.5, -0.5, // Start blue cube
	0.5, -0.5, 0.5,
	-0.5, -0.5, 0.5,
	0.5, -0.5, -0.5,
	-0.5, -0.5, 0.5,
	-0.5, -0.5, -0.5,
	0.5, 0.5, -0.5,
	-0.5, 0.5, -0.5,
	-0.5, 0.5, 0.5,
	0.5, 0.5, -0.5,
	-0.5, 0.5, 0.5,
	0.5, 0.5, 0.5,
	0.5, -0.5, -0.5,
	0.5, 0.5, -0.5,
	0.5, 0.5, 0.5,
	0.5, -0.5, -0.5,
	0.5, 0.5, 0.5,
	0.5, -0.5, 0.5,
	0.5, -0.5, 0.5,
	0.5, 0.5, 0.5,
	-0.5, 0.5, 0.5,
	0.5, -0.5, 0.5,
	-0.5, 0.5, 0.5,
	-0.5, -0.5, 0.5,
	-0.5, -0.5, 0.5,
	-0.5, 0.5, 0.5,
	-0.5, 0.5, -0.5,
	-0.5, -0.5, 0.5,
	-0.5, 0.5, -0.5,
	-0.5, -0.5, -0.5,
	0.5, 0.5, -0.5,
	0.5, -0.5, -0.5,
	-0.5, -0.5, -0.5,
	0.5, 0.5, -0.5,
	-0.5, -0.5, -0.5,
	-0.5, 0.5, -0.5, // End blue cube
};

static const GLfloat colorBufferData[] = {
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.8f,  0.1f,  0.1f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f,
	0.1f,  0.1f,  0.8f
};