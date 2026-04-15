#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>   // get window/context and glfwGetProcAddress

#ifdef __APPLE__
# include <OpenGL/gl.h>        // basic GL types & enums (system header)
#else
# include <GL/gl.h>        // basic GL types & enums (system header)
#endif


// #include "mymath.hpp"
#include "Application.hpp"
#include "opengl.hpp"
#include "mymath.hpp"

// #include "imgui.h"
// #include "imgui_impl_glfw.h"
// #include "imgui_impl_opengl3.h"

using uint = unsigned int;