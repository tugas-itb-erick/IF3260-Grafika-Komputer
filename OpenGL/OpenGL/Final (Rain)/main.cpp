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


#define MAX_PARTICLES 100
#define WCX     640
#define WCY     480
#define RAIN    0
#define SNOW    1
#define HAIL    2


float slowdown = 2.0;
float velocity = 5.0;
float zoom = 2.5;

int loop;
int fall = RAIN;
bool starter = true;

//floor colors
float r = 0.0;
float g = 1.0;
float b = 0.0;
float ground_points[21][21][3];
float ground_colors[21][21][4];
float accum = -10.0f;

typedef struct {
    // Life
    bool alive; // is the particle alive?
    float life; // particle lifespan
    float fade; // decay
    // color
    float red; 
    float green;
    float blue;
    // Position/direction
    float xpos; 
    float ypos; 
    float zpos;
    // Velocity/Direction, only goes down in y dir
    float vel;
    // Gravity
    float gravity;
}particles;

// Paticle System
particles par_sys[MAX_PARTICLES]; 

// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void ScrollCallback( GLFWwindow *window, double xOffset, double yOffset );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );
void initParticles(int i, bool starter);
void init(bool starter);
void drawRain(bool starter);

// Camera
Camera  camera(glm::vec3( 0.0f, 2.0f, 8.0f ) );
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLfloat vertices[6000] =
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
        -0.3, 0.4f, -1.4,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.3, -0.8f, -1.4,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.3f, -0.8f, -2.1,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.3, 0.4f, -1.4,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.3f, 0.4f, -2.1,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.3f, -0.8f, -2.1 ,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        // sisi 2
        -0.3f, 0.4f, -1.4,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.3f, -0.8f, -1.4,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.8f, -0.8f, -1.4,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.3f, 0.4f, -1.4,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.8f, 0.4f, -1.4,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.8f, -0.8f, -1.4,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        // sisi 3
        -0.3f, 0.4f, -1.4,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.3f, 0.4f, -2.1,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.8f, 0.4f, -2.1,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.3f, 0.4f, -1.4,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.8f, 0.4f, -1.4,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.8f, 0.4f, -2.1,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        // sisi 4
        -0.8f, 0.4f, -1.4,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.8f, -0.8f, -1.4,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.8f, -0.8f, -2.1,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.8f, 0.4f, -1.4,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.8f, 0.4f, -2.1,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.8f, -0.8f, -2.1,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        // sisi 5
        -0.3f, 0.4f, -2.1,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.3f, -0.8f, -2.1,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.8f, -0.8f, -2.1,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.3f, 0.4f, -2.1,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.8f, -0.8f, -2.1,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.8f, -0.8f, -2.1,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        // sisi 6
        -0.3f, -0.8f, -1.4,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.3f, -0.8f, -2.1,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.8f, -0.8f, -2.1,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.3f, -0.8f, -1.4,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.8f, -0.8f, -1.4,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.8f, -0.8f, -2.1,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    };

int idx_rain = 3456;

GLfloat inc_x_rain[MAX_PARTICLES] = { 0.0f };
GLfloat inc_y_rain[MAX_PARTICLES] = { 0.0f };
GLfloat inc_z_rain[MAX_PARTICLES] = { 0.0f };

GLfloat init_x_rain[MAX_PARTICLES] = { 0.0f };
GLfloat init_z_rain[MAX_PARTICLES] = { 0.0f };

// The MAIN function, from here we start our application and run our Game loop
int main()
{
    // Init GLFW
    glfwInit();
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    
    GLFWwindow* window = glfwCreateWindow( WIDTH, HEIGHT, "Final Car with Particle", nullptr, nullptr ); // Windowed
    
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

    // Initialize particles
    init(starter);
    drawRain(starter);
    starter = false;

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
    
    GLuint diffuseMap[6], specularMap[6], emissionMap[6];


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
    image = SOIL_load_image( "res/images/metal.jpg", &width, &height, 0, SOIL_LOAD_RGB );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );

    glBindTexture( GL_TEXTURE_2D, specularMap[5] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    image = SOIL_load_image( "res/images/metal.jpg", &width, &height, 0, SOIL_LOAD_RGB );
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
        glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
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
            glUniform1i( glGetUniformLocation( lightShader.Program, "lamp" ), 4 );
            glDrawArrays( GL_TRIANGLES, 36 * 8, 36 * 4 );

        //}
        int start = 36*12;

        float x1, y1, z1;

        lightShader.Use();

        for (loop = 0; loop < MAX_PARTICLES; loop++) {
        if (par_sys[loop].alive == true) {          
             x1 = par_sys[loop].xpos;
             y1 = par_sys[loop].ypos;
             z1 = par_sys[loop].zpos + zoom;

             // Bind Textures using texture units
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuseMap[5]);
            glBindVertexArray(VAO);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specularMap[5]);
            glBindTexture( GL_TEXTURE_2D, VAO );
            glUniform3f(glGetUniformLocation(lightShader.Program, "inc_rain"), inc_x_rain[loop], inc_y_rain[loop], inc_z_rain[loop]);
            glUniform1i(glGetUniformLocation(lightShader.Program, "is_rain"), 1);
            glUniform1i( glGetUniformLocation( lightShader.Program, "metal" ), 5 );
            glDrawArrays( GL_LINES, start, start + 2);
            start += 2;
            
            // Update values
            //Move 
            // Adjust slowdown for speed!
            par_sys[loop].ypos += par_sys[loop].vel / (slowdown*1000);
            inc_y_rain[loop] += par_sys[loop].vel / (slowdown*1000);
            par_sys[loop].vel += par_sys[loop].gravity;
            // Decay
            par_sys[loop].life -= par_sys[loop].fade;
            
            if (par_sys[loop].ypos <= accum) {
                par_sys[loop].life = -1.0;
            }
            //Revive 
            if (par_sys[loop].life < 0.0) {
                initParticles(loop, starter);
            }
        }
    }

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
    if( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
    {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
    {
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
    {
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
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

void initParticles(int idx, bool starter) {
    par_sys[idx].alive = true;
    par_sys[idx].life = 1.0;
    par_sys[idx].fade = float(rand()%100)/1000.0f+0.003f;
                   
    par_sys[idx].xpos = (float) (rand() % 21) - 10;
    par_sys[idx].ypos = 8.0f;
    par_sys[idx].zpos = (float) (rand() % 21) - 10;

    if (!starter) {
        inc_x_rain[idx] =  par_sys[idx].xpos - init_x_rain[idx];
        inc_z_rain[idx] = par_sys[idx].zpos - init_z_rain[idx];
        inc_y_rain[idx] = 0.0f;
    } else {
        init_x_rain[loop] = par_sys[idx].xpos;
        init_z_rain[loop] = par_sys[idx].zpos;
    }
    
    par_sys[idx].red = 0.5;
    par_sys[idx].green = 0.5;
    par_sys[idx].blue = 1.0;
                    
    par_sys[idx].vel = velocity;
    par_sys[idx].gravity = -0.8;//-0.8;
}

void init(bool starter) {
    for (loop = 0; loop < MAX_PARTICLES; loop++) {
        initParticles(loop, starter);
    }
}

void drawRain(bool starter) {
    float x, y, z;

    for (loop = 0; loop < MAX_PARTICLES; loop++) {
        if (par_sys[loop].alive == true) {          
             x = par_sys[loop].xpos;
             y = par_sys[loop].ypos;
             z = par_sys[loop].zpos + zoom;

             if (starter) {
                init_x_rain[loop] = x;
                init_z_rain[loop] = z;
             }

            vertices[idx_rain++] = x;
            vertices[idx_rain++] = y;
            vertices[idx_rain++] = z;

            vertices[idx_rain++] = 1.0f;
            vertices[idx_rain++] = 1.0f;
            vertices[idx_rain++] = 1.0f;

            vertices[idx_rain++] = 1.0f;
            vertices[idx_rain++] = 0.0f;

            vertices[idx_rain++] = x;
            vertices[idx_rain++] = y + 0.5f;
            vertices[idx_rain++] = z;

            vertices[idx_rain++] = 1.0f;
            vertices[idx_rain++] = 1.0f;
            vertices[idx_rain++] = 1.0f;

            vertices[idx_rain++] = 0.0f;
            vertices[idx_rain++] = 0.0f;
            
            // Update values
            //Move
            // Adjust slowdown for speed!
            par_sys[loop].ypos += par_sys[loop].vel / (slowdown*1000);
            inc_y_rain[loop] += par_sys[loop].vel / (slowdown*1000);
            par_sys[loop].vel += par_sys[loop].gravity;
            // Decay
            par_sys[loop].life -= par_sys[loop].fade;
            
            if (par_sys[loop].ypos <= accum) {
                par_sys[loop].life = -1.0;
            }
            //Revive 
            if (par_sys[loop].life < 0.0) {
                initParticles(loop, starter);
            }
        }
    }
}