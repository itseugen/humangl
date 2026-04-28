#pragma once

#include "humangl.h"
#include "Application.hpp"

struct BodyPart;
struct Body;

Mat4	nod(float time, float nodSpeed);
Mat4	applyJointRoation(const BodyPart& part, const Mat4&rotation);
Mat4	rotate_z_clamp(float angle, BodyPart& part);

// Make class or something to have a start time, or pass it from app
namespace Animation
{
	void	clearAnimation(Body &body);
	void	walk(Body &body, float time);
}
