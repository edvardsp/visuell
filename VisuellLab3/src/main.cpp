#include <math.h>
#include <iostream>
#include <ctime>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

#include "visuals.hpp"

#pragma comment(lib, "glew32.lib")
#define GLEW_STATIC

// Global variables
int g_iGLUTWindowHandle = 0;

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    //Setting up  The Display
    //   -RGB color model + Alpha Channel = GLUT_RGBA
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);

    // Configure Window Position
    int iScreenWidth = glutGet(GLUT_SCREEN_WIDTH);
    int iScreenHeight = glutGet(GLUT_SCREEN_HEIGHT);

    (void)iScreenWidth; (void)iScreenHeight;

    glutInitDisplayMode( GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH );

    glutInitWindowPosition( 0,0 );
    glutInitWindowSize( 800, 600 );

    g_iGLUTWindowHandle = glutCreateWindow( "OpenGL" );

    SetupGL();

    //Call to the drawing function
    glutDisplayFunc(DisplayGL);
    glutReshapeFunc(ReshapeGL);
    glutIdleFunc(Idle); 

    // Loop require by OpenGL
    glutMainLoop();
    return 0;
}


