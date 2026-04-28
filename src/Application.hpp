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

enum class FullBodyAnimation
{
	None,
	Walk,
};

struct BodyPart
{
	Mat4 local; // Position relative to parent (or world if root)
	Mat4 shape; // stays for now to save calculations but could be replaced with a Vec3 size and a mat4_scale in the draw function
	Vec3 size; // To not always access the matrix of shape
	Vec3 jointPivot; // The point around the joint rotates (always the joint connecting it to its parent)
	Colour colour = Colour(1.0f, 1.0f, 1.0f);
	TextureType tex = TextureType::None;
	Mat4 animation = mat4_identity();
};

struct Body
{
	BodyPart	torso;
	BodyPart	head;
	BodyPart	upperLeftArm;
	BodyPart	upperRightArm;
	BodyPart	lowerLeftArm;
	BodyPart	lowerRightArm;
	BodyPart	upperLeftLeg;
	BodyPart	upperRightLeg;
	BodyPart	lowerLeftLeg;
	BodyPart	lowerRightLeg;
};

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
	Vec3 _cameraPosition = {-6.0f, 2.0f, 14.0f};
	
	std::unordered_map<TextureType, GLuint>	_textures;

	FullBodyAnimation	_animation = FullBodyAnimation::None;
	
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
		static constexpr float	DEG2RAD = 3.14159265f / 180.0f;
		static constexpr float	DEG2RADFOV = 45.0f * DEG2RAD;

		void	updateCameraDirection();
		static void	keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void	mouseCallback(GLFWwindow* window, double xpos, double ypos);
		GLuint	loadPPM(const std::string& path, int& width, int&height);
		void	initBody();
};

GLuint compileShader(GLenum type, const char* src);
GLuint linkProgram(GLuint vs, GLuint fs);