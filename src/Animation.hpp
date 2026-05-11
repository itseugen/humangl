#pragma once

#include "humangl.h"
#include "mymath.hpp"

struct BodyPart;
struct Body;

/**
 * @brief A class to handle all the animations and their related functions
 */
class Animation
{
	public:
		Animation();
		~Animation();
		void	clearAnimation(Body &body);
		void	walk(Body &body, float currentTime, float deltaTime);
		void	jump(Body &body, float currentTime, float deltaTime);
		void	setStartTime(float time);
		void	setPositionX(float x);
		bool	getLanded();
	private:
		float	_startTime = 0.0f;
		float	_posX = 0.0f;
		float	_posY = 0.0f;
		Mat4	nod(float time, float nodSpeed);
		Mat4	applyJointRotation(const BodyPart& part, const Mat4&rotation);
		Mat4	rotate_z_clamp(float angle, BodyPart& part);
		float	jumpArc(float u, float height);
		bool	_landed = false;
};
