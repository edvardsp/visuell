#pragma once
#include "kappaFileHandler.h"
#include "glCheckerBoard.h"
#include <map>

class GlProject
{
public:
	GlProject(int windowWidth, int windowHeight, int windowHandler, std::string kappaFile = "");
	bool SetupGL();
	
private:
	static GlProject* mInstance;

	// Callback functions for GLUT
	void redraw(void);
	void reshapeGL(int width, int height);
	void mouseButton(int button, int state, int x, int y);
	void mouseMotion(int x, int y);
	void timerFunction(int lastTime);
	// Convenience functions
	void resetVariables();
	void resetAll();
	void resetBoard();
	void updatePieces(std::vector<KappaFileHandler::pieceMetaData> piecesData);
	void changeSelectedPiece(int key);
	void changeSpeed(glm::vec3 movement);
	void handleKeys(float dT);
	bool hasKeyChangedUpToDown(int keyId);
	void exit();

	int mMouseX, mMouseY;
	int mOldMouseX, mOldMouseY;
	int mMouseState, mMouseButton;
	std::map<int, short> mLastKeyState;

	int mBoardHeight, mBoardWidth;
	int mSelectedPieceIndex;
	glm::vec3 mSelectedPieceSpeed;
	bool mSelectedPieceSpeedChanged;
	
	int mWindowHandler;
	int mWindowWidth, mWindowHeight;
	float mViewDistance, mZoom;
	float mHorizontalAngle, mVerticalAngle;
	glm::mat4 mProjection, mView, mEverythingModel, mSystem;
	
	GlCheckerBoard* mCheckerBoard;
	std::vector<GlCheckerPiece*> mCheckerPieces;
	KappaFileHandler* mKappaFileHandler;

	// Hack for glut callbacks
	static void staticRedraw();
	static void staticReshape(int width, int height);
	static void staticMouseButton(int button, int state, int x, int y);
	static void staticMouseMotion(int x, int y);
	static void staticTimerFunction(int lastTime);
};
