#include <gtx/associated_min_max.hpp>
#include "glCheckerPiece.h"
#include <gtc/matrix_transform.inl>
#include <GL/freeglut.h>

GlCheckerPiece::GlCheckerPiece(glm::vec3 position, glm::vec3 color, glm::mat4* system) :
	mPosition(position),
	mColor(color),
	mSystem(system),
	mSelected(false)
{
}

void GlCheckerPiece::Move(glm::vec3 change)
{
	mPosition += change;
}

void GlCheckerPiece::Draw()
{
	glUseProgram(0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	glColor3fv(&mColor[0]);
	auto Model = glm::translate(glm::mat4(1.0f), mPosition);
	mMvp = *mSystem * Model;
	glMultMatrixf(&mMvp[0][0]);

	glutSolidCylinder(mRadius, mHeight, mSlices, mStacks);
}

void GlCheckerPiece::Select()
{
	if (mSelected)
		return;

	mSelected = true;
	mRadius += 0.05f;
	mHeight += 0.05f;
	mColor += mSelectedColorModifier;
}

void GlCheckerPiece::Unselect()
{
	if (!mSelected)
		return;

	mSelected = false;
	mRadius -= 0.05f;
	mHeight -= 0.05f;
	mColor -= mSelectedColorModifier;
}
