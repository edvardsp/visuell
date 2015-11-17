#pragma once
#include "glCheckerPiece.h"
#include <vector>

class KappaFileHandler
{
public:

	struct pieceMetaData
	{
		glm::vec2 pos;
		glm::vec3 color;
	};

	struct kappaFileContent
	{
		std::vector<pieceMetaData> piecesData;
		bool invertedBoard;
	};

	KappaFileHandler(std::string fileName, glm::mat4* system);
	void ChangeFile();
	kappaFileContent GetContent(int boardHeight, int boardWidth);
	

private:
	int mBoardWidth, mBoardHeight;
	glm::mat4 *mSystem;
	std::string mFileName;

	bool GetPieceDataFromLine(std::string line, pieceMetaData* data);
};
