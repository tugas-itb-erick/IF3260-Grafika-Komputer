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
	glm::vec3(1.0f, 1.0f, -1.0f),
	glm::vec3(-1.0f, 1.0f, -1.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),

	glm::vec3(1.0f, -1.0f, 1.0f),
	glm::vec3(-1.0f, -1.0f, 1.0f),
	glm::vec3(-1.0f, -1.0f, -1.0f),
	glm::vec3(1.0f, -1.0f, -1.0f),

	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f),
	glm::vec3(-1.0f, -1.0f, 1.0f),
	glm::vec3(1.0f, -1.0f, 1.0f),

	glm::vec3(1.0f, -1.0f, -1.0f),
	glm::vec3(-1.0f, -1.0f, -1.0f),
	glm::vec3(-1.0f, 1.0f, -1.0f),
	glm::vec3(1.0f, 1.0f, -1.0f),

	glm::vec3(-1.0f, 1.0f, 1.0f),
	glm::vec3(-1.0f, 1.0f, -1.0f),
	glm::vec3(-1.0f, -1.0f, -1.0f),
	glm::vec3(-1.0f, -1.0f, 1.0f),

	glm::vec3(1.0f, 1.0f, -1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, -1.0f, 1.0f),
	glm::vec3(1.0f, -1.0f, -1.0f)
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

	glBegin(GL_QUADS); // Define vertices in counter-clockwise (CCW) order with normal pointing out
	
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