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

/**
 * @brief The different textures to apply, or none to apply a colour
 */
enum class TextureType
{
	None,
	Unicorn,
	Dirt,
};

/**
 * @brief The different animation to apply or none to stand still
 */
enum class FullBodyAnimation
{
	None,
	Walk,
	Jump,
};

/**
 * @brief Each subsection of a body. Always only a box
 */
struct BodyPart
{
	Mat4 local; // Position relative to parent (or world if root)
	Mat4 shape; // Shape of the object, same as size in Mat4 form
	Vec3 size; // Size (shape) of all object sides, same as shape in Vec3 form
	Vec3 jointPivot; // The point around the joint rotates (always the joint connecting it to its parent)
	Colour colour = Colour(1.0f, 1.0f, 1.0f); // The colour of the body part, not visible if texture is set
	TextureType tex = TextureType::None; // The Texture the body part is set to, or None if a colour should be shown
	Mat4 animation = mat4_identity(); // The state of the current animation

	float jointAngleZMin = -360.0f; // The minimum rotation of the joint (to parent) on the z axis
	float jointAngleZMax = 360.0f; // The maximum rotation of the joint (to parent) on the z axis
};

/**
 * @brief Stores all the body parts of the overall body
 */
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