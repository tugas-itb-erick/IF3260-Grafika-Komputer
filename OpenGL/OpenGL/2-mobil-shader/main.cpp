#include "stdafx.h"

#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other includes
#include "shader.h"

#define PI 3.14159
#define CAR_SIZE 3 * 2 * 12 + 3 * 2 * 360 * 360

const GLuint WIDTH = 800, HEIGHT = 600;

int initWheelIdx = 3 * 2 * 12;
int startWheelIdx = 3 * 2 * 12;
int wheelIdx = 3 * 2 * 12;
float degree = 0;

GLfloat car[CAR_SIZE] =
{
	// Positions			// Colors
	-0.5f, -0.2f, 0.0f,		0.0f, 0.0f, 1.0f,
	-0.5f, 0.2f, 0.0f,		0.0f, 0.0f, 1.0f,
	-0.25f, 0.2f, 0.0f,		0.0f, 0.0f, 1.0f,

	-0.5f, -0.2f, 0.0f,		0.0f, 0.0f, 1.0f,
	-0.25f, -0.2f, 0.0f,	0.0f, 0.0f, 1.0f,
	-0.25f, 0.2f, 0.0f,		0.0f, 0.0f, 1.0f,

	-0.25f, 0.2f, 0.0f,		0.0f, 0.0f, 1.0f,
	-0.1f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f,
	0.3f, 0.2f, 0.0f,		0.0f, 0.0f, 1.0f,

	-0.1f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f,
	0.15f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f,
	0.3f, 0.2f, 0.0f,		0.0f, 0.0f, 1.0f,

	0.3f, 0.2f, 0.0f,		0.0f, 0.0f, 1.0f,
	0.5f, 0.2f, 0.0f,		0.0f, 0.0f, 1.0f,
	0.5f, -0.2f, 0.0f,		0.0f, 0.0f, 1.0f,

	0.3f, 0.2f, 0.0f,		0.0f, 0.0f, 1.0f,
	0.3f, -0.2f, 0.0f,		0.0f, 0.0f, 1.0f,
	0.5f, -0.2f, 0.0f,		0.0f, 0.0f, 1.0f,

	-0.25f, -0.2f, 0.0f,	0.0f, 0.0f, 1.0f,
	-0.25f, 0.2f, 0.0f,		0.0f, 0.0f, 1.0f,
	0.3f, 0.2f, 0.0f,		0.0f, 0.0f, 1.0f,

	-0.25f, -0.2f, 0.0f,	0.0f, 0.0f, 1.0f,
	0.3f, -0.2f, 0.0f,		0.0f, 0.0f, 1.0f,
	0.3f, 0.2f, 0.0f,		0.0f, 0.0f, 1.0f,

	-0.20f, 0.2f, 0.0f,		0.0f, 1.0f, 1.0f,
	-0.08f, 0.45f, 0.0f,	0.0f, 1.0f, 1.0f,
	0.0f, 0.2f, 0.0f,		0.0f, 1.0f, 1.0f,

	0.0f, 0.45f, 0.0f,		0.0f, 1.0f, 1.0f,
	-0.08f, 0.45f, 0.0f,	0.0f, 1.0f, 1.0f,
	0.0f, 0.2f, 0.0f,		0.0f, 1.0f, 1.0f,

	0.25f, 0.2f, 0.0f,		0.0f, 1.0f, 1.0f,
	0.13f, 0.45f, 0.0f,		0.0f, 1.0f, 1.0f,
	0.05f, 0.2f, 0.0f,		0.0f, 1.0f, 1.0f,

	0.05f, 0.45f, 0.0f,		0.0f, 1.0f, 1.0f,
	0.13f, 0.45f, 0.0f,		0.0f, 1.0f, 1.0f,
	0.05f, 0.2f, 0.0f,		0.0f, 1.0f, 1.0f,
};

void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Car - Shader", nullptr, nullptr);

	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, screenWidth, screenHeight);

	// Build and compile our shader program
	Shader shader("core.vs", "core.frag");


	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(car), car, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	// Game loop
	while (!glfwWindowShouldClose(window)) {
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		drawCircle(-0.2f, -0.2f, 0.0f, 0.14f, 36 * 3);
		drawCircle(0.2f, -0.2f, 0.0f, 0.14f, 36 * 3);
		wheelIdx = initWheelIdx;
		startWheelIdx = initWheelIdx;
		degree += 0.01f;
		if (degree > 360)
			degree = 0;
		
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw our first triangle
		shader.Use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 3 * 0, CAR_SIZE);
		glBindVertexArray(0);


		// Swap the screen buffers
		glfwSwapBuffers(window);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(car), car, GL_STATIC_DRAW);
	}

	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();

	return 0;
}

void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides)
{
	float deg = 360 / numberOfSides;
	float initdeg = deg;
	for (int i = startWheelIdx; i < numberOfSides + startWheelIdx; i++) {
		car[wheelIdx + i * 18 + 0] = x;
		car[wheelIdx + i * 18 + 1] = y;
		car[wheelIdx + i * 18 + 2] = z;
		car[wheelIdx + i * 18 + 3] = 1.0f;
		car[wheelIdx + i * 18 + 4] = 0.3f;
		car[wheelIdx + i * 18 + 5] = 0.3f;
		car[wheelIdx + i * 18 + 6] = x + (radius * cos((deg + degree) * PI / 180));
		car[wheelIdx + i * 18 + 7] = y + (radius * sin((deg + degree) * PI / 180));
		car[wheelIdx + i * 18 + 8] = z;
		car[wheelIdx + i * 18 + 9] = 0.9f;
		car[wheelIdx + i * 18 + 10] = 0.2f;
		car[wheelIdx + i * 18 + 11] = 0.2f;
		car[wheelIdx + i * 18 + 12] = x + (radius * cos((deg + degree) + initdeg * PI / 180));
		car[wheelIdx + i * 18 + 13] = y + (radius * sin((deg + degree) + initdeg * PI / 180));
		car[wheelIdx + i * 18 + 14] = z;
		car[wheelIdx + i * 18 + 15] = 0.8f;
		car[wheelIdx + i * 18 + 16] = 0.1f;
		car[wheelIdx + i * 18 + 17] = 0.1f;
		wheelIdx += 18;
		deg += initdeg;
	}
	startWheelIdx += wheelIdx;
}