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

enum class TextureType
{
	None,
	Unicorn,
	Dirt,
};

struct BodyPart
{
	Mat4 local; // Position relative to parent (or world if root)
	Mat4 shape;
	Colour colour;
	TextureType tex;
};

struct Body
{
	BodyPart	torso;
	BodyPart	head;
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
	Vec3 _cameraPosition = {-6.0f, 2.0f, 14.0f};

	Application();
	~Application();

	void	initWindow();
	int		setupBuffers();

	void	keybinds();
	
	void		push(const Mat4& mat);
	Mat4		pop();
	Mat4&		top();
	const Mat4&	top() const;
	
	void	draw(const BodyPart& bodyPart, const Mat4& world);

	std::unordered_map<TextureType, GLuint>	_textures;

	Body	_body;

	private:
		GLuint	_mpvLoc;
		GLint	_colLoc;
		GLint	_useTextureLoc;
		GLint	_texLoc;
		bool	_mouseActive = false;
		static void	keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void	mouseCallback(GLFWwindow* window, double xpos, double ypos);
		static constexpr float	DEG2RAD = 3.14159265f / 180.0f;
		static constexpr float	DEG2RADFOV = 45.0f * DEG2RAD;
		void	updateCameraDirection();
		GLuint	loadPPM(const std::string& path, int& width, int&height);
		void	initBody();
			std::vector<Mat4>	_stack;
};

GLuint compileShader(GLenum type, const char* src);
GLuint linkProgram(GLuint vs, GLuint fs);