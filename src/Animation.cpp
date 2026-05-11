#include "Animation.hpp"

Animation::Animation()
{
}

Animation::~Animation()
{
}

/**
 * @brief A sinus function to make a nodding motion
 */
Mat4	Animation::nod(float time, float nodSpeed)
{
	float nodAngle = sin(time * nodSpeed) * 0.4f;
	return  rotate_z(nodAngle);
}

/**
 * @brief Applies any rotation to a joint
 */
Mat4	Animation::applyJointRotation(const BodyPart& part, const Mat4&rotation)
{
	return translate(part.jointPivot) * rotation * translate(-part.jointPivot);
}

/**
 * @brief A small helper function that applies the joint limitations to any Z-Axis rotation
 */
Mat4	Animation::rotate_z_clamp(float angle, BodyPart& part)
{
	float clampedAngle = myClamp(angle, part.jointAngleZMin, part.jointAngleZMax);
	return rotate_z(clampedAngle);
}

/**
 * @brief Clears all animation Matrixes (replace with identity)
 */
void	Animation::clearAnimation(Body &body)
{
	// To keep the x movement, maybe change later
	// body.torso.animation = mat4_identity();
	body.head.animation = mat4_identity();
	body.upperLeftArm.animation = mat4_identity();
	body.lowerLeftArm.animation = mat4_identity();
	body.upperRightArm.animation = mat4_identity();
	body.lowerRightArm.animation = mat4_identity();
	body.upperLeftLeg.animation = mat4_identity();
	body.lowerLeftLeg.animation = mat4_identity();
	body.upperRightLeg.animation = mat4_identity();
	body.lowerRightLeg.animation = mat4_identity();
}

/**
 * @brief Makes the body do walking motions and move on the X-Axis. Movement is "permanent" to not have hard resets
 * @param body The body to move (I only have one)
 * @param currentTime The time (glfw_getTime)
 * @param deltaTime The delta time to account for lags
 */
void	Animation::walk(Body &body, float currentTime, float deltaTime)
{
	float t = currentTime - this->_startTime;
	t = t * 1.0f;

	float armSwing = sin(t * 0.6f) * 0.4f;
	float legSwing = sin(t * 0.8f) * 0.4f;

	body.upperLeftArm.animation = applyJointRotation(body.upperLeftArm, rotate_z_clamp(armSwing, body.upperLeftArm));
	body.upperRightArm.animation = applyJointRotation(body.upperRightArm, rotate_z_clamp(-armSwing, body.upperRightArm));
	body.upperLeftLeg.animation = applyJointRotation(body.upperLeftLeg, rotate_z_clamp(-legSwing, body.upperLeftLeg));
	body.upperRightLeg.animation = applyJointRotation(body.upperRightLeg, rotate_z_clamp(legSwing, body.upperRightLeg));
	
	body.lowerLeftArm.animation = applyJointRotation(body.lowerLeftArm, rotate_z_clamp(armSwing * 0.5f, body.lowerLeftArm));
	body.lowerRightArm.animation = applyJointRotation(body.lowerRightArm, rotate_z_clamp(-armSwing * 0.5f, body.lowerRightArm));
	body.lowerLeftLeg.animation = applyJointRotation(body.lowerLeftLeg, rotate_z_clamp(-legSwing * 0.5f, body.lowerLeftLeg));
	body.lowerRightLeg.animation = applyJointRotation(body.lowerRightLeg, rotate_z_clamp(legSwing * 0.5f, body.lowerRightLeg));
	
	body.head.animation = applyJointRotation(body.head, rotate_z_clamp(sin(t * 0.5f) * 0.2f, body.head));

	this->_posX -= 0.6f * deltaTime;
	body.torso.animation = translate(this->_posX, 0.0f, 0.0f);
}

/**
 * @brief Makes the body do a jump
 * @param body The body to move (I only have one)
 * @param currentTime The time (glfw_getTime)
 * @param deltaTime The delta time to account for lags
 */
void	Animation::jump(Body &body, float currentTime, float deltaTime)
{
	float t = currentTime - _startTime;
	float angle;
	float crouchY = 0.0f;

	this->_landed = false;

	if (t < 2.0f) // down
	{
		float u = t / 2.0f;
		angle = u * M_PI;
	}
	else if (t < 3.0f) // up again
	{
		float u = (t - 2.0f) / 1.0f;
		angle = M_PI + u * M_PI;
	}

	if (t < 3.0f)
	{
		float kneeBend = (45.0f * 0.5f * (1.0f - cos(angle))) * DEG2RAD;
	
		body.lowerLeftLeg.animation  = applyJointRotation(body.lowerLeftLeg,  rotate_z_clamp(-kneeBend, body.lowerLeftLeg));
		body.lowerRightLeg.animation = applyJointRotation(body.lowerRightLeg, rotate_z_clamp(-kneeBend, body.lowerRightLeg));
	
		float hipBend = kneeBend * 0.5f;
	
		body.upperLeftLeg.animation  = applyJointRotation(body.upperLeftLeg,  rotate_z_clamp(hipBend, body.upperLeftLeg));
		body.upperRightLeg.animation = applyJointRotation(body.upperRightLeg, rotate_z_clamp(hipBend, body.upperRightLeg));
	
		body.lowerLeftArm.animation  = applyJointRotation(body.lowerLeftArm,  rotate_z_clamp(kneeBend, body.lowerLeftArm));
		body.lowerRightArm.animation = applyJointRotation(body.lowerRightArm, rotate_z_clamp(kneeBend, body.lowerRightArm));
	
		float L1 = body.upperLeftLeg.size.y;
		float L2 = body.lowerLeftLeg.size.y;
	
		float H = L1 * cos(hipBend) + L2 * cos(hipBend - kneeBend);
		crouchY = -((L1 + L2) - H);
	}

	float jumpStart = 2.5f;
	float jumpEnd = 5.0f;
	float jumpY = 0.0f;
	if (t > jumpStart)
	{
		float u = (t - jumpStart) / (jumpEnd - jumpStart);
		u = myClamp(u, 0.0f, 1.0f);
		jumpY = jumpArc(u, 5.0f);
	}
	float landStart = jumpEnd -0.2f;
	float landEnd = landStart + 3.0f;
	if (t > landStart && t < landEnd)
	{
		if (t < landStart + 1.0f)
		{
			float u = (t - landStart) / 1.0f;
			angle = u * M_PI;
		}
		else
		{
			float u = (t - (landStart + 1.0f)) / 2.0f;
			angle = M_PI - (u * M_PI);
		}
		float kneeBend = (45.0f * 0.5f * (1.0f - cos(angle))) * DEG2RAD;

		body.lowerLeftLeg.animation  = applyJointRotation(body.lowerLeftLeg,  rotate_z_clamp(-kneeBend, body.lowerLeftLeg));
		body.lowerRightLeg.animation = applyJointRotation(body.lowerRightLeg, rotate_z_clamp(-kneeBend, body.lowerRightLeg));

		float hipBend = kneeBend * 0.5f;

		body.upperLeftLeg.animation  = applyJointRotation(body.upperLeftLeg,  rotate_z_clamp(hipBend, body.upperLeftLeg));
		body.upperRightLeg.animation = applyJointRotation(body.upperRightLeg, rotate_z_clamp(hipBend, body.upperRightLeg));

		body.lowerLeftArm.animation  = applyJointRotation(body.lowerLeftArm,  rotate_z_clamp(kneeBend, body.lowerLeftArm));
		body.lowerRightArm.animation = applyJointRotation(body.lowerRightArm, rotate_z_clamp(kneeBend, body.lowerRightArm));

		float L1 = body.upperLeftLeg.size.y;
		float L2 = body.lowerLeftLeg.size.y;

		float H = L1 * cos(hipBend) + L2 * cos(hipBend - kneeBend);
		crouchY = -((L1 + L2) - H);
	}

	if (t > landEnd)
		this->_landed = true;

	this->_posY = crouchY + jumpY;

	body.torso.animation = translate(this->_posX, this->_posY, 0.0f);
}

float	Animation::jumpArc(float u, float height)
{
	return (height * 4.0f * u * (1.0f - u));
}

void	Animation::setStartTime(float time)
{
	this->_startTime = time;
}

void	Animation::setPositionX(float x)
{
	this->_posX = x;
}

bool	Animation::getLanded()
{
	return this->_landed;
}
