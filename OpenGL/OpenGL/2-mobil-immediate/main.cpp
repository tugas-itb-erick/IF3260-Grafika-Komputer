#include <windows.h>
#include <GL/freeglut.h>
#include "stdafx.h"
#include <iostream>
#include <cstring>
using namespace std;

int w = 1000, h = 1000, res = 1500;
int ntime, ntimes, id;
double resdeg = 0;
int n_vertice, reps;
double x[100], y[100], temp;
double xtemp[100][100], ytemp[100][100];
double degre[100];
double xhasil[100], yhasil[100];
double xawal[100], yawal[100];
double factorx[100], factory[100];
bool finish_animate = true;
bool rotation_animate = false;
bool check_multiple = false;
bool rotated[100];
double param1, param2, param3, param4, deg;
string input_operator;

void init(int argc, char** argv) {
	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600); //Window size
	glutCreateWindow("Devonshire"); //Create a window
	glEnable(GL_DEPTH_TEST); //Make sure 3D drawing works when one object is in front of another
}

//Called when the window is resized
void handleResize(int w, int h) {
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport(0, 0, w, h);
}


void updatematrix() {
	for (int i = 0; i<n_vertice; i++) {
		x[i] = xhasil[i];
		y[i] = yhasil[i];
	}
}

void outputmanual() {
	cout << endl << "Input commands available:" << endl << endl;
	cout << "translate <dx> <dy>" << endl;
	cout << "dilate <k>" << endl;
	cout << "rotate <deg> <a> <b>" << endl;
	cout << "reflect <param>" << endl;
	cout << "shear <param> <k>" << endl;
	cout << "stretch <param> <k>" << endl;
	cout << "custom <a> <b> <c> <d>" << endl;
	cout << "multiple <n>" << endl;
	cout << "reset" << endl;
	cout << "exit" << endl << endl;
}

void welcomescreen() {
	cout << " _____                            _     _       \n";
	cout << "|  __ \\                          | |   (_)            \n";
	cout << "| |  | | _____   _____  _ __  ___| |__  _ _ __ ___           \n";
	cout << "| |  | |/ _ \\ \\ / / _ \\| '_ \\/ __| '_ \\| | '__/ _ \\                 \n";
	cout << "| |__| |  __/\\ V / (_) | | | \\__ \\ | | | | | |  __/                            \n";
	cout << "|_____/ \\___| \\_/ \\___/|_| |_|___/_| |_|_|_|  \\___|                                 ___   ___  \n";
	cout << "| |/ / |              (_)                     |  _ \\                               / _ \\ / _ \\ \n";
	cout << "| ' /| |__   __ _ _ __ _ ___ _ __ ___   __ _  | |_) | __ _ _ __   __ _ ___  __ _  | | | | (_) |\n";
	cout << "|  < | '_ \\ / _` | '__| / __| '_ ` _ \\ / _` | |  _ < / _` | '_ \\ / _` / __|/ _` | | | | |> _ < \n";
	cout << "| . \\| | | | (_| | |  | \\__ \\ | | | | | (_| | | |_) | (_| | | | | (_| \\__ \\ (_| | | |_| | (_) |\n";
	cout << "|_|\\_\\_| |_|\\__,_|_|  |_|___/_| |_| |_|\\__,_| |____/ \\__,_|_| |_|\\__, |___/\\__,_|  \\___/ \\___/ \n";
	cout << "                                                                   __/ |                        \n";
	cout << "                                                                  |___/          \n";

	cout << "BY : " << endl << "13516062 Yusuf Rahmat Pratama" << endl << "13516096        Ensof Barhami" << endl << endl << endl;
}

void input(int n) {
	int idx = 1;
	for (int i = 0; i < n; i++) {
		printf("(x%d,y%d) : ", idx, idx);
		scanf("%lf", &xhasil[i]);
		scanf("%lf", &yhasil[i]);
		idx++;
		xawal[i] = xhasil[i];
		yawal[i] = yhasil[i];
		x[i] = 0;
		y[i] = 0;
	}
}

//Draw coordinate lines
void drawline(float x1, float y1, float x2, float y2)
{
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}

void coordinatelines() {
	glClearColor(0, 0, 0, 0);
	glColor3ub(210, 57, 53);

	for (int i = -h / 2; i < h / 2; i += 50)
	{
		if ((int)i == 0) glLineWidth(3.5);
		else if ((int)i % 150 == 0) glLineWidth(2.0);
		else glLineWidth(0.2);
		drawline(-h / 2, float(i), (float)h / 2, float(i));
	}

	for (float i = -w / 2; i < w / 2; i += 50)
	{
		if ((int)i == 0) glLineWidth(3.5);
		else if ((int)i % 150 == 0) glLineWidth(2.0);
		else glLineWidth(0.2);
		drawline(i, -w / 2, i, (float)w / 2);
	}

}


void translate(double(&x1)[100], double(&y1)[100], double inx, double iny) {
	for (int i = 0; i<n_vertice; i++) {
		x1[i] = x1[i] + inx;
		y1[i] = y1[i] + iny;
	}
}

void dilate(double in) {
	for (int i = 0; i<n_vertice; i++) {
		xhasil[i] = xhasil[i] * in;
		yhasil[i] = yhasil[i] * in;
	}
}

void rotate(double(&x1)[100], double(&y1)[100], double deg, double ina, double inb) {
	double temp;
	//Radian
	double rad = deg * (3.141592 / 180);
	translate(x1, y1, -1.0*ina, -1.0*inb);
	for (int i = 0; i<n_vertice; i++) {
		temp = x1[i];
		x1[i] = (x1[i] * cos(rad)) - (y1[i] * sin(rad));
		y1[i] = (temp*sin(rad)) + (y1[i] * cos(rad));
	}
	translate(x1, y1, ina, inb);
}

void getString(string *idx, string *idy, string s) {
	int i = 0;
	int n = s.length();
	int leng = n;
	*idx = "";
	*idy = "";
	bool first = false;
	while (leng--) {
		if (leng != n - 1 && leng != 0) {
			if (!first) {
				if (s[i] == ',') {
					first = true;
				}
				else {
					*idx += s[i];
				}
			}
			else {
				*idy += s[i];
			}
		}
		i++;
	}
}

double toDouble(string s) {
	int n_string = s.length();
	int comma = 0;
	double x = 0;
	bool negative = false;
	for (int i = 0; i<n_string; i++) {
		if (s[i] == '-') {
			negative = true;
		}
		else if (s[i] != '.') {
			x *= 10;
			x += ((int)s[i] - '0');
		}
		else {
			comma = n_string - i - 1;
		}
	}
	while (comma--) {
		x /= 10;
	}
	if (negative)
		x = x * (-1);
	return x;
}

void reflect(string s) {
	if (s.compare("x") == 0) {
		for (int i = 0; i<n_vertice; i++) {
			yhasil[i] *= -1;
		}
	}
	else if (s.compare("y") == 0) {
		for (int i = 0; i<n_vertice; i++) {
			xhasil[i] *= -1;
		}
	}
	else if (s.compare("y=x") == 0) {
		for (int i = 0; i<n_vertice; i++) {
			double temp = xhasil[i];
			xhasil[i] = yhasil[i];
			yhasil[i] = temp;
		}
	}
	else if (s.compare("y=-x") == 0) {
		for (int i = 0; i<n_vertice; i++) {
			double temp = xhasil[i];
			xhasil[i] = -yhasil[i];
			yhasil[i] = -temp;
		}
	}
	else {
		string X, Y;
		getString(&X, &Y, s);
		double xx = toDouble(X);
		double yy = toDouble(Y);
		for (int i = 0; i<n_vertice; i++) {
			xhasil[i] = 2 * xx - xhasil[i];
			yhasil[i] = 2 * yy - yhasil[i];
		}
	}
}

void shear(string inparam, double ink) {
	if (inparam == "x") {
		for (int i = 0; i<n_vertice; i++) {
			xhasil[i] += ink * yhasil[i];
		}
	}
	else if (inparam == "y") {
		for (int i = 0; i<n_vertice; i++) {
			yhasil[i] += ink * xhasil[i];
		}
	}
}

void stretch(string inparam, double ink) {
	if (inparam == "x") {
		for (int i = 0; i<n_vertice; i++) {
			xhasil[i] *= ink;
		}
	}
	else if (inparam == "y") {
		for (int i = 0; i<n_vertice; i++) {
			yhasil[i] *= ink;
		}
	}
}

void custom(double ina, double inb, double inc, double ind) {
	for (int i = 0; i<n_vertice; i++) {
		temp = xhasil[i];
		xhasil[i] = xhasil[i] * ina + yhasil[i] * inb;
		yhasil[i] = temp * inc + yhasil[i] * ind;
	}
}

void reset() {
	check_multiple = false;
	for (int i = 0; i<n_vertice; i++) {
		xhasil[i] = xawal[i];
		yhasil[i] = yawal[i];
	}
}


void insertfactor() {
	for (int i = 0; i<n_vertice; i++) {
		factorx[i] = (xhasil[i] - x[i]) / ((double)res);
		factory[i] = (yhasil[i] - y[i]) / ((double)res);
	}
}

void readyanimate() {
	reps = res;
	finish_animate = false;
	rotation_animate = false;
	insertfactor();
}

void readyrotate() {
	reps = res;
	finish_animate = false;
	rotation_animate = true;
	resdeg = deg / ((double)res);
}

void animaterotate() {
	if (reps > 0) {
		rotate(x, y, resdeg, param1, param2);
		reps--;
	}
	else if (reps == 0) {
		updatematrix();
		reps--;
	}
	else {
		finish_animate = true;
		rotation_animate = false;
		resdeg = 0;
	}
}

void animatemove() {
	if (reps > 0) {
		for (int i = 0; i<n_vertice; i++) {
			x[i] += factorx[i];
			y[i] += factory[i];
		}
		reps--;
	}
	else if (reps == 0) {
		updatematrix();
		reps--;
	}
	else {
		finish_animate = true;
	}
}

void move_object(bool rotat) {
	if (rotat) {
		animaterotate();
	}
	else {
		animatemove();
	}
}

bool transformation(string input_operator) {
	return  input_operator == "translate" || input_operator == "dilate" ||
		input_operator == "rotate" || input_operator == "reflect" ||
		input_operator == "shear" || input_operator == "stretch" ||
		input_operator == "custom";
}

void linear_transformation(string input_operator) {
	string act;
	if (input_operator == "translate") {
		cout << "insert dx and dy\n";
		cin >> param1 >> param2;
		translate(xhasil, yhasil, param1, param2);
		if (!check_multiple)
			readyanimate();
	}
	else if (input_operator == "dilate") {
		cout << "insert factor scalling\n";
		cin >> param1;
		dilate(param1);
		if (!check_multiple)
			readyanimate();
	}
	else if (input_operator == "rotate") {
		cout << "insert degree and coordinate\n";
		cin >> deg >> param1 >> param2;
		rotate(xhasil, yhasil, deg, param1, param2);
		if (!check_multiple)
			readyrotate();
	}
	else if (input_operator == "reflect") {
		cout << "y=x / y=-x / y / x / (x0,y0)\n";
		string s;
		cin >> s;
		reflect(s);
		if (!check_multiple)
			readyanimate();
	}
	else if (input_operator == "shear") {
		cout << "insert x/y and factor shearing\n";
		cin >> act >> param1;
		shear(act, param1);
		if (!check_multiple)
			readyanimate();
	}
	else if (input_operator == "stretch") {
		cout << "insert x/y and factor stretching\n";
		cin >> act >> param1;
		stretch(act, param1);
		if (!check_multiple)
			readyanimate();
	}
	else if (input_operator == "custom") {
		cout << "insert a b c d\n";
		cin >> param1 >> param2 >> param3 >> param4;
		custom(param1, param2, param3, param4);
		if (!check_multiple)
			readyanimate();
	}
}

//Draws the 3D scene
void shapes() {
	//Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w * 0.5, w*0.5, -h * 0.5, h*0.5, 0.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective
	coordinatelines();
	glBegin(GL_POLYGON);
	for (int i = 0; i<n_vertice; i++) {
		glVertex2f(x[i], y[i]);
	}
	glEnd();
	if (finish_animate) {
		if (!check_multiple) {
			cin >> input_operator;
			if (transformation(input_operator)) {
				linear_transformation(input_operator);
			}
			else if (input_operator == "multiple") {
				memset(rotated, false, sizeof(rotated));
				check_multiple = true;
				cout << "How many transformations : ";
				int i = 0;
				id = 0;
				cin >> ntimes;
				ntime = ntimes;
				while (ntimes--) {
					cout << "[Press 0 for info]\ninsert transformation " << i + 1 << " : ";
					cin >> input_operator;
					if (transformation(input_operator)) {
						linear_transformation(input_operator);
						for (int j = 0; j<n_vertice; j++) {
							xtemp[i][j] = xhasil[j];
							ytemp[i][j] = yhasil[j];
						}
						if (input_operator == "rotate") {
							degre[i] = deg;
							rotated[i] = true;
						}
						i++;
					}
					else if (input_operator == "0") {
						ntimes++;
						outputmanual();
					}
					else {
						ntimes++;
						cout << "you entered wrong input\n";
					}
				}
				ntimes = ntime;
			}
			else if (input_operator == "reset") {
				reset();
				readyanimate();
			}
			else if (input_operator == "exit") {
				exit(0);
			}
			else if (input_operator == "0") {
				outputmanual();
			}
			else {
				cout << "you entered wrong input\n";
			}
		}
		if (check_multiple) {
			for (int j = 0; j<n_vertice; j++) {
				xhasil[j] = xtemp[id][j];
				yhasil[j] = ytemp[id][j];
			}
			if (rotated[id]) {
				deg = degre[id];
				readyrotate();
			}
			else {
				rotation_animate = false;
				readyanimate();
			}
			ntimes--;
			id++;
			if (ntimes == 0) {
				id = 0;
				check_multiple = false;
			}
		}
	}
	move_object(rotation_animate);
	glutSwapBuffers();
}


void draw() {
	glutDisplayFunc(shapes);
	glutIdleFunc(shapes);
}

int main(int argc, char** argv) {
	init(argc, argv);
	welcomescreen();
	printf("Input Total Vertices : "); scanf("%d", &n_vertice);
	input(n_vertice);
	cout << "\nPress 0 for info\n";
	readyanimate();
	draw();
	glutReshapeFunc(handleResize);
	glutMainLoop();
	return 0;
}