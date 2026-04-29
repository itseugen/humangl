#pragma once

#include "humangl.h"
#include "Application.hpp"

struct BodyPart;
struct Body;

// Make class or something to have a start time, or pass it from app
class Animation
{
	public:
		void	clearAnimation(Body &body);
		void	walk(Body &body, float currentTime);
		void	setStartTime(float time);
	private:
		float	_startTime = 0.0f;
		Mat4	nod(float time, float nodSpeed);
		Mat4	applyJointRoation(const BodyPart& part, const Mat4&rotation);
		Mat4	rotate_z_clamp(float angle, BodyPart& part);
}
