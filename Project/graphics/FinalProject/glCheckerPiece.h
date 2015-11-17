#pragma once
#include <glm.hpp>
#include <GL/glew.h>

class GlCheckerPiece
{
public:
	GlCheckerPiece(glm::vec3 position, glm::vec3 color ,glm::mat4* system);

	void Move(glm::vec3 change);
	void Draw();
	void Select();
	void Unselect();
	
private:
	const GLint mSlices = 90, mStacks = 120;
	const glm::vec3 mSelectedColorModifier = glm::vec3(0.1, 0.1, 0.1);
	GLfloat mRadius = 0.3f;
	GLfloat mHeight = 0.1f;

	glm::vec3 mPosition;
	glm::vec3 mColor;
	glm::mat4* mSystem;
	glm::mat4 mMvp;
	bool mSelected;
};
