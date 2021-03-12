#include <stdio.h>	// Include the "standard input output" library - this will let us handle input and output values to return errors to the user
#include <string.h>
#include <cmath>
#include <vector>

// Include the stuff we linked in Section 1.3 tutorial and beyond.
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

// Custom classes
#include "cMesh.h"
#include "cShader.h"
#include "cWindow.h"
#include "cCamera.h"



const float toRadians = 3.14159265f / 180.0f;	// If we multiply a number by this value, it will output a radian value

Window mainWindow;

std::vector<Mesh*> meshList;

std::vector<Shader> shaderList;

Camera camera;

GLfloat deltaTime = 0.0f,
lastTime = 0.0f;

// Vertex shader - to be moved to a separate file. Here we are taking in vertices to be modified or used as is, then passed to fragment shader.
static const char* vShader = "Shaders/shader.vert";

// Fragment shader - also to be moved to a separate file. Note: you don't normally pass anything in to the fragment shader, it simply picks up output from the vertex shader.
// Additional note - with the fragment shader, you don't even have to specify an out variable. If you only have one variable, it is assumed to be the colour, which is defaulted as an output.
static const char* fShader = "Shaders/shader.frag";



void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,	// Left face
		1, 3, 2,	// Righ face
		2, 3, 0,	// Front face
		0, 1, 2		// Bottom face
	};

	// Setup vertices of triangle - no depth, just three vertices within normalised space. Remember - the left side and bottom of our window are represented by -1.0 (or "100% left (or down)), or 1.0 for top or right of our window.
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,		// Bottom left 
		0.0f, -1.0f, 1.0f,		// Background
		1.0f, -1.0f, 0.0f,		// Bottom right
		0.0f, 1.0f, 0.0f		// Top
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj2);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.2f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;

	// Parameters of glm::perspective:
	//	1 - The angle for our FOV in the y axis
	//	2 - The aspect ratio, found by dividing screen width by screen height
	//	3 - The near plane, where anything in front of this is clipped
	//	4 - The far plane, where anything beyond this is clipped
	glm::mat4 projection = glm::perspective(45.0f, mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight(), 0.1f, 100.0f);

	while (!mainWindow.GetShouldClose())
	{
		// In this application, we are running in seconds. If we were to use SDL, we would be running in milliseconds, so we would need to adjust for this.
		GLfloat currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		// Get and handle user input events
		glfwPollEvents();	// glfwPollEvents picks up events such as keyboard presses, mouse movements, clicking to close a window, moving a window, resizing a window, and more!

		camera.KeyControl(mainWindow.GetKeys(), deltaTime);
		camera.MouseControl(mainWindow.GetXChange(), mainWindow.GetYChange());

		// Clear the window

		// Parameters of glClearColour:
		//	1 - Normalised red value
		//	2 - Normalised green value
		//	3 - Normalised blue value
		//	4 - Normalised alpha value
		glClearColor(0.5f, 0.0f, 1.0f, 1.0f);	// glClear clears a screen, ready for us to draw to a new frame. glClearColor lets us set the colour of our new frame, not just a black void! Remember the colour values you set should be normalised.

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// We use glClear to clear specific elements of our window. Pixels on screen contain more than just the colour - e.g., stencil data, depth data, and more. So we specify which to clear, as many as we want. In this case, we just clear all the colour buffers.

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();

		glm::mat4 model(1.0f); // Setup a 4x4 identity matrix so that we can calculate using it later

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));	// Take our identity matrix and apply a translation to it (as of writing this, just move in the x-axis)
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));
		meshList[1]->RenderMesh();

		glUseProgram(0);	// Once we're done with a shader program, remember to unbind it.

		mainWindow.SwapBuffers();
	}

	return 0;
}