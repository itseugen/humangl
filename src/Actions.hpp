#pragma once

#include "humangl.h"
#include "Application.hpp"

Mat4	nod(float time, float nodSpeed);
Mat4	applyJointRoation(const BodyPart& part, const Mat4&rotation);