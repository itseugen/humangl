#pragma once

#include "humangl.h"
#include "mymath.hpp"

/**
 * @brief Simply rgb colour
 */
struct Colour
{
	float	r, g, b;

	Colour() : r(1.0f), g(1.0f), b(1.0f) {}
	Colour(float red, float green, float blue): r(red), g(green), b(blue) {}
};

struct Application
{
	GLFWwindow*	_win = nullptr;

	GLuint	_VAO = 0;
	GLuint	_VBO = 0;
	GLuint	_prog;

	float	_tx = 0.0f;
	float	_ty = 0.0f;
	float	_tz = -2.5f;
	float	_dt;

	std::vector<Mat4>	_stack;

	Application();
	~Application();

	void	initWindow();
	int		setupBuffers();

	void	keybinds();

	void		push(const Mat4& mat);
	Mat4	pop();

	void	drawCube(const Mat4& mvp);

	private:
		GLuint	_mpvLoc;
};

GLuint compileShader(GLenum type, const char* src);
GLuint linkProgram(GLuint vs, GLuint fs);