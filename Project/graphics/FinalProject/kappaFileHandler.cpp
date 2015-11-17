#include <string>
#include <iostream>
#include <fstream>
#include <Windows.h>

#include "kappaFileHandler.h"


KappaFileHandler::KappaFileHandler(std::string fileName, glm::mat4* system):
	mFileName(fileName),
	mSystem(system)
{
}

void KappaFileHandler::ChangeFile()
{
	char tmpFileName[1024];
	OPENFILENAMEA ofn;
	HWND hwnd = nullptr;

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = tmpFileName;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(tmpFileName);
	ofn.lpstrFilter = "Kappa file\0*.kappa\0"; // Our own file ending
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = nullptr;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	// Display the Open dialog box. 
	if (GetOpenFileNameA(&ofn) == TRUE)
		mFileName = tmpFileName;
}

KappaFileHandler::kappaFileContent KappaFileHandler::GetContent(int boardHeight, int boardWidth)
{
	mBoardWidth = boardWidth;
	mBoardHeight = boardHeight;
	if (mFileName.empty())
		return kappaFileContent();

	std::ifstream file(mFileName);
	if (!file.is_open())
	{
		MessageBoxA(nullptr, ("The file:\n" + mFileName + "\n does not exist.").data(), "Error", MB_ICONERROR | MB_OK);
		mFileName.clear();
		return kappaFileContent();
	}

	std::vector<pieceMetaData> tmpPieces;
	std::string line;
	if (!getline(file, line))
	{
		MessageBoxA(nullptr, ("The file is corrupted:\n" + mFileName).data(), "Error", MB_ICONERROR | MB_OK);
		mFileName.clear();
		return kappaFileContent();
	}
	bool invertedBoard = line.compare("r") == 0;

	while (getline(file, line))
	{
		pieceMetaData pieceData;
		if (GetPieceDataFromLine(line, &pieceData))
			tmpPieces.push_back(pieceData);
	}

	return kappaFileContent{ tmpPieces, invertedBoard };
}

bool KappaFileHandler::GetPieceDataFromLine(std::string line, pieceMetaData* data)
{
	size_t firstDelim = line.find(",");
	if (firstDelim == std::string::npos) return false;
	GLfloat xPos = stof(line.substr(0, firstDelim));

	size_t secondDelim = line.find(",", firstDelim + 1);
	if (secondDelim == std::string::npos) return false;
	GLfloat yPos = stof(line.substr(firstDelim + 1, secondDelim));

	data->color = glm::vec3(0, 0, 0);
	auto colorString = line.substr(secondDelim + 1, line.length());
	if (!colorString.compare("r"))
		data->color = glm::vec3(0.7, 0.1, 0.1);
	else if (!colorString.compare("w"))
		data->color = glm::vec3(0.7, 0.7, 0.7);

	data->pos = glm::vec2(xPos, yPos);
	return true;
}
