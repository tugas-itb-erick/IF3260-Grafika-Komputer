// Std. Includes
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include <math.h>
#include <GL/gl.h>
#include <GL/glut.h>

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void ScrollCallback( GLFWwindow *window, double xOffset, double yOffset );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );

// Camera
Camera  camera(glm::vec3( 0.0f, 2.0f, 8.0f ) );
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLfloat middlePoint[] = {0, 0, 0};
GLfloat rad = 0.00f;
const int carLastIndex = 36 * 12 * 8; // 3456
GLfloat vertices[] =
    {
        // [Badan mobil bawah]
        // sisi 1
        1.0f, -0.33f, -2.33f,   1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
        1.0f,  1.0f, -2.33f,    1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        1.0f,  1.0f, 3.67f,     1.0f, 1.0f, 1.0f,     1.0f, 1.0f,
        1.0f, -0.33f, -2.33f,   1.0f, 1.0f, 1.0f,     1.0f, 1.0f,
        1.0f, -0.33f, 3.67f,    1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        1.0f,  1.0f,  3.67f,    1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
        // sisi 2
        -1.0f, -0.33f, 3.67f,   0.0f, 0.0f, 0.0f,     0.0f, 0.0f,
        -1.0f, 1.0f, 3.67f,     0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        -1.0f, 1.0f, -2.33f,    0.0f, 0.0f, 0.0f,     1.0f, 1.0f,
        -1.0f, -0.33f, 3.67f,   0.0f, 0.0f, 0.0f,     1.0f, 1.0f,
        -1.0f, -0.33f, -2.33f,  0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        -1.0f, 1.0f, -2.33f,    0.0f, 0.0f, 0.0f,     0.0f, 0.0f,
        // sisi 3
        -1.0f, -0.33f, -2.33f,  0.0f, 0.0f, 0.0f,     0.0f, 0.0f,
        -1.0f, 1.0f, -2.33f,    0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        1.0f, 1.0f, -2.33f,     0.0f, 0.0f, 0.0f,     1.0f, 1.0f,
        -1.0f, -0.33f, -2.33f,  0.0f, 0.0f, 0.0f,     1.0f, 1.0f,
        1.0f, -0.33f, -2.33f,   0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        1.0f, 1.0f, -2.33f,     0.0f, 0.0f, 0.0f,     0.0f, 0.0f,
        // sisi 4
        1.0f, -0.33f, 3.67f,    1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
        1.0f, 1.0f, 3.67f,      1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        -1.0f, 1.0f, 3.67f,     1.0f, 1.0f, 1.0f,     1.0f, 1.0f,
        1.0f, -0.33f, 3.67f,    1.0f, 1.0f, 1.0f,     1.0f, 1.0f,
        -1.0f, -0.33f, 3.67f,   1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        -1.0f, 1.0f, 3.67f,     1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
        // sisi 5
        1.0f, -0.33f, 3.67f,    0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        -1.0f, -0.33f, 3.67f,   0.0f, 0.0f, 0.0f,     1.0f, 1.0f,
        -1.0f, -0.33f, -2.33f,  0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        1.0f, -0.33f, 3.67f,    0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        1.0f, -0.33f, -2.33f,   0.0f, 0.0f, 0.0f,     0.0f, 0.0f,
        -1.0f, -0.33f, -2.33f,  0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        // sisi 6
        1.0f, 1.0f, 3.67f,      1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        -1.0f, 1.0f, 3.67f,     1.0f, 1.0f, 1.0f,     1.0f, 1.0f,
        -1.0f, 1.0f, -2.33f,    1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        1.0f, 1.0f, 3.67f,      1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        1.0f, 1.0f, -2.33f,     1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
        -1.0f, 1.0f, -2.33f,    1.0f, 1.0f, 1.0f,     0.0f, 1.0f,

        // [Badan mobil atas]
        // sisi 1
        -1.0f, 1.0f, -1.33f,    0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        -1.0f, 2.33f, 0.0f,     0.0f, 0.0f, 0.0f,     1.0f, 1.0f,
        1.0f, 2.33f, 0.0f,      0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        -1.0f, 1.0f, -1.33f,    0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        1.0f, 1.0f, -1.33f,     0.0f, 0.0f, 0.0f,     0.0f, 0.0f,
        1.0f, 2.33f, 0.0f,      0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        // sisi 2
        -1.0f, 1.0f, 3.0f,      0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        -1.0f, 2.33f, 2.33f,    0.0f, 0.0f, 0.0f,     1.0f, 1.0f,
        -1.0f, 2.33f, 0.0f,     0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        -1.0f, 1.0f, 3.0f,      0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        -1.0f, 1.0f, -1.33f,    0.0f, 0.0f, 0.0f,     0.0f, 0.0f,
        -1.0f, 2.33f, 0.0f,     0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        // sisi 3
        1.0f, 1.0f, 3.0f,       1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        1.0f, 2.33f, 2.33f,     1.0f, 1.0f, 1.0f,     1.0f, 1.0f,
        -1.0f, 2.33f, 2.33f,    1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        1.0f, 1.0f, 3.0f,       1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        -1.0f, 1.0f, 3.0f,      1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
        -1.0f, 2.33f, 2.33f,    1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        // sisi 4
        1.0f, 1.0f, -1.33f,     1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        1.0f, 2.33f, 0.0f,      1.0f, 1.0f, 1.0f,     1.0f, 1.0f,
        1.0f, 2.33f, 2.33f,     1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        1.0f, 1.0f, -1.33f,     1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        1.0f, 1.0f, 3.0f,       1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
        1.0f, 2.33f, 2.33f,     1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        // sisi 5
        -1.0f, 2.33f, 0.0f,     1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        1.0f, 2.33f, 0.0f,      1.0f, 1.0f, 1.0f,     1.0f, 1.0f,
        1.0f, 2.33f, 2.33f,     1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        -1.0f, 2.33f, 0.0f,     1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        -1.0f, 2.33f, 2.33f,    1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
        1.0f, 2.33f, 2.33f,     1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        // sisi 6
        1.0f, 1.0f, 3.67f,      0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        -1.0f, 1.0f, 3.67f,     0.0f, 0.0f, 0.0f,     1.0f, 1.0f,
        -1.0f, 1.0f, -2.33f,    0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        1.0f, 1.0f, 3.67f,      0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        1.0f, 1.0f, -2.33f,     0.0f, 0.0f, 0.0f,     0.0f, 0.0f,
        -1.0f, 1.0f, -2.33f,    0.0f, 0.0f, 0.0f,     0.0f, 1.0f,

        // [Jendela Belakang]
        0.67f, 1.33f, 2.85f,    1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        0.67f, 2.0f, 2.52f,     1.0f, 1.0f, 1.0f,     1.0f, 1.0f,
        -0.67f, 2.0f, 2.52f,    1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        0.67f, 1.33f, 2.85f,    1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        -0.67f, 1.33f, 2.85f,   1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
        -0.67f, 2.0f, 2.52f,    1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        // [Jendela Sisi Belakang]
        1.01f, 1.0f, 1.33f,     1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        1.01f, 2.0f, 1.33f,     1.0f, 1.0f, 1.0f,     1.0f, 1.0f,
        1.01f, 2.0f, 2.0f,      1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        1.01f, 1.0f, 1.33f,     1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        1.01f, 1.0f, 2.67f,     1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
        1.01f, 2.0f, 2.0f,      1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        // [Jendela Sisi Depan]
        1.01f, 1.0f, -1.0f,     1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        1.01f, 2.0f, 0.0f,      1.0f, 1.0f, 1.0f,     1.0f, 1.0f,
        1.01f, 2.0f, 0.67f,     1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        1.01f, 1.0f, -1.0f,     1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        1.01f, 1.0f, 0.67f,     1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
        1.01f, 2.0f, 0.67f,     1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        // [Jendela Depan]
        -0.67f, 1.33f, -1.01f,  0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        -0.67f, 2.0f, -0.34f,   0.0f, 0.0f, 0.0f,     1.0f, 1.0f,
        0.67f, 2.0f, -0.34f,    0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        -0.67f, 1.33f, -1.01f,  0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        0.67f, 1.33f, -1.01f,   0.0f, 0.0f, 0.0f,     0.0f, 0.0f,
        0.67f, 2.0f, -0.34f,    0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        // [Jendela Sisi Depan 2]
        -1.01f, 1.0f, 0.67f,    0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        -1.01f, 2.0f, 0.67f,    0.0f, 0.0f, 0.0f,     1.0f, 1.0f,
        -1.01f, 2.0f, 0.0f,     0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        -1.01f, 1.0f, 0.67f,    0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        -1.01f, 1.0f, -1.0f,    0.0f, 0.0f, 0.0f,     0.0f, 0.0f,
        -1.01f, 2.0f, 0.0f,     0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        // [Jendela Sisi Belakang 2]
        -1.01f, 1.0f, 2.67f,    0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        -1.01f, 2.0f, 2.0f,     0.0f, 0.0f, 0.0f,     1.0f, 1.0f,
        -1.01f, 2.0f, 1.33f,    0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        -1.01f, 1.0f, 2.67f,    0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        -1.01f, 1.0f, 1.33f,    0.0f, 0.0f, 0.0f,     0.0f, 0.0f,
        -1.01f, 2.0f, 1.33f,    0.0f, 0.0f, 0.0f,     0.0f, 1.0f,

        // [Pintu 1]
        -1.01f, 2.33f, 1.01f,   0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        -1.01f, 2.33f, 0.99f,   0.0f, 0.0f, 0.0f,     1.0f, 1.0f,
        -1.01f, -0.33f, 0.99f,  0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        -1.01f, 2.33f, 1.01f,   0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        -1.01f, -0.33f, 1.01f,  0.0f, 0.0f, 0.0f,     0.0f, 0.0f,
        -1.01f, -0.33f, 0.99f,  0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        // [Pintu 2]
        1.01f, 2.33f, 1.01f,    1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        1.01f, 2.33f, 0.99f,    1.0f, 1.0f, 1.0f,     1.0f, 1.0f,
        1.01f, -0.33f, 0.99f,   1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        1.01f, 2.33f, 1.01f,    1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        1.01f, -0.33f, 1.01f,   1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
        1.01f, -0.33f, 0.99f,   1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        // [Handle 1]
        -1.01f, 0.75f, 1.63f,   0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        -1.01f, 0.75f, 1.33f,   0.0f, 0.0f, 0.0f,     1.0f, 1.0f,
        -1.01f, 0.6f, 1.33f,    0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        -1.01f, 0.75f, 1.63f,   0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        -1.01f, 0.6f, 1.63f,    0.0f, 0.0f, 0.0f,     0.0f, 0.0f,
        -1.01f, 0.6f, 1.33f,    0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        // [Handle 2]
        1.01f, 0.75f, 1.63f,    1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        1.01f, 0.75f, 1.33f,    1.0f, 1.0f, 1.0f,     1.0f, 1.0f,
        1.01f, 0.6f, 1.33f,     1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        1.01f, 0.75f, 1.63f,    1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        1.01f, 0.6f, 1.63f,     1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
        1.01f, 0.6f, 1.33f,     1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        // [Handle 3]
        -1.01f, 0.75f, 0.67f,   0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        -1.01f, 0.75f, 0.37f,   0.0f, 0.0f, 0.0f,     1.0f, 1.0f,
        -1.01f, 0.6f, 0.37f,    0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        -1.01f, 0.75f, 0.67f,   0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        -1.01f, 0.6f, 0.67f,    0.0f, 0.0f, 0.0f,     0.0f, 0.0f,
        -1.01f, 0.6f, 0.37f,    0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        // [Handle 4]
        1.01f, 0.75f, 0.67f,    1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        1.01f, 0.75f, 0.37f,    1.0f, 1.0f, 1.0f,     1.0f, 1.0f,
        1.01f, 0.6f, 0.37f,     1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        1.01f, 0.75f, 0.67f,    1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        1.01f, 0.6f, 0.67f,     1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
        1.01f, 0.6f, 0.37f,     1.0f, 1.0f, 1.0f,     0.0f, 1.0f,

        // [Lampu 1]
        // sisi 1
        0.8f, 0.75f, 3.67f,     1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        0.8f, 0.45f, 3.67f,     1.0f, 1.0f, 1.0f,     1.0f, 1.0f,
        0.8f, 0.45f, 3.8f,      1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        0.8f, 0.75f, 3.67f,     1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        0.8f, 0.75f, 3.8f,      1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
        0.8f, 0.45f, 3.8f,      1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        // sisi 2
        0.8f, 0.75f, 3.67f,     1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        0.8f, 0.45f, 3.67f,     1.0f, 1.0f, 1.0f,     1.0f, 1.0f,
        0.4f, 0.45f, 3.67f,     1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        0.8f, 0.75f, 3.67f,     1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        0.4f, 0.75f, 3.67f,     1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
        0.4f, 0.45f, 3.67f,     1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        // sisi 3
        0.8f, 0.75f, 3.67f,     1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        0.8f, 0.75f, 3.8f,      1.0f, 1.0f, 1.0f,     1.0f, 1.0f,
        0.4f, 0.75f, 3.8f,      1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        0.8f, 0.75f, 3.67f,     1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        0.4f, 0.75f, 3.67f,     1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
        0.4f, 0.75f, 3.8f,      1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        // sisi 4
        0.4f, 0.75f, 3.67f,     1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        0.4f, 0.45f, 3.67f,     1.0f, 1.0f, 1.0f,     1.0f, 1.0f,
        0.4f, 0.45f, 3.8f,      1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        0.4f, 0.75f, 3.67f,     1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        0.4f, 0.75f, 3.8f,      1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
        0.4f, 0.45f, 3.8f,      1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        // sisi 5
        0.8f, 0.75f, 3.8f,      1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        0.8f, 0.45f, 3.8f,      1.0f, 1.0f, 1.0f,     1.0f, 1.0f,
        0.4f, 0.45f, 3.8f,      1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        0.8f, 0.75f, 3.8f,      1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        0.4f, 0.75f, 3.8f,      1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
        0.4f, 0.45f, 3.8f,      1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        // sisi 6
        0.8f, 0.45f, 3.67f,     1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        0.8f, 0.45f, 3.8f,      1.0f, 1.0f, 1.0f,     1.0f, 1.0f,
        0.4f, 0.45f, 3.8f,      1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        0.8f, 0.45f, 3.67f,     1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        0.4f, 0.45f, 3.67f,     1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
        0.4f, 0.45f, 3.8f,      1.0f, 1.0f, 1.0f,     0.0f, 1.0f,

        // [Lampu 2]
        // sisi 1
        -0.8f, 0.75f, 3.67f,    1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        -0.8f, 0.45f, 3.67f,    1.0f, 1.0f, 1.0f,     1.0f, 1.0f,
        -0.8f, 0.45f, 3.8f,     1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        -0.8f, 0.75f, 3.67f,    1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        -0.8f, 0.75f, 3.8f,     1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
        -0.8f, 0.45f, 3.8f,     1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        // sisi 2
        -0.8f, 0.75f, 3.67f,    1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        -0.8f, 0.45f, 3.67f,    1.0f, 1.0f, 1.0f,     1.0f, 1.0f,
        -0.4f, 0.45f, 3.67f,    1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        -0.8f, 0.75f, 3.67f,    1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        -0.4f, 0.75f, 3.67f,    1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
        -0.4f, 0.45f, 3.67f,    1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        // sisi 3
        -0.8f, 0.75f, 3.67f,    1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        -0.8f, 0.75f, 3.8f,     1.0f, 1.0f, 1.0f,     1.0f, 1.0f,
        -0.4f, 0.75f, 3.8f,     1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        -0.8f, 0.75f, 3.67f,    1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        -0.4f, 0.75f, 3.67f,    1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
        -0.4f, 0.75f, 3.8f,     1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        // sisi 4
        -0.4f, 0.75f, 3.67f,    1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        -0.4f, 0.45f, 3.67f,    1.0f, 1.0f, 1.0f,     1.0f, 1.0f,
        -0.4f, 0.45f, 3.8f,     1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        -0.4f, 0.75f, 3.67f,    1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        -0.4f, 0.75f, 3.8f,     1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
        -0.4f, 0.45f, 3.8f,     1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        // sisi 5
        -0.8f, 0.75f, 3.8f,     1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        -0.8f, 0.45f, 3.8f,     1.0f, 1.0f, 1.0f,     1.0f, 1.0f,
        -0.4f, 0.45f, 3.8f,     1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        -0.8f, 0.75f, 3.8f,     1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        -0.4f, 0.75f, 3.8f,     1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
        -0.4f, 0.45f, 3.8f,     1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        // sisi 6
        -0.8f, 0.45f, 3.67f,    1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        -0.8f, 0.45f, 3.8f,     1.0f, 1.0f, 1.0f,     1.0f, 1.0f,
        -0.4f, 0.45f, 3.8f,     1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        -0.8f, 0.45f, 3.67f,    1.0f, 1.0f, 1.0f,     1.0f, 0.0f,
        -0.4f, 0.45f, 3.67f,    1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
        -0.4f, 0.45f, 3.8f,     1.0f, 1.0f, 1.0f,     0.0f, 1.0f,

        // [Lampu 3]
        // sisi 1
        0.8f, 0.75f, -2.33f,    0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        0.8f, 0.45f, -2.33f,    0.0f, 0.0f, 0.0f,     1.0f, 1.0f,
        0.8f, 0.45f, -2.46f,    0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        0.8f, 0.75f, -2.33f,    0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        0.8f, 0.75f, -2.46f,    0.0f, 0.0f, 0.0f,     0.0f, 0.0f,
        0.8f, 0.45f, -2.46f,    0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        // sisi 2
        0.8f, 0.75f, -2.33f,    0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        0.8f, 0.45f, -2.33f,    0.0f, 0.0f, 0.0f,     1.0f, 1.0f,
        0.4f, 0.45f, -2.33f,    0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        0.8f, 0.75f, -2.33f,    0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        0.4f, 0.75f, -2.33f,    0.0f, 0.0f, 0.0f,     0.0f, 0.0f,
        0.4f, 0.45f, -2.33f,    0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        // sisi 3
        0.8f, 0.75f, -2.33f,    0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        0.8f, 0.75f, -2.46f,    0.0f, 0.0f, 0.0f,     1.0f, 1.0f,
        0.4f, 0.75f, -2.46f,    0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        0.8f, 0.75f, -2.33f,    0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        0.4f, 0.75f, -2.33f,    0.0f, 0.0f, 0.0f,     0.0f, 0.0f,
        0.4f, 0.75f, -2.46f,    0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        // sisi 4
        0.4f, 0.75f, -2.33f,    0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        0.4f, 0.45f, -2.33f,    0.0f, 0.0f, 0.0f,     1.0f, 1.0f,
        0.4f, 0.45f, -2.46f,    0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        0.4f, 0.75f, -2.33f,    0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        0.4f, 0.75f, -2.46f,    0.0f, 0.0f, 0.0f,     0.0f, 0.0f,
        0.4f, 0.45f, -2.46f,    0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        // sisi 5
        0.8f, 0.75f, -2.46f,    0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        0.8f, 0.45f, -2.46f,    0.0f, 0.0f, 0.0f,     1.0f, 1.0f,
        0.4f, 0.45f, -2.46f,    0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        0.8f, 0.75f, -2.46f,    0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        0.4f, 0.75f, -2.46f,    0.0f, 0.0f, 0.0f,     0.0f, 0.0f,
        0.4f, 0.45f, -2.46f,    0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        // sisi 6
        0.8f, 0.45f, -2.33f,    0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        0.8f, 0.45f, -2.46f,    0.0f, 0.0f, 0.0f,     1.0f, 1.0f,
        0.4f, 0.45f, -2.46f,    0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        0.8f, 0.45f, -2.33f,    0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        0.4f, 0.45f, -2.33f,    0.0f, 0.0f, 0.0f,     0.0f, 0.0f,
        0.4f, 0.45f, -2.46f,    0.0f, 0.0f, 0.0f,     0.0f, 1.0f,

        // [Lampu 4]
        // sisi 1
        // sisi 1
        -0.8f, 0.75f, -2.33f,   0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        -0.8f, 0.45f, -2.33f,   0.0f, 0.0f, 0.0f,     1.0f, 1.0f,
        -0.8f, 0.45f, -2.46f,   0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        -0.8f, 0.75f, -2.33f,   0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        -0.8f, 0.75f, -2.46f,   0.0f, 0.0f, 0.0f,     0.0f, 0.0f,
        -0.8f, 0.45f, -2.46f,   0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        // sisi 2
        -0.8f, 0.75f, -2.33f,   0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        -0.8f, 0.45f, -2.33f,   0.0f, 0.0f, 0.0f,     1.0f, 1.0f,
        -0.4f, 0.45f, -2.33f,   0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        -0.8f, 0.75f, -2.33f,   0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        -0.4f, 0.75f, -2.33f,   0.0f, 0.0f, 0.0f,     0.0f, 0.0f,
        -0.4f, 0.45f, -2.33f,   0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        // sisi 3
        -0.8f, 0.75f, -2.33f,   0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        -0.8f, 0.75f, -2.46f,   0.0f, 0.0f, 0.0f,     1.0f, 1.0f,
        -0.4f, 0.75f, -2.46f,   0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        -0.8f, 0.75f, -2.33f,   0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        -0.4f, 0.75f, -2.33f,   0.0f, 0.0f, 0.0f,     0.0f, 0.0f,
        -0.4f, 0.75f, -2.46f,   0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        // sisi 4
        -0.4f, 0.75f, -2.33f,   0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        -0.4f, 0.45f, -2.33f,   0.0f, 0.0f, 0.0f,     1.0f, 1.0f,
        -0.4f, 0.45f, -2.46f,   0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        -0.4f, 0.75f, -2.33f,   0.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        -0.4f, 0.75f, -2.46f,   0.0f, 0.0f, 0.0f,     0.0f, 0.0f,
        -0.4f, 0.45f, -2.46f,   0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        // sisi 5
        -0.8f, 0.75f, -2.46f,   0.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        -0.8f, 0.45f, -2.46f,  0.0f, 0.0f, 0.0f,    1.0f, 1.0f,
        -0.4f, 0.45f, -2.46f,  0.0f, 0.0f, 0.0f,    1.0f, 0.0f,
        -0.8f, 0.75f, -2.46f,  0.0f, 0.0f, 0.0f,    1.0f, 0.0f,
        -0.4f, 0.75f, -2.46f,  0.0f, 0.0f, 0.0f,    0.0f, 0.0f,
        -0.4f, 0.45f, -2.46f,  0.0f, 0.0f, 0.0f,    0.0f, 1.0f,
        // sisi 6
        -0.8f, 0.45f, -2.33f,  0.0f, 0.0f, 0.0f,    0.0f, 1.0f,
        -0.8f, 0.45f, -2.46f,  0.0f, 0.0f, 0.0f,    1.0f, 1.0f,
        -0.4f, 0.45f, -2.46f,  0.0f, 0.0f, 0.0f,    1.0f, 0.0f,
        -0.8f, 0.45f, -2.33f,  0.0f, 0.0f, 0.0f,    1.0f, 0.0f,
        -0.4f, 0.45f, -2.33f,  0.0f, 0.0f, 0.0f,    0.0f, 0.0f,
        -0.4f, 0.45f, -2.46f,  0.0f, 0.0f, 0.0f,    0.0f, 1.0f,

        // [Roda Kotak 1]
        // sisi 1
        0.8f, 0.4f, 2.3,    0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.8f, -0.8f, 2.3,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
        0.8f, -0.8f, 3,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.8f, 0.4f, 2.3,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.8f, 0.4f, 3,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.8f, -0.8f, 3,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        // sisi 2
        0.8f, 0.4f, 2.3,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.8f, -0.8f, 2.3,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.3f, -0.8f, 2.3,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.8f, 0.4f, 2.3,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.3f, 0.4f, 2.3,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.3f, -0.8f, 2.3,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        // sisi 3
        0.8f, 0.4f, 2.3,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.8f, 0.4f, 3,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.3f, 0.4f, 3,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.8f, 0.4f, 2.3,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.3f, 0.4f, 2.3,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.3f, 0.4f, 3,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        // sisi 4
        0.3f, 0.4f, 2.3,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.3f, -0.8f, 2.3,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.3f, -0.8f, 3,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.3f, 0.4f, 2.3,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.3f, 0.4f, 3,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.3f, -0.8f, 3,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        // sisi 5
        0.8f, 0.4f, 3,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.8f, -0.8f, 3,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.3f, -0.8f, 3,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.8f, 0.4f, 3,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.3f, -0.8f, 3,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.3f, -0.8f, 3,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        // sisi 6
        0.8f, -0.8f, 2.3,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.8f, -0.8f, 3,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.3f, -0.8f, 3,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.8f, -0.8f, 2.3,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.3f, -0.8f, 2.3,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.3f, -0.8f, 3,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        // [Roda Kotak 2]
        // sisi 1
        0.8f, 0.4f, -1.4,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.8f, -0.8f, -1.4,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.8f, -0.8f, -2.1,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.8f, 0.4f, -1.4,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.8f, 0.4f, -2.1,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.8f, -0.8f, -2.1,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        // sisi 2
        0.8f, 0.4f, -1.4,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.8f, -0.8f, -1.4,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.3f, -0.8f, -1.4,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.8f, 0.4f, -1.4,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.3f, 0.4f, -1.4,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.3f, -0.8f, -1.4,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        // sisi 3
        0.8f, 0.4f, -1.4,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.8f, 0.4f, -2.1,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.3f, 0.4f, -2.1,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.8f, 0.4f, -1.4,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.3f, 0.4f, -1.4,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.3f, 0.4f, -2.1,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        // sisi 4
        0.3f, 0.4f, -1.4,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.3f, -0.8f, -1.4,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.3f, -0.8f, -2.1,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.3f, 0.4f, -1.4,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.3f, 0.4f, -2.1,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.3f, -0.8f, -2.1,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        // sisi 5
        0.8f, 0.4f, -2.1,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.8f, -0.8f, -2.1,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.3f, -0.8f, -2.1,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.8f, 0.4f, -2.1,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.3f, -0.8f, -2.1,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.3f, -0.8f, -2.1,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        // sisi 6
        0.8f, -0.8f, -1.4,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.8f, -0.8f, -2.1,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.3f, -0.8f, -2.1,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.8f, -0.8f, -1.4,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.3f, -0.8f, -1.4,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.3f, -0.8f, -2.1,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        // [Roda Kotak 3]
        // sisi 1
        -0.3, 0.4f, 2.3,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.3, -0.8f, 2.3,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.3, -0.8f, 3,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.3, 0.4f, 2.3,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.3, 0.4f, 3,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.3, -0.8f, 3,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        // sisi 2
        -0.3, 0.4f, 2.3,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.3, -0.8f, 2.3,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.8f, -0.8f, 2.3,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.3, 0.4f, 2.3,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.8f, 0.4f, 2.3,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.8f, -0.8f, 2.3,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        // sisi 3
        -0.3, 0.4f, 2.3,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.3, 0.4f, 3,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.8f, 0.4f, 3,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.3, 0.4f, 2.3,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.8f, 0.4f, 2.3,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.8f, 0.4f, 3,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        // sisi 4
        -0.8f, 0.4f, 2.3,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.8f, -0.8f, 2.3,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.8f, -0.8f, 3,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.8f, 0.4f, 2.3,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.8f, 0.4f, 3,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.8f, -0.8f, 3,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        // // sisi 5
        -0.3, 0.4f, 3,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.3, -0.8f, 3,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.8f, -0.8f, 3,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.3, 0.4f, 3,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.8f, -0.8f, 3,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.8f, -0.8f, 3,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        // // sisi 6
        -0.3, -0.8f, 2.3,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.3, -0.8f, 3,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.8f, -0.8f, 3,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.3, -0.8f, 2.3,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.8f, -0.8f, 2.3,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.8f, -0.8f, 3,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        // [Roda Kotak 4]
        // sisi 1
        -0.3f, 0.4f, -1.4f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.3f, -0.8f, -1.4f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.3f, -0.8f, -2.1f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.3f, 0.4f, -1.4f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.3f, 0.4f, -2.1f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.3f, -0.8f, -2.1f ,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        // sisi 2
        -0.3f, 0.4f, -1.4f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.3f, -0.8f, -1.4f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.8f, -0.8f, -1.4f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.3f, 0.4f, -1.4f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.8f, 0.4f, -1.4f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.8f, -0.8f, -1.4f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        // sisi 3
        -0.3f, 0.4f, -1.4f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.3f, 0.4f, -2.1f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.8f, 0.4f, -2.1f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.3f, 0.4f, -1.4f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.8f, 0.4f, -1.4f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.8f, 0.4f, -2.1f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        // sisi 4
        -0.8f, 0.4f, -1.4f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.8f, -0.8f, -1.4f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.8f, -0.8f, -2.1f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.8f, 0.4f, -1.4f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.8f, 0.4f, -2.1f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.8f, -0.8f, -2.1f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        // sisi 5
        -0.3f, 0.4f, -2.1f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.3f, -0.8f, -2.1f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.8f, -0.8f, -2.1f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.3f, 0.4f, -2.1f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.8f, -0.8f, -2.1f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.8f, -0.8f, -2.1f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        // sisi 6
        -0.3f, -0.8f, -1.4f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.3f, -0.8f, -2.1f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.8f, -0.8f, -2.1f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.3f, -0.8f, -1.4f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.8f, -0.8f, -1.4f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.8f, -0.8f, -2.1f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,    

        // Jalanan 1
        -8.0f, -0.8f, -50.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
        -8.0f, -0.8f, 50.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
        8.0f, -0.8f, -50.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
        8.0f, -0.8f, -50.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
        8.0f, -0.8f, 50.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
        -8.0f, -0.8f, 50.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,    
        // Jalanan 2
        42.0f, -0.8f, -50.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
        42.0f, -0.8f, 50.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
        68.0f, -0.8f, -50.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
        68.0f, -0.8f, -50.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
        68.0f, -0.8f, 50.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
        42.0f, -0.8f, 50.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,    
        // Jalanan 3

        // Jalanan 4

        // Jalanan 5

        // Jalanan 6

        // Jalanan 7

        // Jalanan 8
    };

        // ,  0.0f, 1.0f,
        // ,  1.0f, 1.0f,
        // ,  1.0f, 0.0f,
        // ,  1.0f, 0.0f,
        // ,  0.0f, 0.0f,
        // ,  0.0f, 1.0f,

// The MAIN function, from here we start our application and run our Game loop
int main( )
{
    // Init GLFW
    glfwInit( );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    
    GLFWwindow* window = glfwCreateWindow( WIDTH, HEIGHT, "Car Shader", nullptr, nullptr ); // Windowed
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Set the required callback functions
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    glfwSetScrollCallback( window, ScrollCallback );
    
    // Options, removes the mouse cursor for a more immersive experience
    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // Setup some OpenGL options
    glEnable( GL_DEPTH_TEST );
    
    // enable alpha support
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    
    // Setup and compile our shaders
    Shader lightShader( "res/shaders/light.vs", "res/shaders/light.frag" );
    
    glm::vec3 cubePositions[] =
    {
        glm::vec3( 0.0f, 0.0f, 0.0f ),
        glm::vec3( 2.0f, 5.0f, -15.0f ),
        glm::vec3( -1.5f, -2.2f, -2.5f ),
        glm::vec3( -3.8f, -2.0f, -12.3f ),
        glm::vec3( 2.4f, -0.4f, -3.5f ),
        glm::vec3( -1.7f, 3.0f, -7.5f ),
        glm::vec3( 1.3f, -2.0f, -2.5f ),
        glm::vec3( 1.5f, 2.0f, -2.5f ),
        glm::vec3( 1.5f, 0.2f, -1.5f ),
        glm::vec3( -1.3f, 1.0f, -1.5f )
    };
    
    GLuint diffuseMap[10], specularMap[10], emissionMap[10];


    GLuint VBO, VAO;
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    // Bind our Vertex Array Object first, then bind and set our buffers and pointers.
    glBindVertexArray( VAO );
    
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
    
    // Position attribute
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * )0 );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray( 1 );
    // TexCoord attribute
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * )( 6 * sizeof( GLfloat ) ) );
    glEnableVertexAttribArray( 2 );
    
    glBindVertexArray( 0 ); // Unbind VAO
    
    // Load and create a texture
    int width, height;
    unsigned char *image;
    
    // --== TEXTURE 1 == --
    glGenTextures(1, &diffuseMap[0]);
    glGenTextures(1, &specularMap[0]);
    glGenTextures(1, &emissionMap[0]);

    glBindTexture( GL_TEXTURE_2D, diffuseMap[0] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    image = SOIL_load_image( "res/images/metal.jpg", &width, &height, 0, SOIL_LOAD_RGB );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );

    glBindTexture( GL_TEXTURE_2D, specularMap[0] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    image = SOIL_load_image( "res/images/metal.jpg", &width, &height, 0, SOIL_LOAD_RGB );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );

    glBindTexture( GL_TEXTURE_2D, 0 );


    // --== TEXTURE 2 == --
    glGenTextures(1, &diffuseMap[1]);
    glGenTextures(1, &specularMap[1]);
    glGenTextures(1, &emissionMap[1]);

    glBindTexture( GL_TEXTURE_2D, diffuseMap[1] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    image = SOIL_load_image( "res/images/mirror.jpg", &width, &height, 0, SOIL_LOAD_RGB );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );

    glBindTexture( GL_TEXTURE_2D, specularMap[1] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    image = SOIL_load_image( "res/images/mirror.jpg", &width, &height, 0, SOIL_LOAD_RGB );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );

    glBindTexture( GL_TEXTURE_2D, 1 );


    // --== TEXTURE 3 == --
    glGenTextures(1, &diffuseMap[2]);
    glGenTextures(1, &specularMap[2]);
    glGenTextures(1, &emissionMap[2]);

    glBindTexture( GL_TEXTURE_2D, diffuseMap[2] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    image = SOIL_load_image( "res/images/black.jpeg", &width, &height, 0, SOIL_LOAD_RGB );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );

    glBindTexture( GL_TEXTURE_2D, specularMap[2] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    image = SOIL_load_image( "res/images/black.jpeg", &width, &height, 0, SOIL_LOAD_RGB );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );

    glBindTexture( GL_TEXTURE_2D, 2 );

    // --== TEXTURE 4 == --
    glGenTextures(1, &diffuseMap[3]);
    glGenTextures(1, &specularMap[3]);
    glGenTextures(1, &emissionMap[3]);

    glBindTexture( GL_TEXTURE_2D, diffuseMap[3] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    image = SOIL_load_image( "res/images/sun.jpg", &width, &height, 0, SOIL_LOAD_RGB );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );

    glBindTexture( GL_TEXTURE_2D, specularMap[3] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    image = SOIL_load_image( "res/images/sun.jpg", &width, &height, 0, SOIL_LOAD_RGB );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );

    glBindTexture( GL_TEXTURE_2D, 3 );

    
    // --== TEXTURE 5 == --
    glGenTextures(1, &diffuseMap[4]);
    glGenTextures(1, &specularMap[4]);
    glGenTextures(1, &emissionMap[4]);

    glBindTexture( GL_TEXTURE_2D, diffuseMap[4] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    image = SOIL_load_image( "res/images/wheel.jpg", &width, &height, 0, SOIL_LOAD_RGB );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );

    glBindTexture( GL_TEXTURE_2D, specularMap[4] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    image = SOIL_load_image( "res/images/wheel.jpg", &width, &height, 0, SOIL_LOAD_RGB );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );

    glBindTexture( GL_TEXTURE_2D, 4 );

    // --== TEXTURE 6 == --
    glGenTextures(1, &diffuseMap[5]);
    glGenTextures(1, &specularMap[5]);
    glGenTextures(1, &emissionMap[5]);

    glBindTexture( GL_TEXTURE_2D, diffuseMap[5] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    image = SOIL_load_image( "res/images/road.jpg", &width, &height, 0, SOIL_LOAD_RGB );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );

    glBindTexture( GL_TEXTURE_2D, specularMap[5] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    image = SOIL_load_image( "res/images/road.jpg", &width, &height, 0, SOIL_LOAD_RGB );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );

    glBindTexture( GL_TEXTURE_2D, 5 );
    
    lightShader.Use();
    glUniform1i(glGetUniformLocation(lightShader.Program, "material.diffuse"),  0);
    glUniform1i(glGetUniformLocation(lightShader.Program, "material.specular"), 1);

    // Game loop
    while( !glfwWindowShouldClose( window ) )
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime( );
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Check and call events
        glfwPollEvents( );
        DoMovement( );
        
        // Clear the colorbuffer
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
        // Draw
        lightShader.Use( );
        GLint lightDirLoc = glGetUniformLocation(lightShader.Program, "light.direction");
        GLint viewPosLoc  = glGetUniformLocation(lightShader.Program, "viewPos");
        glUniform3f(lightDirLoc, -0.2f, -1.0f, -0.3f);
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        // Set lights properties
        glUniform3f(glGetUniformLocation(lightShader.Program, "light.ambient"),  0.2f, 0.2f, 0.2f);
        glUniform3f(glGetUniformLocation(lightShader.Program, "light.diffuse"),  0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);
        // Set material properties
        glUniform1f(glGetUniformLocation(lightShader.Program, "material.shininess"), 100.0f);



        
        
        glm::mat4 projection;
        projection = glm::perspective(camera.GetZoom( ), (GLfloat)SCREEN_WIDTH/(GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);
        
        // Create camera transformation
        glm::mat4 view;
        view = camera.GetViewMatrix( );

        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation( lightShader.Program, "model" );
        GLint viewLoc = glGetUniformLocation( lightShader.Program, "view" );
        GLint projLoc = glGetUniformLocation( lightShader.Program, "projection" );
        
        // Pass the matrices to the shader
        glUniformMatrix4fv( viewLoc, 1, GL_FALSE, glm::value_ptr( view ) );
        glUniformMatrix4fv( projLoc, 1, GL_FALSE, glm::value_ptr( projection ) );
        
        glBindVertexArray( VAO );
        
        //for( GLuint i = 0; i < 10; i++ )
        //{
            // Calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model;
            model = glm::translate( model, cubePositions[0] );
            GLfloat angle = 20.0f * 0;
            model = glm::rotate(model, angle, glm::vec3( 1.0f, 0.3f, 0.5f ) );
            glUniformMatrix4fv( modelLoc, 1, GL_FALSE, glm::value_ptr( model ) );
            
            // Bind Textures using texture units
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuseMap[0]);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specularMap[0]);
            glBindTexture( GL_TEXTURE_2D, VAO );
            glUniform1i( glGetUniformLocation( lightShader.Program, "metal" ), 0 );
            glDrawArrays( GL_TRIANGLES, 0, 36 * 2 );
            
            // Bind Textures using texture units
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuseMap[1]);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specularMap[1]);
            glBindTexture( GL_TEXTURE_2D, VAO );
            glUniform1i( glGetUniformLocation( lightShader.Program, "mirror" ), 1 );
            glDrawArrays( GL_TRIANGLES, 36 * 2, 36 );

            // Bind Textures using texture units
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuseMap[2]);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specularMap[2]);
            glBindTexture( GL_TEXTURE_2D, VAO );
            glUniform1i( glGetUniformLocation( lightShader.Program, "handle and door" ), 2 );
            glDrawArrays( GL_TRIANGLES, 36 * 3, 36 );

            // Bind Textures using texture units
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuseMap[3]);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specularMap[3]);
            glBindTexture( GL_TEXTURE_2D, VAO );
            glUniform1i( glGetUniformLocation( lightShader.Program, "lamp" ), 3 );
            glDrawArrays( GL_TRIANGLES, 36 * 4, 36 * 4 );

            // Bind Textures using texture units
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuseMap[4]);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specularMap[4]);
            glBindTexture( GL_TEXTURE_2D, VAO );
            glUniform1i( glGetUniformLocation( lightShader.Program, "wheel" ), 4 );
            glDrawArrays( GL_TRIANGLES, 36 * 8, 36 * 4 );

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuseMap[5]);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specularMap[5]);
            glBindTexture( GL_TEXTURE_2D, VAO );
            glUniform1i( glGetUniformLocation( lightShader.Program, "road" ), 5 );
            glDrawArrays( GL_TRIANGLES, 36 * 12, 36 * 4 );
        //}
        
        glBindVertexArray( 0 );
        
        // Swap the buffers
        glfwSwapBuffers( window );
    }
    
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &VBO );
    glfwTerminate( );
    
    return EXIT_SUCCESS;
}

// Moves/alters the camera positions based on user input
void DoMovement( )
{
    // Camera controls
    if(keys[GLFW_KEY_UP] )
    {
        middlePoint[0+2] -= 0.1f * cos(rad);
        middlePoint[0] -= 0.1f * sin(rad);
        for (int i=0; i<carLastIndex; i+=8) {
            vertices[i+2] -= 0.1f * cos(rad);
            vertices[i] -= 0.1f * sin(rad);
        }
        glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
    }
    
    if(keys[GLFW_KEY_DOWN] )
    {
        middlePoint[0+2] += 0.1f * cos(rad);
        middlePoint[0] += 0.1f * sin(rad);
        for (int i=0; i<carLastIndex; i+=8) {
            vertices[i+2] += 0.1f * cos(rad);
            vertices[i] += 0.1f * sin(rad);
        }
        glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
    }
    
    if(keys[GLFW_KEY_LEFT] )
    {
        rad += 0.1f;
        for (int i=0; i<carLastIndex; i+=8) {
            vertices[i+2] = (vertices[i+2]-middlePoint[2])*cos(0.1f) - (vertices[i]-middlePoint[0])*sin(0.1f) + middlePoint[2];
            vertices[i] = (vertices[i+2]-middlePoint[2])*sin(0.1f) + (vertices[i]-middlePoint[0])*cos(0.1f) + middlePoint[0];
        }
        glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
    }
    
    if(keys[GLFW_KEY_RIGHT] )
    {
        rad -= 0.1f;
        for (int i=0; i<carLastIndex; i+=8) {
            vertices[i+2] = (vertices[i+2]-middlePoint[2])*cos(-0.1f) - (vertices[i]-middlePoint[0])*sin(-0.1f) + middlePoint[2];
            vertices[i] = (vertices[i+2]-middlePoint[2])*sin(-0.1f) + (vertices[i]-middlePoint[0])*cos(-0.1f) + middlePoint[0];
        }
        glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
    }

    // Car controls
    if( keys[GLFW_KEY_W])
    {
        
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if( keys[GLFW_KEY_S])
    {
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if( keys[GLFW_KEY_A])
    {
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if( keys[GLFW_KEY_D])
    {
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( key >= 0 && key < 1024 )
    {
        if( action == GLFW_PRESS )
        {
            keys[key] = true;
        }
        else if( action == GLFW_RELEASE )
        {
            keys[key] = false;
        }
    }
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}	


void ScrollCallback( GLFWwindow *window, double xOffset, double yOffset )
{
    camera.ProcessMouseScroll( yOffset );
}
