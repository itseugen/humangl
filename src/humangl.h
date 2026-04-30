#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <unordered_map>
#include <math.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>   // get window/context and glfwGetProcAddress

#ifdef __APPLE__
# include <OpenGL/gl.h>        // basic GL types & enums (system header)
#else
# include <GL/gl.h>        // basic GL types & enums (system header)
#endif

// #include "mymath.hpp"
// #include "Animation.hpp"
// #include "Application.hpp"
#include "opengl.hpp"
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
	Jump,
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

	float jointAngleZMin = -360.0f;
	float jointAngleZMax = 360.0f;
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

static constexpr float	DEG2RAD = 3.14159265f / 180.0f;
static constexpr float	DEG2RADFOV = 45.0f * DEG2RAD;

// #include "imgui.h"
// #include "imgui_impl_glfw.h"
// #include "imgui_impl_opengl3.h"

using uint = unsigned int;