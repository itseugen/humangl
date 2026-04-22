#include "Actions.hpp"

Mat4 nod(float time, float nodSpeed)
{
	float nodAngle = sin(time * nodSpeed) * 0.4f;
	return  rotate_z(nodAngle);
}