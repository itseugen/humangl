#pragma once

#include "humangl.h"
#include "mymath.hpp"
#include "Animation.hpp"

struct Application
{
	
	Application();
	~Application();

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
	Vec3 _cameraPosition = {-6.0f, 2.0f, 30.0f};
	
	std::unordered_map<TextureType, GLuint>	_textures;

	FullBodyAnimation	_animation = FullBodyAnimation::None;
	Animation			_animationController;

	Body	_body;

	void	initWindow();
	int		setupBuffers();

	void	keybinds();
	
	void		push(const Mat4& mat);
	Mat4		pop();
	Mat4&		top();
	const Mat4&	top() const;
	
	void	draw(const BodyPart& bodyPart, const Mat4& world);
	void	resizeBody();

	private:
		GLuint	_mpvLoc;
		GLint	_colLoc;
		GLint	_useTextureLoc;
		GLint	_texLoc;
		std::vector<Mat4>	_stack;
		bool	_mouseActive = false;
		float	_moveSpeed = 1.5f;

		void	updateCameraDirection();
		static void	keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void	mouseCallback(GLFWwindow* window, double xpos, double ypos);
		GLuint	loadPPM(const std::string& path, int& width, int&height);
		void	initBody();
};

GLuint compileShader(GLenum type, const char* src);
GLuint linkProgram(GLuint vs, GLuint fs);