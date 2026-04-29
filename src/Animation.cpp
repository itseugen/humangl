#include "Animation.hpp"

Mat4	Animation::nod(float time, float nodSpeed)
{
	float nodAngle = sin(time * nodSpeed) * 0.4f;
	return  rotate_z(nodAngle);
}

Mat4	Animation::applyJointRoation(const BodyPart& part, const Mat4&rotation)
{
	return translate(part.jointPivot) * rotation * translate(-part.jointPivot);
}

Mat4	Animation::rotate_z_clamp(float angle, BodyPart& part)
{
	float clampedAngle = myClamp(angle, part.jointAngleZMin, part.jointAngleZMax);
	return rotate_z(clampedAngle);
}

/// @brief Clears any animation from the body
void	Animation::clearAnimation(Body &body)
{
	body.torso.animation = mat4_identity();
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

void	Animation::walk(Body &body, float time)
{
	float t = time * 1.0f;

	float armSwing = sin(t * 0.6f) * 0.4f;
	float legSwing = sin(t * 0.8f) * 0.4f;

	// IF i want to move the torso along the walk,
	// either add it to the main or make animation its own class that has a start time etc

	body.upperLeftArm.animation = applyJointRoation(body.upperLeftArm, rotate_z_clamp(armSwing, body.upperLeftArm));
	body.upperRightArm.animation = applyJointRoation(body.upperRightArm, rotate_z_clamp(-armSwing, body.upperRightArm));
	body.upperLeftLeg.animation = applyJointRoation(body.upperLeftLeg, rotate_z_clamp(-legSwing, body.upperLeftLeg));
	body.upperRightLeg.animation = applyJointRoation(body.upperRightLeg, rotate_z_clamp(legSwing, body.upperRightLeg));

	body.lowerLeftArm.animation = applyJointRoation(body.lowerLeftArm, rotate_z_clamp(armSwing * 0.5f, body.lowerLeftArm));
	body.lowerRightArm.animation = applyJointRoation(body.lowerRightArm, rotate_z_clamp(-armSwing * 0.5f, body.lowerRightArm));
	body.lowerLeftLeg.animation = applyJointRoation(body.lowerLeftLeg, rotate_z_clamp(-legSwing * 0.5f, body.lowerLeftLeg));
	body.lowerRightLeg.animation = applyJointRoation(body.lowerRightLeg, rotate_z_clamp(legSwing * 0.5f, body.lowerRightLeg));

	body.head.animation = applyJointRoation(body.head, rotate_z_clamp(sin(t * 0.5f) * 0.2f, body.head));
}
