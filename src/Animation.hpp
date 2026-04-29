#pragma once

#include "humangl.h"
#include "mymath.hpp"

struct BodyPart;
struct Body;

class Animation
{
	public:
		Animation();
		~Animation();
		void	clearAnimation(Body &body);
		void	walk(Body &body, float currentTime, float deltaTime);
		void	setStartTime(float time);
		void	setPositionX(float x);
	private:
		float	_startTime = 0.0f;
		float	_posX = 0.0f;
		Mat4	nod(float time, float nodSpeed);
		Mat4	applyJointRoation(const BodyPart& part, const Mat4&rotation);
		Mat4	rotate_z_clamp(float angle, BodyPart& part);
};
