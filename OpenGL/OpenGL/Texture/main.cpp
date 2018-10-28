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

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void ScrollCallback( GLFWwindow *window, double xOffset, double yOffset );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );

// Camera
Camera  camera(glm::vec3( 0.0f, 0.0f, 3.0f ) );
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

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
    Shader ourShader( "res/shaders/core.vs", "res/shaders/core.frag" );
    
    // Set up our vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] =
    {
        // [Badan mobil bawah]
        // sisi 1
        1.0f, -0.33f, -2.33f,  0.0f, 0.0f,
        1.0f,  1.0f, -2.33f,  1.0f, 0.0f,
        1.0f,  1.0f, 3.67f,  1.0f, 1.0f,
        1.0f, -0.33f, -2.33f,  1.0f, 1.0f,
        1.0f, -0.33f, 3.67f,  0.0f, 1.0f,
        1.0f,  1.0f,  3.67f,  0.0f, 0.0f,
        // sisi 2
        -1.0f, -0.33f, 3.67f,  0.0f, 0.0f,
        -1.0f, 1.0f, 3.67f,  1.0f, 0.0f,
        -1.0f, 1.0f, -2.33f,  1.0f, 1.0f,
        -1.0f, -0.33f, 3.67f,  1.0f, 1.0f,
        -1.0f, -0.33f, -2.33f,  0.0f, 1.0f,
        -1.0f, 1.0f, -2.33f,  0.0f, 0.0f,
        // sisi 3
        -1.0f, -0.33f, -2.33f,  0.0f, 0.0f,
        -1.0f, 1.0f, -2.33f,  1.0f, 0.0f,
        1.0f, 1.0f, -2.33f,  1.0f, 1.0f,
        -1.0f, -0.33f, -2.33f,  1.0f, 1.0f,
        1.0f, -0.33f, -2.33f,  0.0f, 1.0f,
        1.0f, 1.0f, -2.33f,  0.0f, 0.0f,
        // sisi 4
        1.0f, -0.33f, 3.67f,  0.0f, 0.0f,
        1.0f, 1.0f, 3.67f,  1.0f, 0.0f,
        -1.0f, 1.0f, 3.67f,  1.0f, 1.0f,
        1.0f, -0.33f, 3.67f,  1.0f, 1.0f,
        -1.0f, -0.33f, 3.67f,  0.0f, 1.0f,
        -1.0f, 1.0f, 3.67f,  0.0f, 0.0f,
        // sisi 5
        1.0f, -0.33f, 3.67f,  0.0f, 1.0f,
        -1.0f, -0.33f, 3.67f,  1.0f, 1.0f,
        -1.0f, -0.33f, -2.33f,  1.0f, 0.0f,
        1.0f, -0.33f, 3.67f,  1.0f, 0.0f,
        1.0f, -0.33f, -2.33f,  0.0f, 0.0f,
        -1.0f, -0.33f, -2.33f,  0.0f, 1.0f,
        // sisi 6
        1.0f, 1.0f, 3.67f,  0.0f, 1.0f,
        -1.0f, 1.0f, 3.67f,  1.0f, 1.0f,
        -1.0f, 1.0f, -2.33f,  1.0f, 0.0f,
        1.0f, 1.0f, 3.67f,  1.0f, 0.0f,
        1.0f, 1.0f, -2.33f,  0.0f, 0.0f,
        -1.0f, 1.0f, -2.33f,  0.0f, 1.0f,

        // [Badan mobil atas]
        // sisi 1
        -1.0f, 1.0f, -1.33f,  0.0f, 1.0f,
        -1.0f, 2.33f, 0.0f,  1.0f, 1.0f,
        1.0f, 2.33f, 0.0f,  1.0f, 0.0f,
        -1.0f, 1.0f, -1.33f,  1.0f, 0.0f,
        1.0f, 1.0f, -1.33f,  0.0f, 0.0f,
        1.0f, 2.33f, 0.0f,  0.0f, 1.0f,
        // sisi 2
        -1.0f, 1.0f, 3.0f,  0.0f, 1.0f,
        -1.0f, 2.33f, 2.33f,  1.0f, 1.0f,
        -1.0f, 2.33f, 0.0f,  1.0f, 0.0f,
        -1.0f, 1.0f, 3.0f,  1.0f, 0.0f,
        -1.0f, 1.0f, -1.33f,  0.0f, 0.0f,
        -1.0f, 2.33f, 0.0f,  0.0f, 1.0f,
        // sisi 3
        1.0f, 1.0f, 3.0f,  0.0f, 1.0f,
        1.0f, 2.33f, 2.33f,  1.0f, 1.0f,
        -1.0f, 2.33f, 2.33f,  1.0f, 0.0f,
        1.0f, 1.0f, 3.0f,  1.0f, 0.0f,
        -1.0f, 1.0f, 3.0f,  0.0f, 0.0f,
        -1.0f, 2.33f, 2.33f,  0.0f, 1.0f,
        // sisi 4
        1.0f, 1.0f, -1.33f,  0.0f, 1.0f,
        1.0f, 2.33f, 0.0f,  1.0f, 1.0f,
        1.0f, 2.33f, 2.33f,  1.0f, 0.0f,
        1.0f, 1.0f, -1.33f,  1.0f, 0.0f,
        1.0f, 1.0f, 3.0f,  0.0f, 0.0f,
        1.0f, 2.33f, 2.33f,  0.0f, 1.0f,
        // sisi 5
        -1.0f, 2.33f, 0.0f,  0.0f, 1.0f,
        1.0f, 2.33f, 0.0f,  1.0f, 1.0f,
        1.0f, 2.33f, 2.33f,  1.0f, 0.0f,
        -1.0f, 2.33f, 0.0f,  1.0f, 0.0f,
        -1.0f, 2.33f, 2.33f,  0.0f, 0.0f,
        1.0f, 2.33f, 2.33f,  0.0f, 1.0f,
        // sisi 6
        1.0f, 1.0f, 3.67f,  0.0f, 1.0f,
        -1.0f, 1.0f, 3.67f,  1.0f, 1.0f,
        -1.0f, 1.0f, -2.33f,  1.0f, 0.0f,
        1.0f, 1.0f, 3.67f,  1.0f, 0.0f,
        1.0f, 1.0f, -2.33f,  0.0f, 0.0f,
        -1.0f, 1.0f, -2.33f,  0.0f, 1.0f,

        // [Jendela Belakang]
        0.67f, 1.33f, 2.85f,  0.0f, 1.0f,
        0.67f, 2.0f, 2.52f,  1.0f, 1.0f,
        -0.67f, 2.0f, 2.52f,  1.0f, 0.0f,
        0.67f, 1.33f, 2.85f,  1.0f, 0.0f,
        -0.67f, 1.33f, 2.85f,  0.0f, 0.0f,
        -0.67f, 2.0f, 2.52f,  0.0f, 1.0f,
        // [Jendela Sisi Belakang]
        1.01f, 1.0f, 1.33f,  0.0f, 1.0f,
        1.01f, 2.0f, 1.33f,  1.0f, 1.0f,
        1.01f, 2.0f, 2.0f,  1.0f, 0.0f,
        1.01f, 1.0f, 1.33f,  1.0f, 0.0f,
        1.01f, 1.0f, 2.67f,  0.0f, 0.0f,
        1.01f, 2.0f, 2.0f,  0.0f, 1.0f,
        // [Jendela Sisi Depan]
        1.01f, 1.0f, -1.0f,  0.0f, 1.0f,
        1.01f, 2.0f, 0.0f,  1.0f, 1.0f,
        1.01f, 2.0f, 0.67f,  1.0f, 0.0f,
        1.01f, 1.0f, -1.0f,  1.0f, 0.0f,
        1.01f, 1.0f, 0.67f,  0.0f, 0.0f,
        1.01f, 2.0f, 0.67f,  0.0f, 1.0f,
        // [Jendela Depan]
        -0.67f, 1.33f, -1.01f,  0.0f, 1.0f,
        -0.67f, 2.0f, -0.34f,  1.0f, 1.0f,
        0.67f, 2.0f, -0.34f,  1.0f, 0.0f,
        -0.67f, 1.33f, -1.01f,  1.0f, 0.0f,
        0.67f, 1.33f, -1.01f,  0.0f, 0.0f,
        0.67f, 2.0f, -0.34f,  0.0f, 1.0f,
        // [Jendela Sisi Depan 2]
        -1.01f, 1.0f, 0.67f,  0.0f, 1.0f,
        -1.01f, 2.0f, 0.67f,  1.0f, 1.0f,
        -1.01f, 2.0f, 0.0f,  1.0f, 0.0f,
        -1.01f, 1.0f, 0.67f,  1.0f, 0.0f,
        -1.01f, 1.0f, -1.0f,  0.0f, 0.0f,
        -1.01f, 2.0f, 0.0f,  0.0f, 1.0f,
        // [Jendela Sisi Belakang 2]
        -1.01f, 1.0f, 2.67f,  0.0f, 1.0f,
        -1.01f, 2.0f, 2.0f,  1.0f, 1.0f,
        -1.01f, 2.0f, 1.33f,  1.0f, 0.0f,
        -1.01f, 1.0f, 2.67f,  1.0f, 0.0f,
        -1.01f, 1.0f, 1.33f,  0.0f, 0.0f,
        -1.01f, 2.0f, 1.33f,  0.0f, 1.0f,

        // [Pintu 1]
        -1.01f, 2.33f, 1.01f,  0.0f, 1.0f,
        -1.01f, 2.33f, 0.99f,  1.0f, 1.0f,
        -1.01f, -0.33f, 0.99f,  1.0f, 0.0f,
        -1.01f, 2.33f, 1.01f,  1.0f, 0.0f,
        -1.01f, -0.33f, 1.01f,  0.0f, 0.0f,
        -1.01f, -0.33f, 0.99f,  0.0f, 1.0f,
        // [Pintu 2]
        1.01f, 2.33f, 1.01f,  0.0f, 1.0f,
        1.01f, 2.33f, 0.99f,  1.0f, 1.0f,
        1.01f, -0.33f, 0.99f,  1.0f, 0.0f,
        1.01f, 2.33f, 1.01f,  1.0f, 0.0f,
        1.01f, -0.33f, 1.01f,  0.0f, 0.0f,
        1.01f, -0.33f, 0.99f,  0.0f, 1.0f,
        // [Handle 1]
        -1.01f, 0.75f, 1.63f,  0.0f, 1.0f,
        -1.01f, 0.75f, 1.33f,  1.0f, 1.0f,
        -1.01f, 0.6f, 1.33f,  1.0f, 0.0f,
        -1.01f, 0.75f, 1.63f,  1.0f, 0.0f,
        -1.01f, 0.6f, 1.63f,  0.0f, 0.0f,
        -1.01f, 0.6f, 1.33f,  0.0f, 1.0f,
        // [Handle 2]
        1.01f, 0.75f, 1.63f,  0.0f, 1.0f,
        1.01f, 0.75f, 1.33f,  1.0f, 1.0f,
        1.01f, 0.6f, 1.33f,  1.0f, 0.0f,
        1.01f, 0.75f, 1.63f,  1.0f, 0.0f,
        1.01f, 0.6f, 1.63f,  0.0f, 0.0f,
        1.01f, 0.6f, 1.33f,  0.0f, 1.0f,
        // [Handle 3]
        -1.01f, 0.75f, 0.67f,  0.0f, 1.0f,
        -1.01f, 0.75f, 0.37f,  1.0f, 1.0f,
        -1.01f, 0.6f, 0.37f,  1.0f, 0.0f,
        -1.01f, 0.75f, 0.67f,  1.0f, 0.0f,
        -1.01f, 0.6f, 0.67f,  0.0f, 0.0f,
        -1.01f, 0.6f, 0.37f,  0.0f, 1.0f,
        // [Handle 4]
        1.01f, 0.75f, 0.67f,  0.0f, 1.0f,
        1.01f, 0.75f, 0.37f,  1.0f, 1.0f,
        1.01f, 0.6f, 0.37f,  1.0f, 0.0f,
        1.01f, 0.75f, 0.67f,  1.0f, 0.0f,
        1.01f, 0.6f, 0.67f,  0.0f, 0.0f,
        1.01f, 0.6f, 0.37f,  0.0f, 1.0f,

        // [Lampu 1]
        // sisi 1
        0.8f, 0.75f, 3.67f,  0.0f, 1.0f,
        0.8f, 0.45f, 3.67f,  1.0f, 1.0f,
        0.8f, 0.45f, 3.8f,  1.0f, 0.0f,
        0.8f, 0.75f, 3.67f,  1.0f, 0.0f,
        0.8f, 0.75f, 3.8f,  0.0f, 0.0f,
        0.8f, 0.45f, 3.8f,  0.0f, 1.0f,
        // sisi 2
        0.8f, 0.75f, 3.67f,  0.0f, 1.0f,
        0.8f, 0.45f, 3.67f,  1.0f, 1.0f,
        0.4f, 0.45f, 3.67f,  1.0f, 0.0f,
        0.8f, 0.75f, 3.67f,  1.0f, 0.0f,
        0.4f, 0.75f, 3.67f,  0.0f, 0.0f,
        0.4f, 0.45f, 3.67f,  0.0f, 1.0f,
        // sisi 3
        0.8f, 0.75f, 3.67f,  0.0f, 1.0f,
        0.8f, 0.75f, 3.8f,  1.0f, 1.0f,
        0.4f, 0.75f, 3.8f,  1.0f, 0.0f,
        0.8f, 0.75f, 3.67f,  1.0f, 0.0f,
        0.4f, 0.75f, 3.67f,  0.0f, 0.0f,
        0.4f, 0.75f, 3.8f,  0.0f, 1.0f,
        // sisi 4
        0.4f, 0.75f, 3.67f,  0.0f, 1.0f,
        0.4f, 0.45f, 3.67f,  1.0f, 1.0f,
        0.4f, 0.45f, 3.8f,  1.0f, 0.0f,
        0.4f, 0.75f, 3.67f,  1.0f, 0.0f,
        0.4f, 0.75f, 3.8f,  0.0f, 0.0f,
        0.4f, 0.45f, 3.8f,  0.0f, 1.0f,
        // sisi 5
        0.8f, 0.75f, 3.8f,  0.0f, 1.0f,
        0.8f, 0.45f, 3.8f,  1.0f, 1.0f,
        0.4f, 0.45f, 3.8f,  1.0f, 0.0f,
        0.8f, 0.75f, 3.8f,  1.0f, 0.0f,
        0.4f, 0.75f, 3.8f,  0.0f, 0.0f,
        0.4f, 0.45f, 3.8f,  0.0f, 1.0f,
        // sisi 6
        0.8f, 0.45f, 3.67f,  0.0f, 1.0f,
        0.8f, 0.45f, 3.8f,  1.0f, 1.0f,
        0.4f, 0.45f, 3.8f,  1.0f, 0.0f,
        0.8f, 0.45f, 3.67f,  1.0f, 0.0f,
        0.4f, 0.45f, 3.67f,  0.0f, 0.0f,
        0.4f, 0.45f, 3.8f,  0.0f, 1.0f,

        // [Lampu 2]
        // sisi 1
        -0.8f, 0.75f, 3.67f,  0.0f, 1.0f,
        -0.8f, 0.45f, 3.67f,  1.0f, 1.0f,
        -0.8f, 0.45f, 3.8f,  1.0f, 0.0f,
        -0.8f, 0.75f, 3.67f,  1.0f, 0.0f,
        -0.8f, 0.75f, 3.8f,  0.0f, 0.0f,
        -0.8f, 0.45f, 3.8f,  0.0f, 1.0f,
        // sisi 2
        -0.8f, 0.75f, 3.67f,  0.0f, 1.0f,
        -0.8f, 0.45f, 3.67f,  1.0f, 1.0f,
        -0.4f, 0.45f, 3.67f,  1.0f, 0.0f,
        -0.8f, 0.75f, 3.67f,  1.0f, 0.0f,
        -0.4f, 0.75f, 3.67f,  0.0f, 0.0f,
        -0.4f, 0.45f, 3.67f,  0.0f, 1.0f,
        // sisi 3
        -0.8f, 0.75f, 3.67f,  0.0f, 1.0f,
        -0.8f, 0.75f, 3.8f,  1.0f, 1.0f,
        -0.4f, 0.75f, 3.8f,  1.0f, 0.0f,
        -0.8f, 0.75f, 3.67f,  1.0f, 0.0f,
        -0.4f, 0.75f, 3.67f,  0.0f, 0.0f,
        -0.4f, 0.75f, 3.8f,  0.0f, 1.0f,
        // sisi 4
        -0.4f, 0.75f, 3.67f,  0.0f, 1.0f,
        -0.4f, 0.45f, 3.67f,  1.0f, 1.0f,
        -0.4f, 0.45f, 3.8f,  1.0f, 0.0f,
        -0.4f, 0.75f, 3.67f,  1.0f, 0.0f,
        -0.4f, 0.75f, 3.8f,  0.0f, 0.0f,
        -0.4f, 0.45f, 3.8f,  0.0f, 1.0f,
        // sisi 5
        -0.8f, 0.75f, 3.8f,  0.0f, 1.0f,
        -0.8f, 0.45f, 3.8f,  1.0f, 1.0f,
        -0.4f, 0.45f, 3.8f,  1.0f, 0.0f,
        -0.8f, 0.75f, 3.8f,  1.0f, 0.0f,
        -0.4f, 0.75f, 3.8f,  0.0f, 0.0f,
        -0.4f, 0.45f, 3.8f,  0.0f, 1.0f,
        // sisi 6
        -0.8f, 0.45f, 3.67f,  0.0f, 1.0f,
        -0.8f, 0.45f, 3.8f,  1.0f, 1.0f,
        -0.4f, 0.45f, 3.8f,  1.0f, 0.0f,
        -0.8f, 0.45f, 3.67f,  1.0f, 0.0f,
        -0.4f, 0.45f, 3.67f,  0.0f, 0.0f,
        -0.4f, 0.45f, 3.8f,  0.0f, 1.0f,

        // [Lampu 3]
        // sisi 1
        0.8f, 0.75f, -2.33f,  0.0f, 1.0f,
        0.8f, 0.45f, -2.33f,  1.0f, 1.0f,
        0.8f, 0.45f, -2.46f,  1.0f, 0.0f,
        0.8f, 0.75f, -2.33f,  1.0f, 0.0f,
        0.8f, 0.75f, -2.46f,  0.0f, 0.0f,
        0.8f, 0.45f, -2.46f,  0.0f, 1.0f,
        // sisi 2
        0.8f, 0.75f, -2.33f,  0.0f, 1.0f,
        0.8f, 0.45f, -2.33f,  1.0f, 1.0f,
        0.4f, 0.45f, -2.33f,  1.0f, 0.0f,
        0.8f, 0.75f, -2.33f,  1.0f, 0.0f,
        0.4f, 0.75f, -2.33f,  0.0f, 0.0f,
        0.4f, 0.45f, -2.33f,  0.0f, 1.0f,
        // sisi 3
        0.8f, 0.75f, -2.33f,  0.0f, 1.0f,
        0.8f, 0.75f, -2.46f,  1.0f, 1.0f,
        0.4f, 0.75f, -2.46f,  1.0f, 0.0f,
        0.8f, 0.75f, -2.33f,  1.0f, 0.0f,
        0.4f, 0.75f, -2.33f,  0.0f, 0.0f,
        0.4f, 0.75f, -2.46f,  0.0f, 1.0f,
        // sisi 4
        0.4f, 0.75f, -2.33f,  0.0f, 1.0f,
        0.4f, 0.45f, -2.33f,  1.0f, 1.0f,
        0.4f, 0.45f, -2.46f,  1.0f, 0.0f,
        0.4f, 0.75f, -2.33f,  1.0f, 0.0f,
        0.4f, 0.75f, -2.46f,  0.0f, 0.0f,
        0.4f, 0.45f, -2.46f,  0.0f, 1.0f,
        // sisi 5
        0.8f, 0.75f, -2.46f,  0.0f, 1.0f,
        0.8f, 0.45f, -2.46f,  1.0f, 1.0f,
        0.4f, 0.45f, -2.46f,  1.0f, 0.0f,
        0.8f, 0.75f, -2.46f,  1.0f, 0.0f,
        0.4f, 0.75f, -2.46f,  0.0f, 0.0f,
        0.4f, 0.45f, -2.46f,  0.0f, 1.0f,
        // sisi 6
        0.8f, 0.45f, -2.33f,  0.0f, 1.0f,
        0.8f, 0.45f, -2.46f,  1.0f, 1.0f,
        0.4f, 0.45f, -2.46f,  1.0f, 0.0f,
        0.8f, 0.45f, -2.33f,  1.0f, 0.0f,
        0.4f, 0.45f, -2.33f,  0.0f, 0.0f,
        0.4f, 0.45f, -2.46f,  0.0f, 1.0f,

        // [Lampu 4]
        // sisi 1
        // sisi 1
        -0.8f, 0.75f, -2.33f,  0.0f, 1.0f,
        -0.8f, 0.45f, -2.33f,  1.0f, 1.0f,
        -0.8f, 0.45f, -2.46f,  1.0f, 0.0f,
        -0.8f, 0.75f, -2.33f,  1.0f, 0.0f,
        -0.8f, 0.75f, -2.46f,  0.0f, 0.0f,
        -0.8f, 0.45f, -2.46f,  0.0f, 1.0f,
        // sisi 2
        -0.8f, 0.75f, -2.33f,  0.0f, 1.0f,
        -0.8f, 0.45f, -2.33f,  1.0f, 1.0f,
        -0.4f, 0.45f, -2.33f,  1.0f, 0.0f,
        -0.8f, 0.75f, -2.33f,  1.0f, 0.0f,
        -0.4f, 0.75f, -2.33f,  0.0f, 0.0f,
        -0.4f, 0.45f, -2.33f,  0.0f, 1.0f,
        // sisi 3
        -0.8f, 0.75f, -2.33f,  0.0f, 1.0f,
        -0.8f, 0.75f, -2.46f,  1.0f, 1.0f,
        -0.4f, 0.75f, -2.46f,  1.0f, 0.0f,
        -0.8f, 0.75f, -2.33f,  1.0f, 0.0f,
        -0.4f, 0.75f, -2.33f,  0.0f, 0.0f,
        -0.4f, 0.75f, -2.46f,  0.0f, 1.0f,
        // sisi 4
        -0.4f, 0.75f, -2.33f,  0.0f, 1.0f,
        -0.4f, 0.45f, -2.33f,  1.0f, 1.0f,
        -0.4f, 0.45f, -2.46f,  1.0f, 0.0f,
        -0.4f, 0.75f, -2.33f,  1.0f, 0.0f,
        -0.4f, 0.75f, -2.46f,  0.0f, 0.0f,
        -0.4f, 0.45f, -2.46f,  0.0f, 1.0f,
        // sisi 5
        -0.8f, 0.75f, -2.46f,  0.0f, 1.0f,
        -0.8f, 0.45f, -2.46f,  1.0f, 1.0f,
        -0.4f, 0.45f, -2.46f,  1.0f, 0.0f,
        -0.8f, 0.75f, -2.46f,  1.0f, 0.0f,
        -0.4f, 0.75f, -2.46f,  0.0f, 0.0f,
        -0.4f, 0.45f, -2.46f,  0.0f, 1.0f,
        // sisi 6
        -0.8f, 0.45f, -2.33f,  0.0f, 1.0f,
        -0.8f, 0.45f, -2.46f,  1.0f, 1.0f,
        -0.4f, 0.45f, -2.46f,  1.0f, 0.0f,
        -0.8f, 0.45f, -2.33f,  1.0f, 0.0f,
        -0.4f, 0.45f, -2.33f,  0.0f, 0.0f,
        -0.4f, 0.45f, -2.46f,  0.0f, 1.0f,

        // [Roda Kotak 1]
        // sisi 1
        0.8f, 0.4f, 2.3,  0.0f, 1.0f,
        0.8f, -0.8f, 2.3,  1.0f, 1.0f,
        0.8f, -0.8f, 3,  1.0f, 0.0f,
        0.8f, 0.4f, 2.3,  1.0f, 0.0f,
        0.8f, 0.4f, 3,  0.0f, 0.0f,
        0.8f, -0.8f, 3,  0.0f, 1.0f,
        // sisi 2
        0.8f, 0.4f, 2.3,  0.0f, 1.0f,
        0.8f, -0.8f, 2.3,  1.0f, 1.0f,
        0.3f, -0.8f, 2.3,  1.0f, 0.0f,
        0.8f, 0.4f, 2.3,  1.0f, 0.0f,
        0.3f, 0.4f, 2.3,  0.0f, 0.0f,
        0.3f, -0.8f, 2.3,  0.0f, 1.0f,
        // sisi 3
        0.8f, 0.4f, 2.3,  0.0f, 1.0f,
        0.8f, 0.4f, 3,  1.0f, 1.0f,
        0.3f, 0.4f, 3,  1.0f, 0.0f,
        0.8f, 0.4f, 2.3,  1.0f, 0.0f,
        0.3f, 0.4f, 2.3,  0.0f, 0.0f,
        0.3f, 0.4f, 3,  0.0f, 1.0f,
        // sisi 4
        0.3f, 0.4f, 2.3,  0.0f, 1.0f,
        0.3f, -0.8f, 2.3,  1.0f, 1.0f,
        0.3f, -0.8f, 3,  1.0f, 0.0f,
        0.3f, 0.4f, 2.3,  1.0f, 0.0f,
        0.3f, 0.4f, 3,  0.0f, 0.0f,
        0.3f, -0.8f, 3,  0.0f, 1.0f,
        // sisi 5
        0.8f, 0.4f, 3,  0.0f, 1.0f,
        0.8f, -0.8f, 3,  1.0f, 1.0f,
        0.3f, -0.8f, 3,  1.0f, 0.0f,
        0.8f, 0.4f, 3,  1.0f, 0.0f,
        0.3f, -0.8f, 3,  0.0f, 0.0f,
        0.3f, -0.8f, 3,  0.0f, 1.0f,
        // sisi 6
        0.8f, -0.8f, 2.3,  0.0f, 1.0f,
        0.8f, -0.8f, 3,  1.0f, 1.0f,
        0.3f, -0.8f, 3,  1.0f, 0.0f,
        0.8f, -0.8f, 2.3,  1.0f, 0.0f,
        0.3f, -0.8f, 2.3,  0.0f, 0.0f,
        0.3f, -0.8f, 3,  0.0f, 1.0f,

        // [Roda Kotak 2]
        // sisi 1
        0.8f, 0.4f, -1.4,  0.0f, 1.0f,
        0.8f, -0.8f, -1.4,  1.0f, 1.0f,
        0.8f, -0.8f, -2.1,  1.0f, 0.0f,
        0.8f, 0.4f, -1.4,  1.0f, 0.0f,
        0.8f, 0.4f, -2.1,  0.0f, 0.0f,
        0.8f, -0.8f, -2.1,  0.0f, 1.0f,
        // sisi 2
        0.8f, 0.4f, -1.4,  0.0f, 1.0f,
        0.8f, -0.8f, -1.4,  1.0f, 1.0f,
        0.3f, -0.8f, -1.4,  1.0f, 0.0f,
        0.8f, 0.4f, -1.4,  1.0f, 0.0f,
        0.3f, 0.4f, -1.4,  0.0f, 0.0f,
        0.3f, -0.8f, -1.4,  0.0f, 1.0f,
        // sisi 3
        0.8f, 0.4f, -1.4,  0.0f, 1.0f,
        0.8f, 0.4f, -2.1,  1.0f, 1.0f,
        0.3f, 0.4f, -2.1,  1.0f, 0.0f,
        0.8f, 0.4f, -1.4,  1.0f, 0.0f,
        0.3f, 0.4f, -1.4,  0.0f, 0.0f,
        0.3f, 0.4f, -2.1,  0.0f, 1.0f,
        // sisi 4
        0.3f, 0.4f, -1.4,  0.0f, 1.0f,
        0.3f, -0.8f, -1.4,  1.0f, 1.0f,
        0.3f, -0.8f, -2.1,  1.0f, 0.0f,
        0.3f, 0.4f, -1.4,  1.0f, 0.0f,
        0.3f, 0.4f, -2.1,  0.0f, 0.0f,
        0.3f, -0.8f, -2.1,  0.0f, 1.0f,
        // sisi 5
        0.8f, 0.4f, -2.1,  0.0f, 1.0f,
        0.8f, -0.8f, -2.1,  1.0f, 1.0f,
        0.3f, -0.8f, -2.1,  1.0f, 0.0f,
        0.8f, 0.4f, -2.1,  1.0f, 0.0f,
        0.3f, -0.8f, -2.1,  0.0f, 0.0f,
        0.3f, -0.8f, -2.1,  0.0f, 1.0f,
        // sisi 6
        0.8f, -0.8f, -1.4,  0.0f, 1.0f,
        0.8f, -0.8f, -2.1,  1.0f, 1.0f,
        0.3f, -0.8f, -2.1,  1.0f, 0.0f,
        0.8f, -0.8f, -1.4,  1.0f, 0.0f,
        0.3f, -0.8f, -1.4,  0.0f, 0.0f,
        0.3f, -0.8f, -2.1,  0.0f, 1.0f,

        // [Roda Kotak 3]
        // sisi 1
        -0.3, 0.4f, 2.3,  0.0f, 1.0f,
        -0.3, -0.8f, 2.3,  1.0f, 1.0f,
        -0.3, -0.8f, 3,  1.0f, 0.0f,
        -0.3, 0.4f, 2.3,  1.0f, 0.0f,
        -0.3, 0.4f, 3,  0.0f, 0.0f,
        -0.3, -0.8f, 3,  0.0f, 1.0f,
        // sisi 2
        -0.3, 0.4f, 2.3,  0.0f, 1.0f,
        -0.3, -0.8f, 2.3,  1.0f, 1.0f,
        -0.8f, -0.8f, 2.3,  1.0f, 0.0f,
        -0.3, 0.4f, 2.3,  1.0f, 0.0f,
        -0.8f, 0.4f, 2.3,  0.0f, 0.0f,
        -0.8f, -0.8f, 2.3,  0.0f, 1.0f,
        // sisi 3
        -0.3, 0.4f, 2.3,  0.0f, 1.0f,
        -0.3, 0.4f, 3,  1.0f, 1.0f,
        -0.8f, 0.4f, 3,  1.0f, 0.0f,
        -0.3, 0.4f, 2.3,  1.0f, 0.0f,
        -0.8f, 0.4f, 2.3,  0.0f, 0.0f,
        -0.8f, 0.4f, 3,  0.0f, 1.0f,
        // sisi 4
        -0.8f, 0.4f, 2.3,  0.0f, 1.0f,
        -0.8f, -0.8f, 2.3,  1.0f, 1.0f,
        -0.8f, -0.8f, 3,  1.0f, 0.0f,
        -0.8f, 0.4f, 2.3,  1.0f, 0.0f,
        -0.8f, 0.4f, 3,  0.0f, 0.0f,
        -0.8f, -0.8f, 3,  0.0f, 1.0f,
        // // sisi 5
        -0.3, 0.4f, 3,  0.0f, 1.0f,
        -0.3, -0.8f, 3,  1.0f, 1.0f,
        -0.8f, -0.8f, 3,  1.0f, 0.0f,
        -0.3, 0.4f, 3,  1.0f, 0.0f,
        -0.8f, -0.8f, 3,  0.0f, 0.0f,
        -0.8f, -0.8f, 3,  0.0f, 1.0f,
        // // sisi 6
        -0.3, -0.8f, 2.3,  0.0f, 1.0f,
        -0.3, -0.8f, 3,  1.0f, 1.0f,
        -0.8f, -0.8f, 3,  1.0f, 0.0f,
        -0.3, -0.8f, 2.3,  1.0f, 0.0f,
        -0.8f, -0.8f, 2.3,  0.0f, 0.0f,
        -0.8f, -0.8f, 3,  0.0f, 1.0f,

        // [Roda Kotak 4]
        // sisi 1
        -0.3, 0.4f, -1.4,  0.0f, 1.0f,
        -0.3, -0.8f, -1.4,  1.0f, 1.0f,
        -0.3f, -0.8f, -2.1,  1.0f, 0.0f,
        -0.3, 0.4f, -1.4,  1.0f, 0.0f,
        -0.3f, 0.4f, -2.1,  0.0f, 0.0f,
        -0.3f, -0.8f, -2.1 ,  0.0f, 1.0f,
        // sisi 2
        -0.3f, 0.4f, -1.4,  0.0f, 1.0f,
        -0.3f, -0.8f, -1.4,  1.0f, 1.0f,
        -0.8f, -0.8f, -1.4,  1.0f, 0.0f,
        -0.3f, 0.4f, -1.4,  1.0f, 0.0f,
        -0.8f, 0.4f, -1.4,  0.0f, 0.0f,
        -0.8f, -0.8f, -1.4,  0.0f, 1.0f,
        // sisi 3
        -0.3f, 0.4f, -1.4,  0.0f, 1.0f,
        -0.3f, 0.4f, -2.1,  1.0f, 1.0f,
        -0.8f, 0.4f, -2.1,  1.0f, 0.0f,
        -0.3f, 0.4f, -1.4,  1.0f, 0.0f,
        -0.8f, 0.4f, -1.4,  0.0f, 0.0f,
        -0.8f, 0.4f, -2.1,  0.0f, 1.0f,
        // sisi 4
        -0.8f, 0.4f, -1.4,  0.0f, 1.0f,
        -0.8f, -0.8f, -1.4,  1.0f, 1.0f,
        -0.8f, -0.8f, -2.1,  1.0f, 0.0f,
        -0.8f, 0.4f, -1.4,  1.0f, 0.0f,
        -0.8f, 0.4f, -2.1,  0.0f, 0.0f,
        -0.8f, -0.8f, -2.1,  0.0f, 1.0f,
        // sisi 5
        -0.3f, 0.4f, -2.1,  0.0f, 1.0f,
        -0.3f, -0.8f, -2.1,  1.0f, 1.0f,
        -0.8f, -0.8f, -2.1,  1.0f, 0.0f,
        -0.3f, 0.4f, -2.1,  1.0f, 0.0f,
        -0.8f, -0.8f, -2.1,  0.0f, 0.0f,
        -0.8f, -0.8f, -2.1,  0.0f, 1.0f,
        // sisi 6
        -0.3f, -0.8f, -1.4,  0.0f, 1.0f,
        -0.3f, -0.8f, -2.1,  1.0f, 1.0f,
        -0.8f, -0.8f, -2.1,  1.0f, 0.0f,
        -0.3f, -0.8f, -1.4,  1.0f, 0.0f,
        -0.8f, -0.8f, -1.4,  0.0f, 0.0f,
        -0.8f, -0.8f, -2.1,  0.0f, 1.0f,    
    };

        // ,  0.0f, 1.0f,
        // ,  1.0f, 1.0f,
        // ,  1.0f, 0.0f,
        // ,  1.0f, 0.0f,
        // ,  0.0f, 0.0f,
        // ,  0.0f, 1.0f,
    
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
    
    GLuint VBO, VAO;
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    // Bind our Vertex Array Object first, then bind and set our buffers and pointers.
    glBindVertexArray( VAO );
    
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
    
    // Position attribute
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( GLfloat ), ( GLvoid * )0 );
    glEnableVertexAttribArray( 0 );
    // TexCoord attribute
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( GLfloat ), ( GLvoid * )( 3 * sizeof( GLfloat ) ) );
    glEnableVertexAttribArray( 2 );
    
    glBindVertexArray( 0 ); // Unbind VAO
    
    // Load and create a texture
    GLuint texture[5];
    glGenTextures( 5, texture );
    int width, height;
    unsigned char *image;
   
    // --== TEXTURE == --
    glBindTexture( GL_TEXTURE_2D, texture[0] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    // Load, create texture and generate mipmaps
    image = SOIL_load_image( "res/images/metal.jpg", &width, &height, 0, SOIL_LOAD_RGB );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );
    glBindTexture( GL_TEXTURE_2D, 0 );

    // --== TEXTURE == --
    glBindTexture( GL_TEXTURE_2D, texture[1] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    // Load, create texture and generate mipmaps
    image = SOIL_load_image( "res/images/mirror.jpg", &width, &height, 0, SOIL_LOAD_RGB );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );
    glBindTexture( GL_TEXTURE_2D, 1 );

    // --== TEXTURE == --
    glBindTexture( GL_TEXTURE_2D, texture[2] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    // Load, create texture and generate mipmaps
    image = SOIL_load_image( "res/images/black.jpeg", &width, &height, 0, SOIL_LOAD_RGB );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );
    glBindTexture( GL_TEXTURE_2D, 2 );

    // --== TEXTURE == --
    glBindTexture( GL_TEXTURE_2D, texture[3] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    // Load, create texture and generate mipmaps
    image = SOIL_load_image( "res/images/sun.jpg", &width, &height, 0, SOIL_LOAD_RGB );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );
    glBindTexture( GL_TEXTURE_2D, 3 );

    glBindTexture( GL_TEXTURE_2D, texture[4] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    // Load, create texture and generate mipmaps
    image = SOIL_load_image( "res/images/wheel.jpg", &width, &height, 0, SOIL_LOAD_RGB );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );
    glBindTexture( GL_TEXTURE_2D, 4 );
    
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
        
        // Draw our first triangle
        ourShader.Use( );
        
        
        glm::mat4 projection;
        projection = glm::perspective(camera.GetZoom( ), (GLfloat)SCREEN_WIDTH/(GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);
        
        // Create camera transformation
        glm::mat4 view;
        view = camera.GetViewMatrix( );

        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation( ourShader.Program, "model" );
        GLint viewLoc = glGetUniformLocation( ourShader.Program, "view" );
        GLint projLoc = glGetUniformLocation( ourShader.Program, "projection" );
        
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
            glBindTexture( GL_TEXTURE_2D, texture[0] );
            glUniform1i( glGetUniformLocation( ourShader.Program, "metal" ), 0 );
            glDrawArrays( GL_TRIANGLES, 0, 36 * 2 );
            
            // Bind Textures using texture units
            glBindTexture( GL_TEXTURE_2D, texture[1] );
            glUniform1i( glGetUniformLocation( ourShader.Program, "mirror" ), 1 );
            glDrawArrays( GL_TRIANGLES, 36 * 2, 36 );

            // Bind Textures using texture units
            glBindTexture( GL_TEXTURE_2D, texture[2] );
            glUniform1i( glGetUniformLocation( ourShader.Program, "handle and door" ), 2 );
            glDrawArrays( GL_TRIANGLES, 36 * 3, 36 );

            // Bind Textures using texture units
            glBindTexture( GL_TEXTURE_2D, texture[3] );
            glUniform1i( glGetUniformLocation( ourShader.Program, "lamp" ), 3 );
            glDrawArrays( GL_TRIANGLES, 36 * 4, 36 * 4 );

            // Bind Textures using texture units
            glBindTexture( GL_TEXTURE_2D, texture[4] );
            glUniform1i( glGetUniformLocation( ourShader.Program, "lamp" ), 4 );
            glDrawArrays( GL_TRIANGLES, 36 * 8, 36 * 4 );
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
