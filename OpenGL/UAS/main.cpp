// Std. Includes
#include <string>
#include <algorithm>
#include <vector>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Texture.hpp"
#include "shader.hpp"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

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

struct Particle{
    glm::vec3 pos, speed;
    unsigned char r,g,b,a; // Color
    float size, angle, weight;
    float life; // Remaining life of the particle. if <0 : dead and unused.
    float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

    bool operator<(const Particle& that) const {
        // Sort in reverse order : far particles drawn first.
        return this->cameradistance > that.cameradistance;
    }
};

const int MaxParticles = 100000;
Particle ParticlesContainer[MaxParticles];
int LastUsedParticle = 0;

// Finds a Particle in ParticlesContainer which isn't used yet.
// (i.e. life < 0);
int FindUnusedParticle(){

    for(int i=LastUsedParticle; i<MaxParticles; i++){
        if (ParticlesContainer[i].life < 0){
            LastUsedParticle = i;
            return i;
        }
    }

    for(int i=0; i<LastUsedParticle; i++){
        if (ParticlesContainer[i].life < 0){
            LastUsedParticle = i;
            return i;
        }
    }

    return 0; // All particles are taken, override the first one
}

void SortParticles(){
    std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
}

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

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "Particle.vertexshader", "Particle.fragmentshader" );

    // Vertex shader
    GLuint CameraRight_worldspace_ID  = glGetUniformLocation(programID, "CameraRight_worldspace");
    GLuint CameraUp_worldspace_ID  = glGetUniformLocation(programID, "CameraUp_worldspace");
    GLuint ViewProjMatrixID = glGetUniformLocation(programID, "VP");

    // fragment shader
    GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

    static GLfloat* g_particule_position_size_data = new GLfloat[MaxParticles * 4];
    static GLubyte* g_particule_color_data         = new GLubyte[MaxParticles * 4];

    for(int i=0; i<MaxParticles; i++){
        ParticlesContainer[i].life = -1.0f;
        ParticlesContainer[i].cameradistance = -1.0f;
    }

    GLuint Texture = loadDDS("smoke.jpg");

    // The VBO containing the 4 vertices of the particles.
    // Thanks to instancing, they will be shared by all particles.
    static const GLfloat g_vertex_buffer_data[] = { 
         -0.5f, -0.5f, 0.0f,
          0.5f, -0.5f, 0.0f,
         -0.5f,  0.5f, 0.0f,
          0.5f,  0.5f, 0.0f,
    };
    GLuint billboard_vertex_buffer;
    glGenBuffers(1, &billboard_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // The VBO containing the positions and sizes of the particles
    GLuint particles_position_buffer;
    glGenBuffers(1, &particles_position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    // Initialize with empty (NULL) buffer : it will be updated later, each frame.
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

    // The VBO containing the colors of the particles
    GLuint particles_color_buffer;
    glGenBuffers(1, &particles_color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    // Initialize with empty (NULL) buffer : it will be updated later, each frame.
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

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
        
        // We will need the camera's position in order to sort the particles
        // w.r.t the camera's distance.
        // There should be a getCameraPosition() function in common/controls.cpp, 
        // but this works too.
        glm::vec3 CameraPosition(glm::inverse(view)[3]);

        glm::mat4 ViewProjectionMatrix = projection * view;

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
        
        
        
        // Generate 10 new particule each millisecond,
        // but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
        // newparticles will be huge and the next frame even longer.
        int newparticles = (int)(deltaTime*10000.0);
        if (newparticles > (int)(0.016f*10000.0))
            newparticles = (int)(0.016f*10000.0);
        
        for(int i=0; i<newparticles; i++){
            int particleIndex = FindUnusedParticle();
            ParticlesContainer[particleIndex].life = 5.0f; // This particle will live 5 seconds.
            ParticlesContainer[particleIndex].pos = glm::vec3(0,0,-20.0f);

            float spread = 1.5f;
            glm::vec3 maindir = glm::vec3(0.0f, 10.0f, 0.0f);
            // Very bad way to generate a random direction; 
            // See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
            // combined with some user-controlled parameters (main direction, spread, etc)
            glm::vec3 randomdir = glm::vec3(
                (rand()%2000 - 1000.0f)/1000.0f,
                (rand()%2000 - 1000.0f)/1000.0f,
                (rand()%2000 - 1000.0f)/1000.0f
            );
            
            ParticlesContainer[particleIndex].speed = maindir + randomdir*spread;


            // Very bad way to generate a random color
            ParticlesContainer[particleIndex].r = rand() % 256;
            ParticlesContainer[particleIndex].g = rand() % 256;
            ParticlesContainer[particleIndex].b = rand() % 256;
            ParticlesContainer[particleIndex].a = (rand() % 256) / 3;

            ParticlesContainer[particleIndex].size = (rand()%1000)/2000.0f + 0.1f;
        }

        // Simulate all particles
        int ParticlesCount = 0;
        for(int i=0; i<MaxParticles; i++){

            Particle& p = ParticlesContainer[i]; // shortcut

            if(p.life > 0.0f){

                // Decrease life
                p.life -= deltaTime;
                if (p.life > 0.0f){

                    // Simulate simple physics : gravity only, no collisions
                    p.speed += glm::vec3(0.0f,-9.81f, 0.0f) * (float)deltaTime * 0.5f;
                    p.pos += p.speed * (float)deltaTime;
                    p.cameradistance = glm::length2( p.pos - CameraPosition );
                    //ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

                    // Fill the GPU buffer
                    g_particule_position_size_data[4*ParticlesCount+0] = p.pos.x;
                    g_particule_position_size_data[4*ParticlesCount+1] = p.pos.y;
                    g_particule_position_size_data[4*ParticlesCount+2] = p.pos.z;
                                                   
                    g_particule_position_size_data[4*ParticlesCount+3] = p.size;
                                                   
                    g_particule_color_data[4*ParticlesCount+0] = p.r;
                    g_particule_color_data[4*ParticlesCount+1] = p.g;
                    g_particule_color_data[4*ParticlesCount+2] = p.b;
                    g_particule_color_data[4*ParticlesCount+3] = p.a;

                }else{
                    // Particles that just died will be put at the end of the buffer in SortParticles();
                    p.cameradistance = -1.0f;
                }

                ParticlesCount++;

            }
        }

        SortParticles();

        // Update the buffers that OpenGL uses for rendering.
        // There are much more sophisticated means to stream data from the CPU to the GPU, 
        // but this is outside the scope of this tutorial.
        // http://www.opengl.org/wiki/Buffer_Object_Streaming


        glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
        glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
        glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

        glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
        glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
        glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particule_color_data);


        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Use our shader
        glUseProgram(programID);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniform1i(TextureID, 0);

        // Same as the billboards tutorial
        glUniform3f(CameraRight_worldspace_ID, view[0][0], view[1][0], view[2][0]);
        glUniform3f(CameraUp_worldspace_ID   , view[0][1], view[1][1], view[2][1]);

        glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
        glVertexAttribPointer(
            0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );
        
        // 2nd attribute buffer : positions of particles' centers
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
        glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            4,                                // size : x + y + z + size => 4
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );

        // 3rd attribute buffer : particles' colors
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
        glVertexAttribPointer(
            2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            4,                                // size : r + g + b + a => 4
            GL_UNSIGNED_BYTE,                 // type
            GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
            0,                                // stride
            (void*)0                          // array buffer offset
        );

        // These functions are specific to glDrawArrays*Instanced*.
        // The first parameter is the attribute buffer we're talking about.
        // The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
        // http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
        glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
        glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
        glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1

        // Draw the particules !
        // This draws many times a small triangle_strip (which looks like a quad).
        // This is equivalent to :
        // for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4), 
        // but faster.
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        glBindVertexArray( 0 );
        
        // Swap the buffers
        glfwSwapBuffers( window );
        glfwPollEvents();
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
