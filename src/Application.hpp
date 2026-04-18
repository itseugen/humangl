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

	// Mouse stuff
	float	_yaw = -90.0f;
	float	_pitch = 0.0f;
	float	_lastMouseX = 640.0f;
	float	_lastMouseY = 360.0f;
	bool	_firstMouse = true;

	int		_winHeight = 1080;
	int		_winWidth = 1920;

	Vec3 _cameraFront = {0, 0, -1};
	Vec3 _cameraUp = {0, 1, 0};
	Vec3 _cameraPosition = {0, 0.6f, 3.0f};

	std::vector<Mat4>	_stack;

	Application();
	~Application();

	void	initWindow();
	int		setupBuffers();

	void	keybinds();
	
	void	push(const Mat4& mat);
	Mat4	pop();
	
	void	drawCube(const Mat4& mvp, const Colour& colour);
	
	
	private:
		GLuint	_mpvLoc;
		GLint	_colLoc;
		bool	_mouseActive = false;
		static void	keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void	mouseCallback(GLFWwindow* window, double xpos, double ypos);
};

GLuint compileShader(GLenum type, const char* src);
GLuint linkProgram(GLuint vs, GLuint fs);