#include <math.h>
#include <iostream>
#include <ctime>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "glm/glm.hpp"
#include "visuals.h"
#include "shader.hpp"

#define PI 3.14159265359

// This will be used with shader
//GLuint VertexArrayID;
GLuint vertexbuffer;
GLuint programID;

void ReshapeGL( int w, int h )
{
    std::cout << "ReshapGL( " << w << ", " << h << " );" << std::endl;

    // Prevent a divide-by-zero error
    h = (h == 0) ? 1 : h;

    g_iWindowWidth = w;
    g_iWindowHeight = h;

    glViewport( 0, 0, g_iWindowWidth, g_iWindowHeight );

    // Setup viewing volume
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //         L,      R,      B,     T,     N,      F
    glOrtho (-50.0f, 50.0f, -50.0f, 50.0f, 100.0f, -100.0f);
}

void KeyboardGL( unsigned char c, int x, int y )
{
    // Store the current scene so we can test if it has changed later.
    unsigned char currentScene = g_eCurrentScene;

    switch ( c )
    {
    case '1':
    {
        // White background
        glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
        g_eCurrentScene = 1;
    }
        break;
    case '2':
    {
        // Black background
        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f ); 
        g_eCurrentScene = 2;
    }
        break;
    case '3':
    {
        // Dark-Gray background
        glClearColor( 0.27f, 0.27f, 0.27f, 1.0f );
        g_eCurrentScene = 3;
    }
        break;
    case '4':
    {
        // Light-Gray background
        glClearColor( 0.4f, 0.4f, 0.4f, 1.0f );
        g_eCurrentScene = 4;
    }
        break;
    case '5':
    {
        // Light-Gray background
        glClearColor( 0.7f, 0.7f, 0.7f, 1.0f );
        g_eCurrentScene = 5;
    }
        break;
    case 's':
    case 'S':
    {
        std::cout << "Shade Model: GL_SMOOTH" << std::endl;
        // Switch to smooth shading model
        glShadeModel( GL_SMOOTH );
    }
        break;
    case 'f':
    case 'F':
    {
        std::cout << "Shade Model: GL_FLAT" << std::endl;
        // Switch to flat shading model
        glShadeModel( GL_FLAT );
    }
        break;
    case '\033': // escape quits
    case '\015': // Enter quits
    case 'Q':    // Q quits
    case 'q':    // q (or escape) quits
    {
        // Cleanup up and quit
        Cleanup(0);
    }
        break;
    case '\72': //arrow up
    {
    }
        break;
    }


    if ( currentScene != g_eCurrentScene )
    {
        std::cout << "Changed Render Scene: " << int( g_eCurrentScene ) << std::endl;
    }

    glutPostRedisplay();
}

void Cleanup( int errorCode, bool bExit )
{
    if ( g_iGLUTWindowHandle != 0 )
    {
        glutDestroyWindow( g_iGLUTWindowHandle );
        g_iGLUTWindowHandle = 0;
    }

    if ( bExit )
    {
        exit( errorCode );
    }
}


void DisplayGL()
{
    // Clean up the colour of the window
    // and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    switch ( g_eCurrentScene )
    {
    case 1:
    {
        RenderScene1();
    }
        break;
    case 2:
    {
        RenderScene2();
    }
        break;
    case 3:
    {
        RenderScene3();
    }
        break;
    case 4:
    {
        RenderScene4();
    }
        break;
    case 5:
    {
        RenderScene5();
    }
        break;
    }


    // All drawing commands applied to the
    // hidden buffer, so now, bring forward
    // the hidden buffer and hide the visible one
    glutSwapBuffers();
}

void RenderScene1()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set drawing colour = orange
    glColor3f(1.0, 0.5, 0.2);

    // Draw a built-in primitive
    glutSolidCube(20.0 );
}

void RenderScene2()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set drawing colour = orange
    glColor3f(1.0, 0.5, 0.2);
    glutWireTeapot(20.0);
}

void RenderScene3()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set drawing colour = orange
    glColor3f(1.0, 0.5, 0.2);
    glutSolidSphere(20.0,90,124);
}

void RenderScene4()
{
    // Switch to modelview matrix mode
    glMatrixMode( GL_MODELVIEW );
    
    // Load the identity matrix
    glLoadIdentity(); 

    // Translate back and to the left
    glTranslatef( -1.5f, 1.0f, -6.0f );
    
    // Draw a triangle with different colors on each vertex
    glBegin( GL_TRIANGLES );
    
    // Red
    glColor3f( 1.0f, 0.0f, 0.0f ); 
    
    // Top-Center
    glVertex2f( 0.0f, 1.0f ); 

    // Green
    glColor3f( 0.0f, 1.0f, 0.0f ); 

    // Bottom-Left
    glVertex2f( -1.0f, -1.0f ); 

    // Blue
    glColor3f( 0.0f, 0.0f, 1.0f ); 
    
    // Bottom-Right
    glVertex2f( 1.0f, -1.0f ); 
    
    glEnd();

    // Translate right
    glTranslatef( 3.0f, 0.0f, 0.0f );
    
    // Draw a rectangle with different colors on each vertex
    glBegin( GL_QUADS );
    // Red
    glColor3f( 1.0f, 0.0f, 0.0f ); 
    // Top-Left
    glVertex2f( -1.0f, 1.0f ); 

    // Green
    glColor3f( 0.0f, 1.0f, 0.0f ); 
    // Top-Right
    glVertex2f( 1.0f, 1.0f ); 

    // Blue
    glColor3f( 0.0f, 0.0f, 1.0f ); 
    // Bottom-Right
    glVertex2f( 1.0f, -1.0f ); 

    // White
    glColor3f( 1.0f, 1.0f, 1.0f ); 
    // Bottom-Left
    glVertex2f( -1.0f, -1.0f ); 
    glEnd();

    // Back to center and lower screen
    glTranslatef( -1.5f, -3.0f, 0.0f ); 

    // Draw a circle with blended red/blue vertices.
    const float step = PI / 16;
    const float radius = 1.0f;

    glBegin( GL_TRIANGLE_FAN );
    glColor3f( 1.0f, 1.0f, 1.0f );
    glVertex2f(0.0f, 0.0f);

    for ( float angle = 0.0f; angle < ( 2.0f * PI ); angle += step )
    {
        float fSin = sinf(angle);
        float fCos = cosf(angle);

        glColor3f( ( fCos + 1.0f ) * 0.5f, ( fSin + 1.0f ) * 0.5f , 0.0f);
        glVertex2f( radius * fSin, radius * fCos );
    }
    glColor3f( 1.0f, 0.5f, 0.0f );
    glVertex2f( 0.0f, radius ); // One more vertex to close the circle
    glEnd();
}

void RenderScene5()
{
    // Use our shader
    glUseProgram(programID);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
                0,          // attribute 0. No particular reason for 0, but must match the layout in the shader.
                3,          // size
                GL_FLOAT,   // type
                GL_FALSE,   // normalized?
                0,          // stride
                (void*)0    // array buffer offset
                );

    // Draw the triangle !
    // 3 indices starting at 0 -> 1 triangle
    glDrawArrays(GL_TRIANGLES, 0, 3); 

    glDisableVertexAttribArray(0);
}

void SetupGL()
{
    //Parameter handling
    glShadeModel (GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    // polygon rendering mode
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );

    //Set up light source
    GLfloat light_position[] = { 0.0, 30.0,-50.0,0.0 };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Black background
    glClearColor(0.0f,0.0f,0.0f,1.0f);

    // Register GLUT callbacks
    glutDisplayFunc(DisplayGL);
    glutKeyboardFunc(KeyboardGL);
    glutReshapeFunc(ReshapeGL);

    // Setup initial GL State
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glClearDepth( 1.0f );

    // Init GLEW
    if ( glewInit() != GLEW_OK )
    {
        std::cerr << "Failed to initialize GLEW." << std::endl;
        exit(-1);
    }

    // Setup initial GL State
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glClearDepth( 1.0f );

    std::cout << "Initialise OpenGL: Success!" << std::endl;

    // VAO
    // glGenVertexArrays(1, &VertexArrayID);
    // glBindVertexArray(VertexArrayID);
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );

    // VBO
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
    };

    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
}
