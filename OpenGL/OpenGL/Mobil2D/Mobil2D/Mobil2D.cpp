// Include standard headers
#include "stdafx.h"

#include <GL/glut.h>
GLfloat angle = 45.0f;
int refreshmill = 1;

void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(refreshmill, timer, 0);
}

void whiteStud() {
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(0.01, 16, 16);
}

void blackWheel() {
	glColor3f(0.129412, 0.129412, 0.129412);
	glutSolidSphere(0.1, 16, 16);
}

void wheelWithStud() {
	int iTimeElapsed = glutGet(GLUT_ELAPSED_TIME);
	float fScale = 0.5f;
	long i;
	glPushMatrix();
	glTranslatef(0.25, 0.25, 0.0);
	glRotatef(iTimeElapsed * fScale, 0.0, 0.0, 1.0);
	blackWheel(); // draw the wheel without studs.
				  // five studs, step 72 degree (72*5=360 degree).
	for (i = 0; i<5; i++) {
		glPushMatrix();
		glRotatef(72 * i, 0.0, 0.0, 1.0); // rotate coordinate 72 degree.
		glTranslatef(0.04, 0.0, 0.0); // translate on the rotated coordinate.
		whiteStud(); // draw the stud.
		glPopMatrix();
	}
	glTranslatef(0.0, 0.0, 0.0); // translate in order to draw a stud at the center of the wheel.
	whiteStud(); // draw the stud at the center of the wheel.
	glFlush();
	glPopMatrix();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glColor3f(1.0, 1.0, 0.0);
	//car lower body
	glBegin(GL_POLYGON);
	glVertex3f(-0.75, 0.0, 0);
	glVertex3f(-0.75, -0.2, 0);
	glVertex3f(0.75, -0.2, 0);
	glVertex3f(0.75, 0.0, 0);
	glEnd();
	//car upper body
	glBegin(GL_POLYGON);
	glVertex3f(-0.5, 0.0, 0);
	glVertex3f(0.75, 0.0, 0);
	glVertex3f(0.45, 0.2, 0);
	glVertex3f(-0.2, 0.2, 0);
	glEnd();
	//car window
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex3f(-0.2, 0.0, 0);
	glVertex3f(0.1, 0.0, 0);
	glVertex3f(0.1, 0.16, 0);
	glVertex3f(-0.2, 0.16, 0);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(0.15, 0.0, 0);
	glVertex3f(0.45, 0.0, 0);
	glVertex3f(0.45, 0.16, 0);
	glVertex3f(0.15, 0.16, 0);
	glEnd();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(0.25f, -0.5f, 0.0f);
	//glRotatef(angle, 0.0f, 0.0f, 1.0f);
	wheelWithStud();
	glPopMatrix();
	glutSwapBuffers();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(-0.75f, -0.5f, 0.0f);
	//glRotatef(angle, 0.0f, 0.0f, 1.0f);
	wheelWithStud();
	glPopMatrix();
	glutSwapBuffers();

	angle += 0.2;
	//glFlush();
}

void reshape(GLsizei width, GLsizei height) {
	if (height == 0) height = 1;
	GLfloat aspect = (GLfloat)width / (GLfloat)height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (width >= height) {
		gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
	}
	else {
		gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
	}
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("example of geometric primitive");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
	glutMainLoop();
	return 0;
}