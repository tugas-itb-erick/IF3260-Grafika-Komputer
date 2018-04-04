#include "stdafx.h"
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

/* Global variables */
const float PI = 3.14159265359;
const int WIDTH = 800;
const int HEIGHT = 600;
const float TRANSLATE_CONSTANT = 0.002f;
const float ROTATE_DEGREE = 60.0f;
const float SCALE_FACTOR_DELTA = 0.0025f;

glm::vec3 box[24] = {
    glm::vec3(1.0f, -0.33f, -2.33f),
    glm::vec3(1.0f, 1.0f, -2.33f),
    glm::vec3(1.0f, 1.0f, 3.67f),
    glm::vec3(1.0f, -0.33f, 3.67f),
    
    glm::vec3(-1.0f, -0.33f, -2.33f),
    glm::vec3(-1.0f, 1.0f, -2.33f),
    glm::vec3(1.0f, 1.0f, -2.33f),
    glm::vec3(1.0f, -0.33f, -2.33f),
    
    glm::vec3(-1.0f, -0.33f, 3.67f),
    glm::vec3(-1.0f, 1.0f, 3.67f),
    glm::vec3(-1.0f, 1.0f, -2.33f),
    glm::vec3(-1.0f, -0.33f, -2.33f),
    
    glm::vec3(1.0f, -0.33f, 3.67f),
    glm::vec3(1.0f, 1.0f, 3.67f),
    glm::vec3(-1.0f, 1.0f, 3.67f),
    glm::vec3(-1.0f, -0.33f, 3.67f),
    
    glm::vec3(1.0f, -0.33f, 3.67f),
    glm::vec3(-1.0f, -0.33f, 3.67f),
    glm::vec3(-1.0f, -0.33f, -2.33f),
    glm::vec3(1.0f, -0.33f, -2.33f),
    
    glm::vec3(1.0f, 1.0f, 3.67f),
    glm::vec3(-1.0f, 1.0f, 3.67f),
    glm::vec3(-1.0f, 1.0f, -2.33f),
    glm::vec3(1.0f, 1.0f, -2.33f)
};

glm::vec3 box2[24] = {
    //depan
    glm::vec3(-1.0f, 1.0f, -1.33f),
    glm::vec3(-1.0f, 2.33f, 0.0f),
    glm::vec3(1.0f, 2.33f, 0.0f),
    glm::vec3(1.0f, 1.0f, -1.33f),
    
    glm::vec3(-1.0f, 1.0f, 3.0f),
    glm::vec3(-1.0f, 2.33f, 2.33f),
    glm::vec3(-1.0f, 2.33f, 0.0f),
    glm::vec3(-1.0f, 1.0f, -1.33f),
    
    //belakang
    glm::vec3(1.0f, 1.0f, 3.0f),
    glm::vec3(1.0f, 2.33f, 2.33f),
    glm::vec3(-1.0f, 2.33f, 2.33f),
    glm::vec3(-1.0f, 1.0f, 3.0f),
    
    glm::vec3(1.0f, 1.0f, -1.33f),
    glm::vec3(1.0f, 2.33f, 0.0f),
    glm::vec3(1.0f, 2.33f, 2.33f),
    glm::vec3(1.0f, 1.0f, 3.0f),
    
    glm::vec3(-1.0f, 2.33f, 0.0f),
    glm::vec3(1.0f, 2.33f, 0.0f),
    glm::vec3(1.0f, 2.33f, 2.33f),
    glm::vec3(-1.0f, 2.33f, 2.33f),
    
    glm::vec3(1.0f, 1.0f, 3.67f),
    glm::vec3(-1.0f, 1.0f, 3.67f),
    glm::vec3(-1.0f, 1.0f, -2.33f),
    glm::vec3(1.0f, 1.0f, -2.33f)
};

//back window yg ijo
glm::vec3 window[4] = {
    glm::vec3(0.67f, 1.33f, 2.85f),
    glm::vec3(0.67f, 2.0f, 2.52f),
    glm::vec3(-0.67f, 2.0f, 2.52f),
    glm::vec3(-0.67f, 1.33f, 2.85f)
};

//side back window
glm::vec3 window2[4] = {
    glm::vec3(1.01f, 1.0f, 1.33f),
    glm::vec3(1.01f, 2.0f, 1.33f),
    glm::vec3(1.01f, 2.0f, 2.0f),
    glm::vec3(1.01f, 1.0f, 2.67f)
};

//side front window
glm::vec3 window3[4] = {
    glm::vec3(1.01f, 1.0f, -1.0f),
    glm::vec3(1.01f, 2.0f, 0.0f),
    glm::vec3(1.01f, 2.0f, 0.67f),
    glm::vec3(1.01f, 1.0f, 0.67f)
};

//front window
glm::vec3 window4[4] = {
    glm::vec3(-0.67f, 1.33f, -1.01f),
    glm::vec3(-0.67f, 2.0f, -0.34f),
    glm::vec3(0.67f, 2.0f, -0.34f),
    glm::vec3(0.67f, 1.33f, -1.01f)
};

//side front window
glm::vec3 window5[4] = {
    glm::vec3(-1.01f, 1.0f, 0.67f),
    glm::vec3(-1.01f, 2.0f, 0.67f),
    glm::vec3(-1.01f, 2.0f, 0.0f),
    glm::vec3(-1.01f, 1.0f, -1.0f)
};

//side back window
glm::vec3 window6[4] = {
    glm::vec3(-1.01f, 1.0f, 2.67f),
    glm::vec3(-1.01f, 2.0f, 2.0f),
    glm::vec3(-1.01f, 2.0f, 1.33f),
    glm::vec3(-1.01f, 1.0f, 1.33f)
};

glm::vec3 door1[4] = {
    glm::vec3(-1.01f, 2.33f, 1.01f),
    glm::vec3(-1.01f, 2.33f, 0.99f),
    glm::vec3(-1.01f, -0.33f, 0.99f),
    glm::vec3(-1.01f, -0.33f, 1.01f)
};

glm::vec3 door2[4] = {
    glm::vec3(1.01f, 2.33f, 1.01f),
    glm::vec3(1.01f, 2.33f, 0.99f),
    glm::vec3(1.01f, -0.33f, 0.99f),
    glm::vec3(1.01f, -0.33f, 1.01f)
};

glm::vec3 handle1[4] = {
    glm::vec3(-1.01f, 0.75f, 1.63f),
    glm::vec3(-1.01f, 0.75f, 1.33f),
    glm::vec3(-1.01f, 0.6f, 1.33f),
    glm::vec3(-1.01f, 0.6f, 1.63f)
};

glm::vec3 handle2[4] = {
    glm::vec3(-1.01f, 0.75f, 0.67f),
    glm::vec3(-1.01f, 0.75f, 0.37f),
    glm::vec3(-1.01f, 0.6f, 0.37f),
    glm::vec3(-1.01f, 0.6f, 0.67f)
};

glm::vec3 handle3[4] = {
    glm::vec3(1.01f, 0.75f, 1.63f),
    glm::vec3(1.01f, 0.75f, 1.33f),
    glm::vec3(1.01f, 0.6f, 1.33f),
    glm::vec3(1.01f, 0.6f, 1.63f)
};

glm::vec3 handle4[4] = {
    glm::vec3(1.01f, 0.75f, 0.67f),
    glm::vec3(1.01f, 0.75f, 0.37f),
    glm::vec3(1.01f, 0.6f, 0.37f),
    glm::vec3(1.01f, 0.6f, 0.67f)
};

glm::vec3 lamp1[24] = {
    glm::vec3(0.8f, 0.75f, 3.67f),
    glm::vec3(0.8f, 0.45f, 3.67f),
    glm::vec3(0.8f, 0.45f, 3.8f),
    glm::vec3(0.8f, 0.75f, 3.8f),
    
    glm::vec3(0.8f, 0.75f, 3.67f),
    glm::vec3(0.8f, 0.45f, 3.67f),
    glm::vec3(0.4f, 0.45f, 3.67f),
    glm::vec3(0.4f, 0.75f, 3.67f),
    
    glm::vec3(0.8f, 0.75f, 3.67f),
    glm::vec3(0.8f, 0.75f, 3.8f),
    glm::vec3(0.4f, 0.75f, 3.8f),
    glm::vec3(0.4f, 0.75f, 3.67f),
    
    glm::vec3(0.4f, 0.75f, 3.67f),
    glm::vec3(0.4f, 0.45f, 3.67f),
    glm::vec3(0.4f, 0.45f, 3.8f),
    glm::vec3(0.4f, 0.75f, 3.8f),
    
    glm::vec3(0.8f, 0.75f, 3.8f),
    glm::vec3(0.8f, 0.45f, 3.8f),
    glm::vec3(0.4f, 0.45f, 3.8f),
    glm::vec3(0.4f, 0.75f, 3.8f),
    
    glm::vec3(0.8f, 0.45f, 3.67f),
    glm::vec3(0.8f, 0.45f, 3.8f),
    glm::vec3(0.4f, 0.45f, 3.8f),
    glm::vec3(0.4f, 0.45f, 3.67f)
};

glm::vec3 lamp2[24] = {
    glm::vec3(-0.8f, 0.75f, 3.67f),
    glm::vec3(-0.8f, 0.45f, 3.67f),
    glm::vec3(-0.8f, 0.45f, 3.8f),
    glm::vec3(-0.8f, 0.75f, 3.8f),
    
    glm::vec3(-0.8f, 0.75f, 3.67f),
    glm::vec3(-0.8f, 0.45f, 3.67f),
    glm::vec3(-0.4f, 0.45f, 3.67f),
    glm::vec3(-0.4f, 0.75f, 3.67f),
    
    glm::vec3(-0.8f, 0.75f, 3.67f),
    glm::vec3(-0.8f, 0.75f, 3.8f),
    glm::vec3(-0.4f, 0.75f, 3.8f),
    glm::vec3(-0.4f, 0.75f, 3.67f),
    
    glm::vec3(-0.4f, 0.75f, 3.67f),
    glm::vec3(-0.4f, 0.45f, 3.67f),
    glm::vec3(-0.4f, 0.45f, 3.8f),
    glm::vec3(-0.4f, 0.75f, 3.8f),
    
    glm::vec3(-0.8f, 0.75f, 3.8f),
    glm::vec3(-0.8f, 0.45f, 3.8f),
    glm::vec3(-0.4f, 0.45f, 3.8f),
    glm::vec3(-0.4f, 0.75f, 3.8f),
    
    glm::vec3(-0.8f, 0.45f, 3.67f),
    glm::vec3(-0.8f, 0.45f, 3.8f),
    glm::vec3(-0.4f, 0.45f, 3.8f),
    glm::vec3(-0.4f, 0.45f, 3.67f)
};

glm::vec3 lamp3[24] = {
    glm::vec3(0.8f, 0.75f, -2.33f),
    glm::vec3(0.8f, 0.45f, -2.33f),
    glm::vec3(0.8f, 0.45f, -2.46f),
    glm::vec3(0.8f, 0.75f, -2.46f),
    
    glm::vec3(0.8f, 0.75f, -2.33f),
    glm::vec3(0.8f, 0.45f, -2.33f),
    glm::vec3(0.4f, 0.45f, -2.33f),
    glm::vec3(0.4f, 0.75f, -2.33f),
    
    glm::vec3(0.8f, 0.75f, -2.33f),
    glm::vec3(0.8f, 0.75f, -2.46f),
    glm::vec3(0.4f, 0.75f, -2.46f),
    glm::vec3(0.4f, 0.75f, -2.33f),
    
    glm::vec3(0.4f, 0.75f, -2.33f),
    glm::vec3(0.4f, 0.45f, -2.33f),
    glm::vec3(0.4f, 0.45f, -2.46f),
    glm::vec3(0.4f, 0.75f, -2.46f),
    
    glm::vec3(0.8f, 0.75f, -2.46f),
    glm::vec3(0.8f, 0.45f, -2.46f),
    glm::vec3(0.4f, 0.45f, -2.46f),
    glm::vec3(0.4f, 0.75f, -2.46f),
    
    glm::vec3(0.8f, 0.45f, -2.33f),
    glm::vec3(0.8f, 0.45f, -2.46f),
    glm::vec3(0.4f, 0.45f, -2.46f),
    glm::vec3(0.4f, 0.45f, -2.33f)
};

glm::vec3 lamp4[24] = {
    glm::vec3(-0.8f, 0.75f, -2.33f),
    glm::vec3(-0.8f, 0.45f, -2.33f),
    glm::vec3(-0.8f, 0.45f, -2.46f),
    glm::vec3(-0.8f, 0.75f, -2.46f),
    
    glm::vec3(-0.8f, 0.75f, -2.33f),
    glm::vec3(-0.8f, 0.45f, -2.33f),
    glm::vec3(-0.4f, 0.45f, -2.33f),
    glm::vec3(-0.4f, 0.75f, -2.33f),
    
    glm::vec3(-0.8f, 0.75f, -2.33f),
    glm::vec3(-0.8f, 0.75f, -2.46f),
    glm::vec3(-0.4f, 0.75f, -2.46f),
    glm::vec3(-0.4f, 0.75f, -2.33f),
    
    glm::vec3(-0.4f, 0.75f, -2.33f),
    glm::vec3(-0.4f, 0.45f, -2.33f),
    glm::vec3(-0.4f, 0.45f, -2.46f),
    glm::vec3(-0.4f, 0.75f, -2.46f),
    
    glm::vec3(-0.8f, 0.75f, -2.46f),
    glm::vec3(-0.8f, 0.45f, -2.46f),
    glm::vec3(-0.4f, 0.45f, -2.46f),
    glm::vec3(-0.4f, 0.75f, -2.46f),
    
    glm::vec3(-0.8f, 0.45f, -2.33f),
    glm::vec3(-0.8f, 0.45f, -2.46f),
    glm::vec3(-0.4f, 0.45f, -2.46f),
    glm::vec3(-0.4f, 0.45f, -2.33f)
};

glm::vec3 wheel1[24] = {
	glm::vec3(0.8f, 0.4f, 2.3),
	glm::vec3(0.8f, -0.8f, 2.3),
	glm::vec3(0.8f, -0.8f, 3),
	glm::vec3(0.8f, 0.4f, 3),

	glm::vec3(0.8f, 0.4f, 2.3),
	glm::vec3(0.8f, -0.8f, 2.3),
	glm::vec3(0.3f, -0.8f, 2.3),
	glm::vec3(0.3f, 0.4f, 2.3),

	glm::vec3(0.8f, 0.4f, 2.3),
	glm::vec3(0.8f, 0.4f, 3),
	glm::vec3(0.3f, 0.4f, 3),
	glm::vec3(0.3f, 0.4f, 2.3),

	glm::vec3(0.3f, 0.4f, 2.3),
	glm::vec3(0.3f, -0.8f, 2.3),
	glm::vec3(0.3f, -0.8f, 3),
	glm::vec3(0.3f, 0.4f, 3),

	glm::vec3(0.8f, 0.4f, 3),
	glm::vec3(0.8f, -0.8f, 3),
	glm::vec3(0.3f, -0.8f, 3),
	glm::vec3(0.3f, -0.8f, 3),

	glm::vec3(0.8f, -0.8f, 2.3),
	glm::vec3(0.8f, -0.8f, 3),
	glm::vec3(0.3f, -0.8f, 3),
	glm::vec3(0.3f, -0.8f, 2.3)
};

glm::vec3 wheel2[24] = {
	glm::vec3(0.8f, 0.4f, -1.4),
	glm::vec3(0.8f, -0.8f, -1.4),
	glm::vec3(0.8f, -0.8f, -2.1),
	glm::vec3(0.8f, 0.4f, -2.1),

	glm::vec3(0.8f, 0.4f, -1.4),
	glm::vec3(0.8f, -0.8f, -1.4),
	glm::vec3(0.3f, -0.8f, -1.4),
	glm::vec3(0.3f, 0.4f, -1.4),

	glm::vec3(0.8f, 0.4f, -1.4),
	glm::vec3(0.8f, 0.4f, -2.1),
	glm::vec3(0.3f, 0.4f, -2.1),
	glm::vec3(0.3f, 0.4f, -1.4),

	glm::vec3(0.3f, 0.4f, -1.4),
	glm::vec3(0.3f, -0.8f, -1.4),
	glm::vec3(0.3f, -0.8f, -2.1),
	glm::vec3(0.3f, 0.4f, -2.1),

	glm::vec3(0.8f, 0.4f, -2.1),
	glm::vec3(0.8f, -0.8f, -2.1),
	glm::vec3(0.3f, -0.8f, -2.1),
	glm::vec3(0.3f, -0.8f, -2.1),

	glm::vec3(0.8f, -0.8f, -1.4),
	glm::vec3(0.8f, -0.8f, -2.1),
	glm::vec3(0.3f, -0.8f, -2.1),
	glm::vec3(0.3f, -0.8f, -1.4)
};

glm::vec3 wheel3[24] = {
	glm::vec3(-0.3, 0.4f, 2.3),
	glm::vec3(-0.3, -0.8f, 2.3),
	glm::vec3(-0.3, -0.8f, 3),
	glm::vec3(-0.3, 0.4f, 3),

	glm::vec3(-0.3, 0.4f, 2.3),
	glm::vec3(-0.3, -0.8f, 2.3),
	glm::vec3(-0.8f, -0.8f, 2.3),
	glm::vec3(-0.8f, 0.4f, 2.3),

	glm::vec3(-0.3, 0.4f, 2.3),
	glm::vec3(-0.3, 0.4f, 3),
	glm::vec3(-0.8f, 0.4f, 3),
	glm::vec3(-0.8f, 0.4f, 2.3),

	glm::vec3(-0.8f, 0.4f, 2.3),
	glm::vec3(-0.8f, -0.8f, 2.3),
	glm::vec3(-0.8f, -0.8f, 3),
	glm::vec3(-0.8f, 0.4f, 3),

	glm::vec3(-0.3, 0.4f, 3),
	glm::vec3(-0.3, -0.8f, 3),
	glm::vec3(-0.8f, -0.8f, 3),
	glm::vec3(-0.8f, -0.8f, 3),

	glm::vec3(-0.3, -0.8f, 2.3),
	glm::vec3(-0.3, -0.8f, 3),
	glm::vec3(-0.8f, -0.8f, 3),
	glm::vec3(-0.8f, -0.8f, 2.3)
};

glm::vec3 wheel4[24] = {
	glm::vec3(-0.3, 0.4f, -1.4),
	glm::vec3(-0.3, -0.8f, -1.4),
	glm::vec3(-0.3f, -0.8f, -2.1),
	glm::vec3(-0.3f, 0.4f, -2.1),

	glm::vec3(-0.3f, 0.4f, -1.4),
	glm::vec3(-0.3f, -0.8f, -1.4),
	glm::vec3(-0.8f, -0.8f, -1.4),
	glm::vec3(-0.8f, 0.4f, -1.4),

	glm::vec3(-0.3f, 0.4f, -1.4),
	glm::vec3(-0.3f, 0.4f, -2.1),
	glm::vec3(-0.8f, 0.4f, -2.1),
	glm::vec3(-0.8f, 0.4f, -1.4),

	glm::vec3(-0.8f, 0.4f, -1.4),
	glm::vec3(-0.8f, -0.8f, -1.4),
	glm::vec3(-0.8f, -0.8f, -2.1),
	glm::vec3(-0.8f, 0.4f, -2.1),

	glm::vec3(-0.3f, 0.4f, -2.1),
	glm::vec3(-0.3f, -0.8f, -2.1),
	glm::vec3(-0.8f, -0.8f, -2.1),
	glm::vec3(-0.8f, -0.8f, -2.1),

	glm::vec3(-0.3f, -0.8f, -1.4),
	glm::vec3(-0.3f, -0.8f, -2.1),
	glm::vec3(-0.8f, -0.8f, -2.1),
	glm::vec3(-0.8f, -0.8f, -1.4)
};

glm::mat3 rx, ry, rz;
glm::mat3 dilate;
glm::vec3 translate;
float degreeX = 0,
degreeY = 0,
degreeZ = 0;
float tx = 0,
ty = 0,
tz = 0;
float scaleFactor = 1;


void drawHexagon(GLfloat center_x, GLfloat center_y, GLfloat center_z, GLfloat size, GLfloat height, GLfloat* color)
{
glPushMatrix();
{
glColor3f(color[0], color[1], color[2]);
GLfloat firstHex[12][3];
GLfloat secondHex[12][3];
glBegin(GL_POLYGON);
{
for (GLint i = 0; i < 12; i++)
{
GLfloat angle = 2 * PI / 12 * (i + 0.5);
GLfloat x_tempVertex = center_x + size * cos(angle);
GLfloat z_tempVertex = center_z + size * sin(angle);
firstHex[i][0] = x_tempVertex;
firstHex[i][1] = center_y;
firstHex[i][2] = z_tempVertex;
glVertex3f(x_tempVertex, center_y, z_tempVertex);
}
}
glEnd();

glBegin(GL_POLYGON);
{
for (GLint i = 0; i < 12; i++)
{
GLfloat angle = 2 * PI / 12 * (i + 0.5);
GLfloat x_tempVertex = center_x + size * cos(angle);
GLfloat z_tempVertex = center_z + size * sin(angle);
secondHex[i][0] = x_tempVertex;
secondHex[i][1] = center_y + height;
secondHex[i][2] = z_tempVertex;
glVertex3f(x_tempVertex, center_y+height, z_tempVertex);
}
}
glEnd();

// Begin filling the rectangles between the two hexagons
// drawing begins at ( 0, 0, 0 ) and moves clockwise
//  ( 0, 0, 1 ) [the mid single coord in each set represents the middle point up/down respectivly]
glColor3f(1,1,1);
glBegin(GL_QUADS);
{
for (GLint i = 0; i < 5; i++)
{
glVertex3f(firstHex[i][0], firstHex[i][1], firstHex[i][2]); // top left
glVertex3f(secondHex[i][0], secondHex[i][1], secondHex[i][2]); // bottom left
glVertex3f(secondHex[i+1][0], secondHex[i+1][1], secondHex[i+1][2]); // top right
glVertex3f(firstHex[i+1][0], firstHex[i+1][1], firstHex[i+1][2]); // bottom right
}

// corner case (connecting the end to the beginning)
glVertex3f(firstHex[5][0], firstHex[5][1], firstHex[5][2]); // top left
glVertex3f(secondHex[5][0], secondHex[5][1], secondHex[5][2]); // bottom left
glVertex3f(secondHex[0][0], secondHex[0][1], secondHex[0][2]); // top right
glVertex3f(firstHex[0][0], firstHex[0][1], firstHex[0][2]); // bottom right
}
glEnd();
}
glPopMatrix();
}

/* Initialize OpenGL Graphics */
void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClearDepth(1.0f);        // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
    glShadeModel(GL_SMOOTH);   // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

/* Handler for window-repaint event. Called back when the window first appears and
 whenever the window needs to be re-painted. */
void display() {
    float rxArr[9] = {
        1, 0, 0,
        0, cos(degreeX * PI * 180), -sin(degreeX * PI * 180),
        0, sin(degreeX * PI * 180), cos(degreeX * PI * 180)
    };
    float ryArr[9] = {
        cos(degreeY * PI * 180), 0, sin(degreeY * PI * 180),
        0, 1, 0,
        -sin(degreeY * PI * 180), 0, cos(degreeY * PI * 180)
    };
    float rzArr[9] = {
        cos(degreeZ * PI * 180), -sin(degreeZ * PI * 180), 0,
        sin(degreeZ * PI * 180), cos(degreeZ * PI * 180), 0,
        0, 0, 1
    };
    float dilateArr[9] = {
        scaleFactor, 0, 0,
        0, scaleFactor, 0,
        0, 0, scaleFactor
    };
    float translateArr[3] = {
        tx,
        ty,
        tz
    };
    memcpy(glm::value_ptr(rx), rxArr, sizeof(rxArr));
    memcpy(glm::value_ptr(ry), ryArr, sizeof(ryArr));
    memcpy(glm::value_ptr(rz), rzArr, sizeof(rzArr));
    memcpy(glm::value_ptr(dilate), dilateArr, sizeof(dilateArr));
    memcpy(glm::value_ptr(translate), translateArr, sizeof(translateArr));
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
    
    // Render a color-cube consisting of 6 quads with different colors
    glLoadIdentity();                 // Reset the model-view matrix
    glTranslatef(0.0f, 0.0f, -7.0f);  // Move right and into the screen
    
    glBegin(GL_QUADS);// Define vertices in counter-clockwise (CCW) order with normal pointing out
    
    for (int i = 0; i < 24; i++) {
        switch ((i+1)/4) // cuma buat ngatur warna supaya beda2 tiap sisi kotak
        {
            case 0:
                glColor3f(0, 1, 1);
                break;
            case 1:
                glColor3f(1, 0, 0);
                break;
            case 2:
                glColor3f(0, 1, 0);
                break;
            case 3:
                glColor3f(0, 0, 1);
                break;
            case 4:
                glColor3f(1, 1, 0);
                break;
            case 5:
                glColor3f(1, 0, 1);
                break;
            default:
                glColor3f(1, 1, 1);
        }
        
        // Gambar
        glVertex3f(box[i].x, box[i].y, box[i].z);
        
        // Transformasi
        box[i] = rx * box[i];
        box[i] = ry * box[i];
        box[i] = rz * box[i];
        box[i] = dilate * box[i];
        box[i] = box[i] + translate;
    }
    glEnd();  // End of drawing color-cube
    
    glBegin(GL_QUADS);// Define vertices in counter-clockwise (CCW) order with normal pointing out
    
    for (int i = 0; i < 24; i++) {
        switch ((i+1)/4) // cuma buat ngatur warna supaya beda2 tiap sisi kotak
        {
            case 0:
                glColor3f(0, 1, 1);
                break;
            case 1:
                glColor3f(1, 0, 0);
                break;
            case 2:
                glColor3f(0, 1, 0);
                break;
            case 3:
                glColor3f(0, 0, 1);
                break;
            case 4:
                glColor3f(1, 1, 0);
                break;
            case 5:
                glColor3f(1, 0, 1);
                break;
            default:
                glColor3f(1, 1, 1);
        }
        
        // Gambar
        glVertex3f(box2[i].x, box2[i].y, box2[i].z);
        
        // Transformasi
        box2[i] = rx * box2[i];
        box2[i] = ry * box2[i];
        box2[i] = rz * box2[i];
        box2[i] = dilate * box2[i];
        box2[i] = box2[i] + translate;
    }
    glEnd();  // End of drawing color-cube
    
    glBegin(GL_QUADS);// Define vertices in counter-clockwise (CCW) order with normal pointing out
    
    for (int i = 0; i < 24; i++) {
        glColor3f(1.0f, 0.0f, 0.0f);

        
        // Gambar
        glVertex3f(lamp1[i].x, lamp1[i].y, lamp1[i].z);
        
        // Transformasi
        lamp1[i] = rx * lamp1[i];
        lamp1[i] = ry * lamp1[i];
        lamp1[i] = rz * lamp1[i];
        lamp1[i] = dilate * lamp1[i];
        lamp1[i] = lamp1[i] + translate;
    }
    glEnd();  // End of drawing color-cube

    glBegin(GL_QUADS);// Define vertices in counter-clockwise (CCW) order with normal pointing out
    
    for (int i = 0; i < 24; i++) {
        glColor3f(1.0f, 0.0f, 0.0f);

        
        // Gambar
        glVertex3f(lamp2[i].x, lamp2[i].y, lamp2[i].z);
        
        // Transformasi
        lamp2[i] = rx * lamp2[i];
        lamp2[i] = ry * lamp2[i];
        lamp2[i] = rz * lamp2[i];
        lamp2[i] = dilate * lamp2[i];
        lamp2[i] = lamp2[i] + translate;
    }
    glEnd();  // End of drawing color-cube

    glBegin(GL_QUADS);// Define vertices in counter-clockwise (CCW) order with normal pointing out
    
    for (int i = 0; i < 24; i++) {
        glColor3f(1.0f, 1.0f, 0.0f);

        
        // Gambar
        glVertex3f(lamp3[i].x, lamp3[i].y, lamp3[i].z);
        
        // Transformasi
        lamp3[i] = rx * lamp3[i];
        lamp3[i] = ry * lamp3[i];
        lamp3[i] = rz * lamp3[i];
        lamp3[i] = dilate * lamp3[i];
        lamp3[i] = lamp3[i] + translate;
    }
    glEnd();  // End of drawing color-cube

    glBegin(GL_QUADS);// Define vertices in counter-clockwise (CCW) order with normal pointing out
    
    for (int i = 0; i < 24; i++) {
        glColor3f(1.0f, 1.0f, 0.0f);

        
        // Gambar
        glVertex3f(lamp4[i].x, lamp4[i].y, lamp4[i].z);
        
        // Transformasi
        lamp4[i] = rx * lamp4[i];
        lamp4[i] = ry * lamp4[i];
        lamp4[i] = rz * lamp4[i];
        lamp4[i] = dilate * lamp4[i];
        lamp4[i] = lamp4[i] + translate;
    }
    glEnd();  // End of drawing color-cube

    glBegin(GL_QUADS);// Define vertices in counter-clockwise (CCW) order with normal pointing out
    
    for (int i = 0; i < 4; i++) {
        glColor3f(0.0f, 0.0f, 1.0f);
        //GREY COLOR
        //glColor3f(0.329412f, 0.329412f, 0.329412f);
        // Gambar
        glVertex3f(window[i].x, window[i].y, window[i].z);
        
        // Transformasi
        window[i] = rx * window[i];
        window[i] = ry * window[i];
        window[i] = rz * window[i];
        window[i] = dilate * window[i];
        window[i] = window[i] + translate;
    }
    glEnd();  // End of drawing color-cube
    
    glBegin(GL_QUADS);// Define vertices in counter-clockwise (CCW) order with normal pointing out
    
    for (int i = 0; i < 4; i++) {
        glColor3f(0.0f, 0.0f, 1.0f);
        
        // Gambar
        glVertex3f(window2[i].x, window2[i].y, window2[i].z);
        
        // Transformasi
        window2[i] = rx * window2[i];
        window2[i] = ry * window2[i];
        window2[i] = rz * window2[i];
        window2[i] = dilate * window2[i];
        window2[i] = window2[i] + translate;
    }
    glEnd();  // End of drawing color-cube
    
    glBegin(GL_QUADS);// Define vertices in counter-clockwise (CCW) order with normal pointing out
    
    for (int i = 0; i < 4; i++) {
        glColor3f(0.0f, 0.0f, 1.0f);
        
        // Gambar
        glVertex3f(window3[i].x, window3[i].y, window3[i].z);
        
        // Transformasi
        window3[i] = rx * window3[i];
        window3[i] = ry * window3[i];
        window3[i] = rz * window3[i];
        window3[i] = dilate * window3[i];
        window3[i] = window3[i] + translate;
    }
    glEnd();  // End of drawing color-cube
    
    glBegin(GL_QUADS);// Define vertices in counter-clockwise (CCW) order with normal pointing out
    
    for (int i = 0; i < 4; i++) {
        glColor3f(0.0f, 0.0f, 1.0f);
        
        // Gambar
        glVertex3f(window4[i].x, window4[i].y, window4[i].z);
        
        // Transformasi
        window4[i] = rx * window4[i];
        window4[i] = ry * window4[i];
        window4[i] = rz * window4[i];
        window4[i] = dilate * window4[i];
        window4[i] = window4[i] + translate;
    }
    glEnd();  // End of drawing color-cube
    
    glBegin(GL_QUADS);// Define vertices in counter-clockwise (CCW) order with normal pointing out
    
    for (int i = 0; i < 4; i++) {
        glColor3f(0.0f, 0.0f, 1.0f);
        
        // Gambar
        glVertex3f(window5[i].x, window5[i].y, window5[i].z);
        
        // Transformasi
        window5[i] = rx * window5[i];
        window5[i] = ry * window5[i];
        window5[i] = rz * window5[i];
        window5[i] = dilate * window5[i];
        window5[i] = window5[i] + translate;
    }
    glEnd();  // End of drawing color-cube
    
    glBegin(GL_QUADS);// Define vertices in counter-clockwise (CCW) order with normal pointing out
    
    for (int i = 0; i < 4; i++) {
        glColor3f(0.0f, 0.0f, 1.0f);
        
        // Gambar
        glVertex3f(window6[i].x, window6[i].y, window6[i].z);
        
        // Transformasi
        window6[i] = rx * window6[i];
        window6[i] = ry * window6[i];
        window6[i] = rz * window6[i];
        window6[i] = dilate * window6[i];
        window6[i] = window6[i] + translate;
    }
    glEnd();  // End of drawing color-cube

    glBegin(GL_QUADS);// Define vertices in counter-clockwise (CCW) order with normal pointing out
    
    for (int i = 0; i < 4; i++) {
        glColor3f(0.329412f, 0.329412f, 0.329412f);
        
        // Gambar
        glVertex3f(door1[i].x, door1[i].y, door1[i].z);
        
        // Transformasi
        door1[i] = rx * door1[i];
        door1[i] = ry * door1[i];
        door1[i] = rz * door1[i];
        door1[i] = dilate * door1[i];
        door1[i] = door1[i] + translate;
    }
    glEnd();  // End of drawing color-cube

    glBegin(GL_QUADS);// Define vertices in counter-clockwise (CCW) order with normal pointing out
    
    for (int i = 0; i < 4; i++) {
        glColor3f(0.329412f, 0.329412f, 0.329412f);
        
        // Gambar
        glVertex3f(door2[i].x, door2[i].y, door2[i].z);
        
        // Transformasi
        door2[i] = rx * door2[i];
        door2[i] = ry * door2[i];
        door2[i] = rz * door2[i];
        door2[i] = dilate * door2[i];
        door2[i] = door2[i] + translate;
    }
    glEnd();  // End of drawing color-cube

    glBegin(GL_QUADS);// Define vertices in counter-clockwise (CCW) order with normal pointing out
    
    for (int i = 0; i < 4; i++) {
        glColor3f(0.329412f, 0.329412f, 0.329412f);
        
        // Gambar
        glVertex3f(handle1[i].x, handle1[i].y, handle1[i].z);
        
        // Transformasi
        handle1[i] = rx * handle1[i];
        handle1[i] = ry * handle1[i];
        handle1[i] = rz * handle1[i];
        handle1[i] = dilate * handle1[i];
        handle1[i] = handle1[i] + translate;
    }
    glEnd();  // End of drawing color-cube

    glBegin(GL_QUADS);// Define vertices in counter-clockwise (CCW) order with normal pointing out
    
    for (int i = 0; i < 4; i++) {
        glColor3f(0.329412f, 0.329412f, 0.329412f);
        
        // Gambar
        glVertex3f(handle2[i].x, handle2[i].y, handle2[i].z);
        
        // Transformasi
        handle2[i] = rx * handle2[i];
        handle2[i] = ry * handle2[i];
        handle2[i] = rz * handle2[i];
        handle2[i] = dilate * handle2[i];
        handle2[i] = handle2[i] + translate;
    }
    glEnd();  // End of drawing color-cube

    glBegin(GL_QUADS);// Define vertices in counter-clockwise (CCW) order with normal pointing out
    
    for (int i = 0; i < 4; i++) {
        glColor3f(0.329412f, 0.329412f, 0.329412f);
        
        // Gambar
        glVertex3f(handle3[i].x, handle3[i].y, handle3[i].z);
        
        // Transformasi
        handle3[i] = rx * handle3[i];
        handle3[i] = ry * handle3[i];
        handle3[i] = rz * handle3[i];
        handle3[i] = dilate * handle3[i];
        handle3[i] = handle3[i] + translate;
    }
    glEnd();  // End of drawing color-cube

    glBegin(GL_QUADS);// Define vertices in counter-clockwise (CCW) order with normal pointing out
    
    for (int i = 0; i < 4; i++) {
        glColor3f(0.329412f, 0.329412f, 0.329412f);
        
        // Gambar
        glVertex3f(handle4[i].x, handle4[i].y, handle4[i].z);
        
        // Transformasi
        handle4[i] = rx * handle4[i];
        handle4[i] = ry * handle4[i];
        handle4[i] = rz * handle4[i];
        handle4[i] = dilate * handle4[i];
        handle4[i] = handle4[i] + translate;
    }
    glEnd();  // End of drawing color-cube

	glBegin(GL_QUADS);// Define vertices in counter-clockwise (CCW) order with normal pointing out
	for (int i = 0; i < 24; i++) {
		glColor3f(1, 1, 1);


		// Gambar
		glVertex3f(wheel1[i].x, wheel1[i].y, wheel1[i].z);

		// Transformasi
		wheel1[i] = rx * wheel1[i];
		wheel1[i] = ry * wheel1[i];
		wheel1[i] = rz * wheel1[i];
		wheel1[i] = dilate * wheel1[i];
		wheel1[i] = wheel1[i] + translate;
	}
	glEnd();  // End of drawing color-cube

	glBegin(GL_QUADS);// Define vertices in counter-clockwise (CCW) order with normal pointing out
	for (int i = 0; i < 24; i++) {
		glColor3f(1, 1, 1);
		// Gambar
		glVertex3f(wheel2[i].x, wheel2[i].y, wheel2[i].z);

		// Transformasi
		wheel2[i] = rx * wheel2[i];
		wheel2[i] = ry * wheel2[i];
		wheel2[i] = rz * wheel2[i];
		wheel2[i] = dilate * wheel2[i];
		wheel2[i] = wheel2[i] + translate;
	}
	glEnd();  // End of drawing color-cube

	glBegin(GL_QUADS);// Define vertices in counter-clockwise (CCW) order with normal pointing out
	for (int i = 0; i < 24; i++) {
		glColor3f(1, 1, 1);
		// Gambar
		glVertex3f(wheel3[i].x, wheel3[i].y, wheel3[i].z);

		// Transformasi
		wheel3[i] = rx * wheel3[i];
		wheel3[i] = ry * wheel3[i];
		wheel3[i] = rz * wheel3[i];
		wheel3[i] = dilate * wheel3[i];
		wheel3[i] = wheel3[i] + translate;
	}
	glEnd();  // End of drawing color-cube

	glBegin(GL_QUADS);// Define vertices in counter-clockwise (CCW) order with normal pointing out
	for (int i = 0; i < 24; i++) {
		glColor3f(1, 1, 1);
		// Gambar
		glVertex3f(wheel4[i].x, wheel4[i].y, wheel4[i].z);

		// Transformasi
		wheel4[i] = rx * wheel4[i];
		wheel4[i] = ry * wheel4[i];
		wheel4[i] = rz * wheel4[i];
		wheel4[i] = dilate * wheel4[i];
		wheel4[i] = wheel4[i] + translate;
	}
	glEnd();  // End of drawing color-cube
    
    glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}

/* Handler for window re-size event. Called back when the window first appears and
 whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    
    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);
    
    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void redisplay() {
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            degreeX = ROTATE_DEGREE;
            break;
        case 's':
            degreeX = -ROTATE_DEGREE;
            break;
        case 'd':
            degreeY = ROTATE_DEGREE;
            break;
        case 'a':
            degreeY = -ROTATE_DEGREE;
            break;
        case 'q':
            degreeZ = ROTATE_DEGREE;
            break;
        case 'e':
            degreeZ = -ROTATE_DEGREE;
            break;
        case ',':
            scaleFactor = 1 - SCALE_FACTOR_DELTA;
            break;
        case '.':
            scaleFactor = 1 + SCALE_FACTOR_DELTA;
            break;
        case '[':
            tz = TRANSLATE_CONSTANT;
            break;
        case ']':
            tz = -TRANSLATE_CONSTANT;
            break;
        default:
            break;
    }
}

void keyboardUp(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            degreeX = 0;
            break;
        case 's':
            degreeX = 0;
            break;
        case 'd':
            degreeY = 0;
            break;
        case 'a':
            degreeY = 0;
            break;
        case 'q':
            degreeZ = 0;
            break;
        case 'e':
            degreeZ = 0;
            break;
        case ',':
            scaleFactor = 1;
            break;
        case '.':
            scaleFactor = 1;
            break;
        case '[':
            tz = 0;
            break;
        case ']':
            tz = 0;
            break;
        default:
            break;
    }
}

void specialInput(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            ty = TRANSLATE_CONSTANT;
            break;
        case GLUT_KEY_DOWN:
            ty = -TRANSLATE_CONSTANT;
            break;
        case GLUT_KEY_LEFT:
            tx = -TRANSLATE_CONSTANT;
            break;
        case GLUT_KEY_RIGHT:
            tx = TRANSLATE_CONSTANT;
            break;
        default:
            break;
    }
}

void specialInputUp(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            ty = 0;
            break;
        case GLUT_KEY_DOWN:
            ty = 0;
            break;
        case GLUT_KEY_LEFT:
            tx = 0;
            break;
        case GLUT_KEY_RIGHT:
            tx = 0;
            break;
        default:
            break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);            // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
    glutInitWindowSize(WIDTH, HEIGHT);   // Set the window's initial width & height
    glutInitWindowPosition(100, 50); // Position the window's initial top-left corner
    glutCreateWindow("Car3D - Transformation");
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(redisplay);
    glutKeyboardUpFunc(keyboardUp);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialInput);
    glutSpecialUpFunc(specialInputUp);
    
    initGL();                       // Our own OpenGL initialization
    glutMainLoop();                 // Enter the infinite event-processing loop
    return 0;
}
