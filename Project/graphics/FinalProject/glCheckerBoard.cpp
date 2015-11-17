#include "glCheckerBoard.h"
#include <gtc/matrix_transform.inl>
#include "bufferData.h"
#include "shader.h"

GlCheckerBoard::GlCheckerBoard(int width, int height, glm::vec3 position, bool inverted, glm::mat4* system) :
	mWidth(width),
	mHeight(height),
	mInverted(inverted),
	mPosition(position),
	mSystem(system)
{
	mProgramID = LoadShaders("TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");

	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);

	glGenBuffers(1, &mColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorBufferData), colorBufferData, GL_STATIC_DRAW);
}

void GlCheckerBoard::Draw()
{
	// Use our shader
	glUseProgram(mProgramID);

	// Enable buffers
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glm::mat4 Model;

	for (int i = 0; i < mWidth; i++)
	{
		for (int j = 0; j < mHeight; j++)
		{
			Model = glm::translate(glm::mat4(1.0f), glm::vec3(i+0.5, j+0.5, 0.1) + mPosition);
			//std::cout << glm::to_string(Model) << std::endl;
			Model = glm::scale(Model, glm::vec3(1, 1, 0.2));
			mMvp = *mSystem * Model;
			glUniformMatrix4fv(0, 1, GL_FALSE, &mMvp[0][0]);
			glDrawArrays(GL_TRIANGLES, ((i + j + mInverted) % 2 == 0) ? 12 * 3 : 0, 12 * 3);
		}
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
