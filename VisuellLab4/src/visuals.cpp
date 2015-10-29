/***************************************************************
*   Includes
***************************************************************/

#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "visuals.hpp"

/***************************************************************
*   Defines
***************************************************************/

#define PI 3.14159265358979323846f

#define mouseSpeed 0.00005f

// units per second
#define cursor_speed 0.004f

/***************************************************************
*   Variables
***************************************************************/

// This will be used with shader
// GLuint VertexArrayID
GLuint box_vert_buf, box_color_buf;
GLuint leg_vert_buf, leg_color_buf;
GLuint yellow_color_buf, red_color_buf;
GLuint blue_color_buf, green_color_buf;

// for obj
GLuint objvertexbuffer;
GLuint programID_1, programID_2;

int last_time, current_time;

GLint MatrixID; // Handler Matrix for moving the cam
glm::mat4 MVP; // Final Homogeneous Matrix

glm::mat4 MVP1, MVP2, MVP3, MVP4, MVP5;
glm::mat4 MODEL_EVERYTHING, MODEL_LEG_1, MODEL_LEG_2;
glm::mat4 MODEL_FOOT_1, MODEL_FOOT_2;
glm::mat4 Projection, View, Model;

// Variables for moving camera with mouse
int mouse_x = 800/2;
int mouse_y = 600/2;
int mouse_button =  GLUT_LEFT_BUTTON;
int mouse_state = GLUT_UP;
int sp_key = 0;

float counter = 0.0f;
float rotateAngle = 0.0f;

// Initial position : on +Z
glm::vec3 start_pos = glm::vec3(5.63757f, 1.7351f, -2.19067f);
glm::vec3 position = start_pos;

// Initial horizontal angle : toward -Z
float horizontalAngle = -1.07947f;

// Initial vertical angle : none
float verticalAngle = -0.369055f;

// Initial Field of View
float initialFoV = 45.0f;
glm::vec3 direction =  glm::vec3(0.0f, 0.0f, 0.0f) - position;
glm::vec3 right  = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

/***************************************************************
*   Functions
***************************************************************/

void MouseGL(int button, int state, int x, int y)
{
    // if key is pressed
    if (state == GLUT_DOWN)
    {
        // save in global memomry
        mouse_state = state;

        // move point to the midle of screen
        glutWarpPointer(800/2, 600/2);
    }
    else
    {
        mouse_state = GLUT_UP;
    }
}

void Mouse_active(int x, int y)
{
    //save values in global memory
    mouse_x = x;
    mouse_y = y;
}

void Idle()
{
    current_time = glutGet(GLUT_ELAPSED_TIME);

    int dt = current_time - last_time;

    if (g_eCurrentScene >= 3 || g_eCurrentScene != 6)
    {
        if(mouse_state == GLUT_DOWN)
        {
            // 800 = window width, 600 = window height
            horizontalAngle += mouseSpeed * static_cast<float>(800/2 - mouse_x); 
            verticalAngle   += mouseSpeed * static_cast<float>(600/2 - mouse_y);

            // Direction : Spherical coordinates to Cartesian coordinates conversion
            direction= glm::vec3(
                std::cos(verticalAngle) * std::sin(horizontalAngle),
                std::sin(verticalAngle),
                std::cos(verticalAngle) * std::cos(horizontalAngle)
                );

            // Right vector
            right = glm::vec3(
                std::sin(horizontalAngle - PI/2.0f),
                0.0f,
                std::cos(horizontalAngle - PI/2.0f)
                );

            // Up vector
            up = glm::cross(right, direction);
        }

        // Move forward
        float scalar = static_cast<float>(dt) * cursor_speed;
        if (sp_key == GLUT_KEY_UP)
        {
            position += direction * scalar * 2.0f;
        }

        // Move backward
        if (sp_key == GLUT_KEY_DOWN)
        {
            position -= direction * scalar;
        }

        // Strafe right
        if (sp_key == GLUT_KEY_RIGHT)
        {
            position += right * scalar;
        }

        // Strafe left
        if (sp_key== GLUT_KEY_LEFT)
        {
            position -= right * scalar;
        }
        sp_key = 0;

        // Camera matrix
        View = glm::lookAt(
            position,           // Camera is at position, in World Space
            position+direction, // and looks here : at the same position, plus "direction"
            up                  // Head is up (set to 0,-1,0 to look upside-down)
            );

        glutPostRedisplay();

        std::cout << position.x << " " << position.y <<  " " << position.z << std::endl;
    }

    if (g_eCurrentScene == 5 || g_eCurrentScene == 6)
    {
        counter += 0.002f * static_cast<float>(dt);

        if (g_eCurrentScene == 5)
        {
            MODEL_EVERYTHING = glm::translate(MODEL_EVERYTHING, glm::vec3(0.0f, 0.0f, 0.0013f*counter));
        }

        MODEL_LEG_1 = glm::rotate(MODEL_LEG_1, std::cos(counter), glm::vec3(1.0f, 0.0f, 0.0f));
        MODEL_LEG_2 = glm::rotate(MODEL_LEG_2, -1.0f*std::cos(counter), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    // update when the last timer
    last_time = current_time;
}


void ReshapeGL(int w, int h)
{
    std::cout << "ReshapGL( " << w << ", " << h << " );" << std::endl;

    // Prevent a divide-by-zero error
    if (h == 0)
    {
        // Making Height Equal One
        h = 1;
    }

    g_iWindowWidth = w;
    g_iWindowHeight = h;

    glViewport(0, 0, 800, 600);
}

void Specialkey(int c, int x, int y )
{
    sp_key = c;
}

void KeyboardGL( unsigned char c, int x, int y )
{
    // Store the current scene so we can test if it has changed later.
    unsigned char currentScene = g_eCurrentScene;

    switch (c)
    {
    case '1':
        // Black background
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        g_eCurrentScene = 1;
        break;

    case '2':
        // Black background
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        g_eCurrentScene = 2;
        break;

    case '3':
        // Dark-Gray background
        glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
        g_eCurrentScene = 3;
        break;

    case '4':
        // Light-Gray background
        glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
        g_eCurrentScene = 4;
        break;

    case '5':
        // Light-Gray background
        glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
        g_eCurrentScene = 5;
        position = start_pos;

        // This model will be applied to the whole scene 5
        MODEL_EVERYTHING = glm::mat4(1.0f); // Identity

        // This model will be applied to leg 1
        MODEL_LEG_1 = glm::mat4(1.0f);

        // This model will be applied to leg 2
        MODEL_LEG_2 = glm::mat4(1.0f);
        break;

    case '6':
        // Light-Gray background
        glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
        g_eCurrentScene = 6;
        position = start_pos;

        // This model will be applied to the whole scene 5
        MODEL_EVERYTHING = glm::mat4(1.0f); // Identity

        // This model will be applied to leg 1
        MODEL_LEG_1 = glm::mat4(1.0f);

        // This model will be applied to leg 2
        MODEL_LEG_2 = glm::mat4(1.0f);
        break;

    case 's':
    case 'S':
        std::cout << "Shade Model: GL_SMOOTH" << std::endl;
        // Switch to smooth shading model
        glShadeModel( GL_SMOOTH );
        break;

    case 'f':
    case 'F':
        std::cout << "Shade Model: GL_FLAT" << std::endl;
        // Switch to flat shading model
        glShadeModel( GL_FLAT );
        break;

    case '\033': // escape quits
    case '\015': // Enter quits
    case 'Q':    // Q quits
    case 'q':    // q (or escape) quits
        // Cleanup up and quit
        Cleanup(0);
        break;

    case '\72': //arrow up
        break;
    }


    if (currentScene != g_eCurrentScene)
    {
        std::cout << "Changed Render Scene: " << g_eCurrentScene << std::endl;
    }

    glutPostRedisplay();
}

void Cleanup(int errorCode, bool bExit)
{
    if (g_iGLUTWindowHandle != 0)
    {
        glutDestroyWindow(g_iGLUTWindowHandle);
        g_iGLUTWindowHandle = 0;
    }

    if (bExit)
    {
        exit(errorCode);
    }
}

void DisplayGL()
{
    // Clean up the colour of the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    switch (g_eCurrentScene)
    {
    default:
    case 1: RenderScene1(); break;
    case 2: RenderScene2(); break;
    case 3: RenderScene3(); break;
    case 4: RenderScene4(); break;
    case 5: RenderScene5(); break;
    case 6: RenderScene6(); break;
    }

    // All drawing commands applied to the
    // hidden buffer, so now, bring forward
    // the hidden buffer and hide the visible one
    glutSwapBuffers();
}

void RenderScene1()
{    
    // Disable depth test
    glDisable(GL_DEPTH_TEST);

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

    // Camera matrix
    View = glm::lookAt(
        glm::vec3(5.63757f, 1.7351f, -2.19067f), // Camera is at (4,3,-3), in World Space
        glm::vec3(0.0f, 0.0f, 0.0f),             // and looks at the origin
        glm::vec3(0.0f, 1.0f, 0.0f)              // Head is up (set to 0,-1,0 to look upside-down)
        );

    // Model matrix : an identity matrix (model will be at the origin)
    Model = glm::mat4(1.0f);

    // Our ModelViewProjection : multiplication of our 3 matrices
    // Remember, matrix multiplication is the other way around
    MVP = Projection * View * Model;

    // Use our shader
    glUseProgram(programID_1);

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, box_vert_buf);
    glVertexAttribPointer(
        0,          // attribute. No particular reason for 0, but must match the layout in the shader.
        3,          // size
        GL_FLOAT,   // type
        GL_FALSE,   // normalized?
        0,          // stride
        (void*)0    // array buffer offset
        );

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, box_color_buf);
    glVertexAttribPointer(
        1,          // attribute. No particular reason for 1, but must match the layout in the shader.
        3,          // size
        GL_FLOAT,   // type
        GL_FALSE,   // normalized?
        0,          // stride
        (void*)0    // array buffer offset
        );

    // Draw the triangles
    // 12*3 indices starting at 0 -> 12 triangles
    glDrawArrays(GL_TRIANGLES, 0, 12*3);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glDisable(GL_DEPTH_TEST);
}

void RenderScene2()
{
    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

    // Camera matrix
    View = glm::lookAt(
        glm::vec3(5.63757f, 1.7351f, -2.19067f), // Camera is at (4,3,-3), in World Space
        glm::vec3(0.0f, 0.0f, 0.0f),             // and looks at the origin
        glm::vec3(0.0f, 1.0f, 0.0f)              // Head is up (set to 0,-1,0 to look upside-down)
        );

    // Model matrix : an identity matrix (model will be at the origin)
    Model = glm::mat4(1.0f);

    // Our ModelViewProjection : multiplication of our 3 matrices
    // Remember, matrix multiplication is the other way around
    MVP = Projection * View * Model;

    // Use our shader
    glUseProgram(programID_1);

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    // Use our shader
    glUseProgram(programID_1);

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, box_vert_buf);
    glVertexAttribPointer(
        0,          // attribute. No particular reason for 0, but must match the layout in the shader.
        3,          // size
        GL_FLOAT,   // type
        GL_FALSE,   // normalized?
        0,          // stride
        (void*)0    // array buffer offset
        );

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, box_color_buf);
    glVertexAttribPointer(
        1,          // attribute. No particular reason for 1, but must match the layout in the shader.
        3,          // size
        GL_FLOAT,   // type
        GL_FALSE,   // normalized?
        0,          // stride
        (void*)0    // array buffer offset
        );

    // Draw the triangles
    // 12*3 indices starting at 0 -> 12 triangles
    glDrawArrays(GL_TRIANGLES, 0, 12*3);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glDisable(GL_DEPTH_TEST);
}

void RenderScene3()
{
    // Disable Back Face Culling
    glDisable(GL_CULL_FACE);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.01f, 1000.0f);

    // Model matrix : an identity matrix (model will be at the origin)
    Model = glm::mat4(1.0f);

    // Our ModelViewProjection : multiplication of our 3 matrices
    // Remember, matrix multiplication is the other way around
    MVP = Projection * View * Model;

    // Use our shader
    glUseProgram(programID_1);

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, box_vert_buf);
    glVertexAttribPointer(
        0,          // attribute. No particular reason for 0, but must match the layout in the shader.
        3,          // size
        GL_FLOAT,   // type
        GL_FALSE,   // normalized?
        0,          // stride
        (void*)0    // array buffer offset
        );

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, box_color_buf);
    glVertexAttribPointer(
        1,          // attribute. No particular reason for 1, but must match the layout in the shader.
        3,          // size
        GL_FLOAT,   // type
        GL_FALSE,   // normalized?
        0,          // stride
        (void*)0    // array buffer offset
        );

    // Draw the triangles
    // 12*3 indices starting at 0 -> 12 triangles
    glDrawArrays(GL_TRIANGLES, 0, 12*3);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
}
void RenderScene4()
{
    // Enable bac-face culling
    glEnable(GL_CULL_FACE);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.01f, 1000.0f);

    // Model matrix : an identity matrix (model will be at the origin)
    Model = glm::mat4(1.0f);

    // Our ModelViewProjection : multiplication of our 3 matrices
    // Remember, matrix multiplication is the other way around
    MVP = Projection * View * Model;

    // Use our shader
    glUseProgram(programID_1);

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, box_vert_buf);
    glVertexAttribPointer(
        0,          // attribute. No particular reason for 0, but must match the layout in the shader.
        3,          // size
        GL_FLOAT,   // type
        GL_FALSE,   // normalized?
        0,          // stride
        (void*)0    // array buffer offset
        );

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, box_color_buf);
    glVertexAttribPointer(
        1,          // attribute. No particular reason for 1, but must match the layout in the shader.
        3,          // size
        GL_FLOAT,   // type
        GL_FALSE,   // normalized?
        0,          // stride
        (void*)0    // array buffer offset
        );

    // Draw the triangles
    // 12*3 indices starting at 0 -> 12 triangles
    glDrawArrays(GL_TRIANGLES, 0, 12*3);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
}

void RenderScene5()
{
    //Enable back-Face culling
    //glEnable(GL_CULL_FACE);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Use our shader
    glUseProgram(programID_1);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, box_vert_buf);
    glVertexAttribPointer(
        0,          // attribute. No particular reason for 0, but must match the layout in the shader.
        3,          // size
        GL_FLOAT,   // type
        GL_FALSE,   // normalized?
        0,          // stride
        (void*)0    // array buffer offset
        );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, blue_color_buf);
    glVertexAttribPointer(
        1,          // attribute. No particular reason for 1, but must match the layout in the shader.
        3,          // size
        GL_FLOAT,   // type
        GL_FALSE,   // normalized?
        0,          // stride
        (void*)0    // array buffer offset
        );

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.01f, 1000.0f);

    // =================================================================================
    //      HIP
    // =================================================================================

    // Transform CUBE
    // cube --> parallelepiped
    Model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.1f, 0.1f));

    // Our ModelViewProjection : multiplication of our 3 matrices
    MVP1 = Projection * View* MODEL_EVERYTHING * Model;
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP1[0][0]);

    // Draw the trinagles
    // 12*3 indices starting at 0 -> 12 triangles
    glDrawArrays(GL_TRIANGLES, 0, 12*3);

    glBindBuffer(GL_ARRAY_BUFFER, green_color_buf);
    glVertexAttribPointer(
        1,          // attribute. No particular reason for 1, but must match the layout in the shader.
        3,          // size
        GL_FLOAT,   // type
        GL_FALSE,   // normalized?
        0,          // stride
        (void*)0    // array buffer offset
        );

    // =================================================================================
    //      HIP JOINT 1
    // =================================================================================

    // Transform the cube
    Model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
    Model = glm::scale(Model, glm::vec3(0.2f, 0.2f, 0.2f));

    // MVP
    MVP3 = Projection * View * MODEL_EVERYTHING * MODEL_LEG_1 * Model;
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP3[0][0]);

    // Draw the trinagles
    // 12*3 indices starting at 0 -> 12 triangles
    glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

    // =================================================================================
    //      HIP JOINT 2
    // =================================================================================

    //transform the cube
    Model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    Model = glm::scale(Model, glm::vec3(0.2f, 0.2f, 0.2f));

    // MVP
    MVP2 = Projection * View * MODEL_EVERYTHING * MODEL_LEG_2 * Model;
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]);

    // Draw the trinagles
    // 12*3 indices starting at 0 -> 12 triangles
    glDrawArrays(GL_TRIANGLES, 0, 12*3);

    // end
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, leg_vert_buf);
    glVertexAttribPointer(
        0,          // attribute. No particular reason for 0, but must match the layout in the shader.
        3,          // size
        GL_FLOAT,   // type
        GL_FALSE,   // normalized?
        0,          // stride
        (void*)0    // array buffer offset
        );

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, leg_color_buf);
    glVertexAttribPointer(
        1,          // attribute. No particular reason for 1, but must match the layout in the shader.
        3,          // size
        GL_FLOAT,   // type
        GL_FALSE,   // normalized?
        0,          // stride
        (void*)0    // array buffer offset
        );

    GLsizei quadsSize = 2 * 4, quadStripSize = 6 * 2 + 2;

    glBindBuffer(GL_ARRAY_BUFFER, yellow_color_buf);
    glVertexAttribPointer(
        1,          // attribute. No particular reason for 1, but must match the layout in the shader.
        3,          // size
        GL_FLOAT,   // type
        GL_FALSE,   // normalized?
        0,          // stride
        (void*)0    // array buffer offset
        );

    // =================================================================================
    //      LEG 1
    // =================================================================================

    // transform the cube
    Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.6f, 0.1f));
    Model = glm::translate(Model, glm::vec3(-10.0f, -1.0f, 0.0f));

    // MVP
    MVP4 = Projection * View * MODEL_EVERYTHING * MODEL_LEG_1 * Model;
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP4[0][0]);

    // Draw the quads
    glDrawArrays(GL_QUADS, 0, quadsSize);
    glDrawArrays(GL_QUADS, quadsSize, quadsSize);
    glDrawArrays(GL_QUAD_STRIP, 2 * quadsSize, quadStripSize);

    glBindBuffer(GL_ARRAY_BUFFER, red_color_buf);
    glVertexAttribPointer(
        1,          // attribute. No particular reason for 1, but must match the layout in the shader.
        3,          // size
        GL_FLOAT,   // type
        GL_FALSE,   // normalized?
        0,          // stride
        (void*)0    // array buffer offset
        );

    // =================================================================================
    //      LEG 2
    // =================================================================================

    // Transform the cube
    Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.6f, 0.1f));
    Model = glm::translate(Model, glm::vec3(10.0f, -1.0f, 0.0f));

    // MVP
    MVP5 = Projection * View * MODEL_EVERYTHING * MODEL_LEG_2 * Model;
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP5[0][0]);

    // Draw the quads
    glDrawArrays(GL_QUADS, 0, quadsSize);
    glDrawArrays(GL_QUADS, quadsSize, quadsSize);
    glDrawArrays(GL_QUAD_STRIP, 2 * quadsSize, quadStripSize);

    // done
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    //glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
}

void RenderScene6()
{
    RenderScene5();
}

void SetupGL()
{
    //Parameter handling
    glShadeModel (GL_SMOOTH);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // polygon rendering mode
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    //Set up light source
    GLfloat light_position[] = { 0.0f, 30.0f, -50.0f, 0.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Register GLUT callbacks
    glutDisplayFunc(DisplayGL);
    glutKeyboardFunc(KeyboardGL);
    glutSpecialFunc(Specialkey);
    glutReshapeFunc(ReshapeGL);
    glutMouseFunc(MouseGL);
    glutMotionFunc(Mouse_active);

    //Call to the drawing function
    glutIdleFunc(Idle);
    last_time = glutGet(GLUT_ELAPSED_TIME);

    // Setup initial GL State
    glClearDepth(1.0f);

    // Init GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW." << std::endl;
        exit(-1);
    }

    // Setup initial GL State
    glClearDepth(1.0f);

    std::cout << "Initialise OpenGL: Success!" << std::endl;

    // VAO
    //        glGenVertexArrays(1, &VertexArrayID);
    //        glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    programID_1 = LoadShaders("TransformVertexShader.vert", "ColorFragmentShader.frag");

    // VBO -- VERTEX
    static const GLfloat g_box_vert_buf_data[] = {
        -1.0f, -1.0f, -1.0f, // triangle 1 : begin
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f, // triangle 1 : end
         1.0f,  1.0f, -1.0f, // triangle 2 : begin
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f, // triangle 2 : end
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // 1. Generate 1 buffer, put the resulting identifier in vertexbuffer
    // 2. Bind the buffer to which type
    // 3. Send it top OpenGL
    glGenBuffers(1, &box_vert_buf);
    glBindBuffer(GL_ARRAY_BUFFER, box_vert_buf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_box_vert_buf_data), g_box_vert_buf_data, GL_STATIC_DRAW);

    static const GLfloat g_box_color_buf_data[] = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
        0.822f,  0.569f,  0.201f,
        0.435f,  0.602f,  0.223f,
        0.310f,  0.747f,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
        0.359f,  0.583f,  0.152f,
        0.483f,  0.596f,  0.789f,
        0.559f,  0.861f,  0.639f,
        0.195f,  0.548f,  0.859f,
        0.014f,  0.184f,  0.576f,
        0.771f,  0.328f,  0.970f,
        0.406f,  0.615f,  0.116f,
        0.676f,  0.977f,  0.133f,
        0.971f,  0.572f,  0.833f,
        0.140f,  0.616f,  0.489f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
        0.055f,  0.953f,  0.042f,
        0.714f,  0.505f,  0.345f,
        0.783f,  0.290f,  0.734f,
        0.722f,  0.645f,  0.174f,
        0.302f,  0.455f,  0.848f,
        0.225f,  0.587f,  0.040f,
        0.517f,  0.713f,  0.338f,
        0.053f,  0.959f,  0.120f,
        0.393f,  0.621f,  0.362f,
        0.673f,  0.211f,  0.457f,
        0.820f,  0.883f,  0.371f,
        0.982f,  0.099f,  0.879f
    };

    // 1. Generate 1 buffer, put the resulting identifier in vertexbuffer
    // 2. Bind the buffer to which type
    // 3. Send it top OpenGL
    glGenBuffers(1, &box_color_buf);
    glBindBuffer(GL_ARRAY_BUFFER, box_color_buf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_box_color_buf_data), g_box_color_buf_data, GL_STATIC_DRAW);

    GLfloat mip = -0.8f;
    GLfloat toe = 1.5f;

    static const GLfloat g_leg_vert_buf_data[] = {
        -1.0f, -1.0f,  0.0f, // Quad 1 
        -1.0f,   mip,  0.0f,
        -1.0f,   mip,  toe,
        -1.0f, -1.0f,  toe,
        -1.0f, -1.0f, -1.0f, // Quad 2
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  0.0f,
        -1.0f, -1.0f,  0.0f,
         1.0f, -1.0f,  0.0f, // Quad 3
         1.0f,   mip,  0.0f,
         1.0f,   mip,  toe,
         1.0f, -1.0f,  toe,
         1.0f, -1.0f, -1.0f, // Quad 4
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  0.0f,
         1.0f, -1.0f,  0.0f,
        -1.0f,   mip,  0.0f, // Quad strip begin
         1.0f,   mip,  0.0f,
        -1.0f,   mip,  toe,
         1.0f,   mip,  toe,
        -1.0f, -1.0f,  toe,
         1.0f, -1.0f,  toe,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  0.0f,
         1.0f,  1.0f,  0.0f,
        -1.0f,   mip,  0.0f,
         1.0f,   mip,  0.0f, // Quad strip end
    };

    // 1. Generate 1 buffer, put the resulting identifier in vertexbuffer
    // 2. Bind the buffer to which type
    // 3. Send it top OpenGL
    glGenBuffers(1, &leg_vert_buf);
    glBindBuffer(GL_ARRAY_BUFFER, leg_vert_buf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_leg_vert_buf_data), g_leg_vert_buf_data, GL_STATIC_DRAW);

    static const GLfloat g_leg_color_buf_data[] = {
        0.374f, 0.842f, 0.102f, // Quad 1
        0.204f, 0.857f, 0.298f,
        0.427f, 0.310f, 0.756f,
        0.829f, 0.154f, 0.259f,
        0.778f, 0.274f, 0.292f, // Qaud 2
        0.510f, 0.754f, 0.737f,
        0.498f, 0.829f, 0.777f,
        0.961f, 0.483f, 0.572f,
        0.431f, 0.362f, 0.918f, // Quad 3
        0.689f, 0.877f, 0.544f,
        0.927f, 0.207f, 0.283f,
        0.519f, 0.440f, 0.640f,
        0.674f, 0.130f, 0.676f, // Quad 4
        0.436f, 0.432f, 0.868f,
        0.799f, 0.981f, 0.422f,
        0.700f, 0.714f, 0.104f,
        0.278f, 0.119f, 0.501f, // Quad strip begin
        0.428f, 0.242f, 0.332f,
        0.846f, 0.943f, 0.351f,
        0.688f, 0.362f, 0.225f,
        0.688f, 0.909f, 0.916f, 
        0.452f, 0.746f, 0.462f,
        0.460f, 0.206f, 0.913f,
        0.444f, 0.762f, 0.328f,
        0.778f, 0.406f, 0.973f, 
        0.674f, 0.886f, 0.596f,
        0.935f, 0.807f, 0.991f,
        0.750f, 0.339f, 0.850f,
        0.904f, 0.254f, 0.856f, 
        0.733f, 0.686f, 0.309f,
    };

    // 1. Generate 1 buffer, put the resulting identifier in vertexbuffer
    // 2. Bind the buffer to which type
    // 3. Send it top OpenGL
    glGenBuffers(1, &leg_color_buf);
    glBindBuffer(GL_ARRAY_BUFFER, leg_color_buf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_leg_color_buf_data), g_leg_color_buf_data, GL_STATIC_DRAW);

    static const GLfloat g_yellow_color_buf_data[] = {
        1.0f, 1.0f, 0.0f, // Quad 1
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, // Qaud 2
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, // Quad 3
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, // Quad 4
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, // Quad strip begin
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
    };

    // 1. Generate 1 buffer, put the resulting identifier in vertexbuffer
    // 2. Bind the buffer to which type
    // 3. Send it top OpenGL
    glGenBuffers(1, &yellow_color_buf);
    glBindBuffer(GL_ARRAY_BUFFER, yellow_color_buf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_yellow_color_buf_data), g_yellow_color_buf_data, GL_STATIC_DRAW);

    static const GLfloat g_red_color_buf_data[] = {
        1.0f, 0.0f, 0.0f, // Quad 1
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, // Qaud 2
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, // Quad 3
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, // Quad 4
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, // Quad strip begin
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
    };

    // 1. Generate 1 buffer, put the resulting identifier in vertexbuffer
    // 2. Bind the buffer to which type
    // 3. Send it top OpenGL
    glGenBuffers(1, &red_color_buf);
    glBindBuffer(GL_ARRAY_BUFFER, red_color_buf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_red_color_buf_data), g_red_color_buf_data, GL_STATIC_DRAW);

    static const GLfloat g_blue_color_buf_data[] = {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f
    };

    // 1. Generate 1 buffer, put the resulting identifier in vertexbuffer
    // 2. Bind the buffer to which type
    // 3. Send it top OpenGL
    glGenBuffers(1, &blue_color_buf);
    glBindBuffer(GL_ARRAY_BUFFER, blue_color_buf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_blue_color_buf_data), g_blue_color_buf_data, GL_STATIC_DRAW);

    static const GLfloat g_green_color_buf_data[] = {
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    // 1. Generate 1 buffer, put the resulting identifier in vertexbuffer
    // 2. Bind the buffer to which type
    // 3. Send it top OpenGL
    glGenBuffers(1, &green_color_buf);
    glBindBuffer(GL_ARRAY_BUFFER, green_color_buf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_green_color_buf_data), g_green_color_buf_data, GL_STATIC_DRAW);

}
