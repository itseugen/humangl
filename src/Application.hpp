#pragma once

#include "humangl.h"

struct Application
{
	GLFWwindow*	_win = nullptr;

	GLuint	_VAO = 0;
	GLuint	_VBO = 0;
	GLuint	_prog;

	Application();
	~Application();

	void	initWindow();
	int		setupBuffers();
};

GLuint compileShader(GLenum type, const char* src);
GLuint linkProgram(GLuint vs, GLuint fs);