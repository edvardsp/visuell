#pragma once
#include <glm.hpp>
#include <GL/glew.h>

class GlCheckerBoard
{
public:
	GlCheckerBoard(int width, int height, glm::vec3 position, bool inverted, glm::mat4* system);

	void SetDimensions(int width, int height) { mWidth = width; mHeight = height; }
	void Draw();

private:
	const GLfloat mThickness = 0.2f;
	int mWidth, mHeight;
	bool mInverted;
	GLuint mProgramID;
	GLuint mVertexBuffer, mColorBuffer;

	glm::vec3 mPosition;
	glm::mat4* mSystem;
	glm::mat4 mMvp;
};