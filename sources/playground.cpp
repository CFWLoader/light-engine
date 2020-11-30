// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include <common/shader.hpp>

#include <BatchData.hpp>
#include <BMPLoader.hpp>

int drawTexture(int programID, const BMPLoader& bmp) {
	GLuint textureID;
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp.GetWidth(), bmp.GetHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, bmp.GetData());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	return 0;
}

void drawTextureDemo() {
	BMPLoader bmpInst("uvtemplate.bmp");

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("DrawTexture.vertexshader", "DrawTexture.fragmentshader");

	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

	Rect rectInst = Rect(0.0, 0.0, 0.0, 1.0, 1.0);
	// Rect rectInst2 = Rect(0.0f, 0.0f, 0.0f, 0.2f, 0.2f);
	// rectInst2.setColor(0.8f, 0.8f, 0.5f);
	BatchData primData = BatchData();
	primData.addRect(rectInst);
	// primData.addRect(rectInst2);

	static GLfloat* g_vertex_buffer_data =  primData.getVBO();
	// static GLfloat* g_color_buffer_data = primData.getColorBuffer();
	static GLfloat g_uv_buffer_data[] = { 
		0.0f, 0.0f, 
		1.0f, 0.0f, 
		0.0f, 1.0f, 
		1.0f, 0.0f, 
		1.0f, 1.0f, 
		0.0f, 1.0f };
	/*
	for (int idx = 0; idx < 18; idx += 3) {
		printf("(%f, %f, %f)\n", g_vertex_buffer_data[idx], g_vertex_buffer_data[idx + 1], g_vertex_buffer_data[idx + 2]);
	}
	*/
	const size_t vertNum = primData.numOfVertex();

	// printf("%d", vertNum);

	/*
	static const GLfloat g_vertex_buffer_data[] = {
		-0.2f, -0.2f, 0.0f,
		 0.2f, -0.2f, 0.0f,
		-0.2f,  0.2f, 0.0f,
		0.1f,  0.3f, 0.0f,
		0.3f,  0.3f, 0.0f,
		0.4f,  0.4f, 0.0f,
	};
	*/

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertNum * 3, g_vertex_buffer_data, GL_STATIC_DRAW);

	/*
	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertNum * 3, g_color_buffer_data, GL_STATIC_DRAW);
	*/
	GLuint uvBuffer;
	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertNum * 2, g_uv_buffer_data, GL_STATIC_DRAW);

	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		drawTexture(programID, bmpInst);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
		glVertexAttribPointer(
			1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			2,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 6); // 3 indices starting at 0 -> 1 triangle

		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);
}

void drawMulRectDemo() {
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	Rect rectInst = Rect(0.0, 0.0, 0.1f);
	Rect rectInst2 = Rect(0.0f, 0.0f, 0.0f, 0.2f, 0.2f);
	rectInst2.setColor(0.8f, 0.8f, 0.5f);
	BatchData primData = BatchData();
	primData.addRect(rectInst);
	primData.addRect(rectInst2);

	static GLfloat* g_vertex_buffer_data = primData.getVBO();
	static GLfloat* g_color_buffer_data = primData.getColorBuffer();
	/*
	for (int idx = 0; idx < 18; idx += 3) {
		printf("(%f, %f, %f)\n", g_vertex_buffer_data[idx], g_vertex_buffer_data[idx + 1], g_vertex_buffer_data[idx + 2]);
	}
	*/
	const size_t vertNum = primData.numOfVertex();

	/*
	static const GLfloat g_vertex_buffer_data[] = {
		-0.2f, -0.2f, 0.0f,
		 0.2f, -0.2f, 0.0f,
		-0.2f,  0.2f, 0.0f,
		0.1f,  0.3f, 0.0f,
		0.3f,  0.3f, 0.0f,
		0.4f,  0.4f, 0.0f,
	};
	*/

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertNum * 3, g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertNum * 3, g_color_buffer_data, GL_STATIC_DRAW);

	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, vertNum); // 3 indices starting at 0 -> 1 triangle

		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);
}

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Tutorial 02 - Red triangle", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// drawMulRectDemo();
	drawTextureDemo();

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

