#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glProject.h"


#pragma comment(lib, "glew32.lib")
#define GLEW_STATIC

const int windowsWidth = 600;
const int windowsHeight = 400;

void PrintInputHelp()
{
	puts("Arguments:\n");
	puts("\t-h                  \tDisplays this.");
	puts("\t-f  <filepath>      \tKappa file.");
}

int argind = 1;
char *arg;

int getarg(int argc, char* argv[])
{
	arg = "";
	if (argind >= argc)
		return EOF;

	if (!(strlen(argv[argind]) == 2 && argv[argind][0] == '-'))
	{
		argind++;
		return '?';
	}

	// Check if last argument or next argument is an option
	if ((argind + 1) < argc && argv[argind + 1][0] != '-')
	{
		argind += 2;
		arg = argv[argind - 1];
		return argv[argind - 2][1];
	}

	argind++;
	return argv[argind - 1][1];
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	char c;
	std::string kappaFile = "";

	while ((c = getarg(argc, argv)) != EOF)
	{
		switch (c)
		{
		
		case 'f':
			kappaFile = arg;
			break;

		case 'h':
			PrintInputHelp();
			return 0;

		default:
			fprintf(stderr, "-%c is an invalid option, please use the -h option for help.\n", c);
			return 0;
		}
	}

	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);

	// Centering the window on the screen
	int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
	int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
	glutInitWindowPosition(screenWidth / 2 - windowsWidth / 2, screenHeight / 2 - windowsHeight / 2);
	glutInitWindowSize(windowsWidth, windowsHeight);
	
	// Setup program
	auto glProject = new GlProject(windowsWidth, windowsHeight, glutCreateWindow("CheckerBoard"), kappaFile);

	// If setup was successful, start main loop.
	if(glProject->SetupGL())
		glutMainLoop();

	return 0;
}