#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm.hpp>
#include <ext.hpp>
#include <gtc/matrix_transform.hpp>

#include "glProject.h"
#include "glCheckerBoard.h"

// World view defines
#define MOUSE_SPEED 0.5f
#define ZOOM_SCROLL_SPEED 0.2f
#define DEFAULT_VIEW_DISTANCE 7.0f

// Piece animation defines
#define PIECE_KEY_SPEED 10.0f
#define PIECE_MAX_SPEED 5.0f
#define PIECE_SPEED_DECREASE_MULTI 20.0f
#define PIECE_CONST_SPEED_DECREASE 1.0f
#define PIECE_MIN_SPEED 0.001f
#define PIECE_MOUSE_MOVEMENT_MULTIPLIER 0.01385f

#define PI 3.14f

#define DEFAULT_BOARD_HEIGHT 5
#define DEFAULT_BOARD_WIDTH 8
#define TIMER_FUNCTION_PERIOD_MS 15

#define between(x,y,z) ((x < y) && (y < z))
#define minLen(x,y) ((glm::length(x)<glm::length(y))?x:y)

GlProject* GlProject::mInstance = nullptr;

GlProject::GlProject(int windowWidth, int windowHeight, int windowHandler, std::string kappaFile):
	mOldMouseX(0),
	mOldMouseY(0),
	mSelectedPieceIndex(-1),
	mSelectedPieceSpeedChanged(false),
	mWindowHandler(windowHandler),
	mWindowWidth(windowWidth), 
	mWindowHeight(windowHeight),
	mViewDistance(DEFAULT_VIEW_DISTANCE),
	mCheckerBoard(nullptr)
{
	mKappaFileHandler = new KappaFileHandler(kappaFile, &mSystem);
	resetVariables();
}

bool GlProject::SetupGL()
{
	if (mInstance)
		return false;
	mInstance = this;

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	// Setup material
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set up light source
	glLightfv(GL_LIGHT0, GL_POSITION, &glm::vec4(5,5,5.0,0.0)[0]);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Register GLUT callbacks
	glutDisplayFunc(staticRedraw);
	glutReshapeFunc(staticReshape);
	glutMouseFunc(staticMouseButton);
	glutMotionFunc(staticMouseMotion);
	glutTimerFunc(TIMER_FUNCTION_PERIOD_MS,staticTimerFunction, glutGet(GLUT_ELAPSED_TIME));

	// Init GLEW
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW." << std::endl;
		return false;
	}

	resetBoard();

	return true;
}

void GlProject::redraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mEverythingModel = glm::rotate(glm::mat4(1.0f), mHorizontalAngle, glm::vec3(0, 1, 0));
	mEverythingModel = glm::rotate(mEverythingModel, mVerticalAngle, glm::vec3(1, 0, 0));
	mEverythingModel = glm::translate(mEverythingModel, glm::vec3(-float(mBoardWidth) / 2.0, -float(mBoardHeight) / 2.0, 0));

	mView = glm::lookAt(
		glm::vec3(0, 0, mViewDistance + mZoom), // Camera is at position, in World Space
		glm::vec3(0, 0, 0),  // looks at origo
		glm::vec3(0, 1, 0)   // Head is up
		);

	mSystem = mProjection * mView * mEverythingModel;

	// Draw all elements
	mCheckerBoard->Draw();
	for (auto piece : mCheckerPieces)
		piece->Draw();

	glutSwapBuffers();
}

void GlProject::reshapeGL(int width, int height)
{
	// Prevent divide by zero
	if (height == 0)
		height = 1;

	mWindowWidth = width;
	mWindowHeight = height;

	// Update viewport
	glViewport(0, 0, width, height);

	// To prevent a lot of zooming when making the window higher.
	mViewDistance += (height - mWindowHeight) / 80.0f;
	// Update new aspect ratio
	mProjection = glm::perspective(45.0f, float(width) / float(height), 0.01f, 1000.0f);
}

void GlProject::mouseButton(int button, int state, int x, int y)
{
	mMouseState = state;
	mMouseButton = button;
	mMouseX = x;
	mMouseY = y;

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		mOldMouseX = x, mOldMouseY = y;
		break;
	case GLUT_RIGHT_BUTTON:
		glutWarpPointer(mWindowWidth / 2, mWindowHeight / 2);
		break;

	case 3: // Scroll up
	case 4: // Scroll down
		mZoom += (button == 4) ? ZOOM_SCROLL_SPEED : -ZOOM_SCROLL_SPEED;
		glutPostRedisplay();
		break;
	}
}

void GlProject::mouseMotion(int x, int y)
{
	mMouseX = x;
	mMouseY = y;
}

void GlProject::timerFunction(int lastTime)
{
	int currentTime = glutGet(GLUT_ELAPSED_TIME);

	float dT = float(currentTime - lastTime) / 1000.0f;

	mSelectedPieceSpeedChanged = false;

	handleKeys(dT);

	if (mMouseState == GLUT_DOWN && mMouseButton == GLUT_RIGHT_BUTTON)
	{
		mHorizontalAngle -= dT * MOUSE_SPEED * float(mWindowWidth / 2 - mMouseX);
		mVerticalAngle -= dT * MOUSE_SPEED * float(mWindowHeight / 2 - mMouseY);
		glutPostRedisplay();
	}

	if (mMouseState == GLUT_DOWN && mMouseButton == GLUT_LEFT_BUTTON &&
		between(-1, mSelectedPieceIndex, int(mCheckerPieces.size())))
	{
		auto movement = glm::vec3((mMouseX - mOldMouseX), -(mMouseY - mOldMouseY), 0) * PIECE_MOUSE_MOVEMENT_MULTIPLIER;
		mCheckerPieces[mSelectedPieceIndex]->Move(movement);
		mOldMouseX = mMouseX, mOldMouseY = mMouseY;
		glutPostRedisplay();
	}

	// Reduce the speed, improves steering 
	if (glm::length(mSelectedPieceSpeed) > PIECE_MIN_SPEED)
	{
		glm::vec3 changeSpeed = minLen(mSelectedPieceSpeed*(PIECE_MAX_SPEED / glm::length(mSelectedPieceSpeed))*PIECE_CONST_SPEED_DECREASE*dT, mSelectedPieceSpeed);
		mSelectedPieceSpeed -= changeSpeed;
	}

	if (glm::length(mSelectedPieceSpeed) > PIECE_MIN_SPEED && !mSelectedPieceSpeedChanged)
	{
		glm::vec3 changeSpeed = minLen(mSelectedPieceSpeed*(PIECE_MAX_SPEED / glm::length(mSelectedPieceSpeed))*PIECE_CONST_SPEED_DECREASE*dT, mSelectedPieceSpeed);
		mSelectedPieceSpeed -= changeSpeed;
	}

	// If Piece has speed, move it
	if (glm::length(mSelectedPieceSpeed) > PIECE_MIN_SPEED)
	{
		if (between(-1, mSelectedPieceIndex, int(mCheckerPieces.size())))
		{
			mCheckerPieces[mSelectedPieceIndex]->Move(dT*mSelectedPieceSpeed);
			glutPostRedisplay();
		}
	}

	glutTimerFunc(TIMER_FUNCTION_PERIOD_MS, staticTimerFunction, currentTime);
}

void GlProject::resetVariables()
{
	mBoardHeight = DEFAULT_BOARD_HEIGHT;
	mBoardWidth = DEFAULT_BOARD_WIDTH;
	mHorizontalAngle = 0.0f;
	mVerticalAngle = PI;
	mMouseX = mWindowWidth / 2;
	mMouseY = mWindowHeight / 2;
	mSelectedPieceIndex = -1;
	mZoom = 0.0f;
	mSelectedPieceSpeed = glm::vec3(0, 0, 0);

	// mProjection matrix : 45° Field of mView, 4:3 ratio, display range : 0.1 unit <-> 100 units
	mProjection = glm::perspective(45.0f, float(mWindowWidth) / float(mWindowHeight), 0.01f, 1000.0f);
}

void GlProject::resetAll()
{
	resetVariables();

	resetBoard();
}

void GlProject::resetBoard()
{
	if (mCheckerBoard)
		delete(mCheckerBoard);
	std::string cornerColor;

	auto fileContent = mKappaFileHandler->GetContent(mBoardHeight, mBoardWidth);

	mCheckerBoard = new GlCheckerBoard(mBoardWidth, mBoardHeight, glm::vec3(0, 0, 0), fileContent.invertedBoard, &mSystem);
	updatePieces(fileContent.piecesData);

	glutPostRedisplay();
}

void GlProject::updatePieces(std::vector<KappaFileHandler::pieceMetaData> piecesData)
{
	for (auto piece : mCheckerPieces)
		delete piece;
	mCheckerPieces.clear();

	for (auto pieceData : piecesData)
	{
		mCheckerPieces.push_back(new GlCheckerPiece(
			glm::vec3(pieceData.pos[0] * mBoardWidth, (1.0 - pieceData.pos[1]) * mBoardHeight, 0.2),
			pieceData.color,
			&mSystem));
	}
}

void GlProject::changeSelectedPiece(int indexChange)
{
	if (between(-1, mSelectedPieceIndex, int(mCheckerPieces.size())))
		mCheckerPieces[mSelectedPieceIndex]->Unselect();

	mSelectedPieceIndex += indexChange;

	// Check if within limits, and adjust if necessary
	if (mSelectedPieceIndex < -1)
		mSelectedPieceIndex = int(mCheckerPieces.size()) - 1;
	if (mSelectedPieceIndex > int(mCheckerPieces.size()))
		mSelectedPieceIndex = 0;

	if (between(-1, mSelectedPieceIndex, int(mCheckerPieces.size())))
		mCheckerPieces[mSelectedPieceIndex]->Select();
}

void GlProject::changeSpeed(glm::vec3 movement)
{
	auto nextSpeed = mSelectedPieceSpeed + movement;
	
	if (glm::length(nextSpeed) > PIECE_MAX_SPEED)
		nextSpeed *= PIECE_MAX_SPEED / glm::length(nextSpeed);

	mSelectedPieceSpeed = nextSpeed;
	mSelectedPieceSpeedChanged = true;
}

void GlProject::handleKeys(float dT)
{
	if (GetAsyncKeyState('W') || GetAsyncKeyState(VK_UP))
	{
		changeSpeed(glm::vec3(0, PIECE_KEY_SPEED, 0)*dT);
	}
	
	if (GetAsyncKeyState('S') || GetAsyncKeyState(VK_DOWN))
	{
		changeSpeed(glm::vec3(0, -PIECE_KEY_SPEED, 0)*dT);
	}

	if (GetAsyncKeyState('A') || GetAsyncKeyState(VK_LEFT))
	{
		changeSpeed(glm::vec3(-PIECE_KEY_SPEED, 0, 0)*dT);
	}

	if (GetAsyncKeyState('D') || GetAsyncKeyState(VK_RIGHT))
	{
		changeSpeed(glm::vec3(PIECE_KEY_SPEED, 0, 0)*dT);
	}

	if (hasKeyChangedUpToDown('O'))
	{
		mKappaFileHandler->ChangeFile();
		resetBoard();
	}

	if (hasKeyChangedUpToDown('R'))
	{
		resetAll();
	}

	if (hasKeyChangedUpToDown(VK_RSHIFT))
	{
		changeSelectedPiece(1);
		glutPostRedisplay();
	}

	if (hasKeyChangedUpToDown(VK_RCONTROL))
	{
		changeSelectedPiece(-1);
		glutPostRedisplay();
	}

	if (hasKeyChangedUpToDown('Q'))
	{
		exit();
	}
}

bool GlProject::hasKeyChangedUpToDown(int keyId)
{
	short keyState = GetAsyncKeyState(keyId);
	short lastState = mLastKeyState[keyId];
	mLastKeyState[keyId] = keyState;
	return keyState && !lastState;
}

void GlProject::exit()
{
	glutDestroyWindow(mWindowHandler);
	mWindowHandler = 0;
	mInstance = nullptr;
}

void GlProject::staticRedraw()
{
	if (mInstance)
		mInstance->redraw();
}

void GlProject::staticReshape(int width, int height)
{
	if (mInstance)
		mInstance->reshapeGL(width, height);
}

void GlProject::staticMouseButton(int button, int state, int x, int y)
{
	if (mInstance)
		mInstance->mouseButton(button, state, x, y);
}

void GlProject::staticMouseMotion(int x, int y)
{
	if (mInstance)
		mInstance->mouseMotion(x,y);
}

void GlProject::staticTimerFunction(int lastTime)
{
	if (mInstance)
		mInstance->timerFunction(lastTime);
}
