#include "Application.hpp"

Application::Application()
{
	initWindow();
	if(this->_win == nullptr) return;
	// setupImGui();
	setupBuffers();
	this->_mpvLoc = glGetUniformLocation(this->_prog, "uMVP");
	this->_colLoc = glGetUniformLocation(this->_prog, "uColor");
}

Application::~Application()
{
	glDeleteBuffers(1, &this->_VBO);
	glDeleteVertexArrays(1, &this->_VAO);
	// ImGui_ImplOpenGL3_Shutdown();
	// ImGui_ImplGlfw_Shutdown();
	// ImGui::DestroyContext();
	glfwDestroyWindow(this->_win);
	glfwTerminate();
}

/**
 * Basic window initialization
 */
void	Application::initWindow()
{
	if(!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor*	monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	GLFWwindow*	win = glfwCreateWindow(this->_winWidth, this->_winHeight, "HumanGL", nullptr, nullptr);
	if(!win)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(win);

		if(!loadGLFunctions())
	{
		std::cerr<<"Failed loading GL functions\n";
		return;
	}

	glfwSetWindowUserPointer(win, this);
	glfwSetKeyCallback(win, keyCallback);

	this->_win = win;
}

int	Application::setupBuffers()
{
	const char* vs_src;
	const char* fs_src;

		vs_src = R"(#version 330 core
		layout(location=0) in vec3 aPos;
		uniform mat4 uMVP;
		uniform vec3 uColor;
		out vec3 vColor;
		void main()
		{
			vColor = uColor;
			gl_Position = uMVP * vec4(aPos, 1.0);
		}
		)";
		fs_src = R"(#version 330 core
		in vec3 vColor; out vec4 FragColor;
		void main(){ FragColor = vec4(vColor,1.0); }
		)";
		// VAO -> Vertex Array Object; VBO -> Vertex Buffer Object, stores data in GPU
	glGenVertexArrays(1, &this->_VAO);
	glGenBuffers(1, &this->_VBO);

	glBindVertexArray(this->_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);

	static const GLfloat	cube[] =
	{
	// Front face (z = 0.5)
	-0.5f,-0.5f, 0.5f,
	 0.5f,-0.5f, 0.5f,
	 0.5f, 0.5f, 0.5f,

	-0.5f,-0.5f, 0.5f,
	 0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,

	// Back face (z = -0.5)
	-0.5f,-0.5f,-0.5f,
	 0.5f, 0.5f,-0.5f,
	 0.5f,-0.5f,-0.5f,

	-0.5f,-0.5f,-0.5f,
	-0.5f, 0.5f,-0.5f,
	 0.5f, 0.5f,-0.5f,

	// Left face (x = -0.5)
	-0.5f,-0.5f,-0.5f,
	-0.5f,-0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,

	-0.5f,-0.5f,-0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f,-0.5f,

	// Right face (x = 0.5)
	 0.5f,-0.5f,-0.5f,
	 0.5f, 0.5f, 0.5f,
	 0.5f,-0.5f, 0.5f,

	 0.5f,-0.5f,-0.5f,
	 0.5f, 0.5f,-0.5f,
	 0.5f, 0.5f, 0.5f,

	// Top face (y = 0.5)
	-0.5f, 0.5f,-0.5f,
	-0.5f, 0.5f, 0.5f,
	 0.5f, 0.5f, 0.5f,

	-0.5f, 0.5f,-0.5f,
	 0.5f, 0.5f, 0.5f,
	 0.5f, 0.5f,-0.5f,

	// Bottom face (y = -0.5)
	-0.5f,-0.5f,-0.5f,
	 0.5f,-0.5f, 0.5f,
	-0.5f,-0.5f, 0.5f,

	-0.5f,-0.5f,-0.5f,
	 0.5f,-0.5f,-0.5f,
	 0.5f,-0.5f, 0.5f
	};

	// // Allows GPU to access the basic cube building block (uploads it)
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	// // First 3 floats are coordinates
	glEnableVertexAttribArray(0);
	// 6 * sizeof(float) because each vertex has 6 floats (3 for position, 3 for color)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0);

	glBindVertexArray(0);
	// compile shaders + link
	GLuint vs = compileShader(GL_VERTEX_SHADER, vs_src);
	GLuint fs = compileShader(GL_FRAGMENT_SHADER, fs_src);
	if(!vs || !fs)
	{
		std::cerr<<"Shader compile error\n";
		return -1;
	}

	this->_prog = linkProgram(vs, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
	return 0;
}

GLuint compileShader(GLenum type, const char* src)
{
	// new shader
	GLuint s = glCreateShader(type);
	glShaderSource(s, 1, &src, nullptr);
	glCompileShader(s);
	// Query Compilation success
	GLint ok=0; glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
	if(!ok)
	{
		char buf[1024]; glGetShaderInfoLog(s, 1024, nullptr, buf);
		std::cerr<<"Shader compile error: "<<buf<<"\n";
		return 0;
	}
	return s;
}

GLuint linkProgram(GLuint vs, GLuint fs)
{
	// Empty program
	GLuint p = glCreateProgram();
	glAttachShader(p, vs);
	glAttachShader(p, fs);
	glLinkProgram(p);
	// Query Linking success
	GLint ok=0; glGetProgramiv(p, GL_LINK_STATUS, &ok);
	if(!ok)
	{
		char buf[1024]; glGetProgramInfoLog(p, 1024, nullptr, buf);
		std::cerr<<"Program link error: "<< buf<<"\n";
		return 0;
	}
	return p;
}

/// @brief Addes a matrix to the top of the stack, used for hierarchical transformations (like a model matrix for an object, then pushing a child object with its own transform on top)
/// @param mat the matrix to push onto the stack
void	Application::push(const Mat4& mat)
{
	this->_stack.push_back(mat);
}

/// @brief Pops a matrix from the top of the stack
/// @return The popped matrix
Mat4	Application::pop()
{
	if(this->_stack.empty())
	{
		throw std::runtime_error("Matrix stack underflow!");
	}
	Mat4 top = this->_stack.back();
	this->_stack.pop_back();
	return top;
}

/// @brief Draws a 1x1x1 cube centered at the origin using the provided MVP matrix and colour
/// @param mvp The combined Model-View-Projection matrix to transform the cube vertices
/// @param colour The RGB colour to use for the cube
void	Application::drawCube(const Mat4& mvp, const Colour& colour)
{
	glUseProgram(this->_prog);
	glUniformMatrix4fv(this->_mpvLoc, 1, GL_FALSE, mvp.m);

	glUniform3f(this->_colLoc, colour.r, colour.g, colour.b);

	glBindVertexArray(this->_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

/**
 * @brief Setup all the keybinds that could be used continuously (like holding down W to move forward)
 */
void	Application::keybinds()
{
	float	speed = 1.5f * this->_dt; // Adjust speed based on frame time
	float	rotSpeed = 40.0f * this->_dt;
	Vec3 right = norm(cross(this->_cameraFront, this->_cameraUp));
	if (glfwGetKey(_win, GLFW_KEY_W) == GLFW_PRESS)
		_cameraPosition += _cameraFront * speed;

	if (glfwGetKey(_win, GLFW_KEY_S) == GLFW_PRESS)
		_cameraPosition -= _cameraFront * speed;

	if (glfwGetKey(_win, GLFW_KEY_A) == GLFW_PRESS)
		_cameraPosition -= right * speed;

	if (glfwGetKey(_win, GLFW_KEY_D) == GLFW_PRESS)
		_cameraPosition += right * speed;

	if (glfwGetKey(_win, GLFW_KEY_SPACE) == GLFW_PRESS)
		_cameraPosition += _cameraUp * speed;

	if (glfwGetKey(_win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		_cameraPosition -= _cameraUp * speed;

	if (glfwGetKey(_win, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		this->_yaw -= rotSpeed;
		this->updateCameraDirection();
	}
	if (glfwGetKey(_win, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		this->_yaw += rotSpeed;
		this->updateCameraDirection();
	}
	if (glfwGetKey(_win, GLFW_KEY_UP) == GLFW_PRESS)
	{
		this->_pitch += rotSpeed;
		if(this->_pitch > 89.0f) this->_pitch = 89.0f;
		this->updateCameraDirection();
	}
	if (glfwGetKey(_win, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		this->_pitch -= rotSpeed;
		if(this->_pitch < -89.0f) this->_pitch = -89.0f;
		this->updateCameraDirection();
	}
}

/// @brief THe mouse callback function for GLFW, handles mouse movement and updates the camera direction accordingly
/// @param window 
/// @param xpos 
/// @param ypos 
void	Application::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	if(app->_firstMouse)
	{
		app->_lastMouseX = (float)xpos;
		app->_lastMouseY = (float)ypos;
		app->_firstMouse = false;
		return;
	}

	float sensitivity = 0.001f;
	float deg2rad = 3.14159265f / 180.0f;

	float xoffset = ((float)xpos - app->_lastMouseX) * sensitivity;
	float yoffset = (app->_lastMouseY - (float)ypos) * sensitivity; // reversed since y-coordinates go from bottom to top

	app->_lastMouseX = (float)xpos;
	app->_lastMouseY = (float)ypos;

	app->_yaw   += xoffset;
	app->_pitch += yoffset;

	if(app->_pitch > 89.0f) app->_pitch = 89.0f;
	if(app->_pitch < -89.0f) app->_pitch = -89.0f;

	float yawRad = app->_yaw * deg2rad;
	float pitchRad = app->_pitch * deg2rad;

	Vec3 front;
	front.x = cosf(yawRad) * cosf(pitchRad);
	front.y = sinf(pitchRad);
	front.z = sinf(yawRad) * cosf(pitchRad);
	app->_cameraFront = norm(front);
}

/// @brief The key callback function for GLFW, handles key presses and releases
/// @param window 
/// @param key 
/// @param scancode 
/// @param action 
/// @param mods 
void	Application::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window,1);
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		if(app->_mouseActive == false)
		{
			glfwSetCursorPosCallback(app->_win, mouseCallback);
			glfwSetInputMode(app->_win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSetCursorPos(app->_win, app->_winWidth / 2.0, app->_winHeight / 2.0);
			app->_mouseActive = true;
			app->_firstMouse = true;
		}
		else
		{
			glfwSetCursorPosCallback(app->_win, nullptr);
			glfwSetInputMode(app->_win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			app->_mouseActive = false;
		}
	}
}

/// @brief Standard MVP calculation (Replace DEG2RADFOV with DEG2RAD and the fov if it ever becomes a variable)
/// @return The combined Model-View-Projection matrix
Mat4	Application::calcMVP()
{
	Mat4	proj = perspective(DEG2RADFOV, (float)this->_winWidth/(float)this->_winHeight, 0.1f, 100.f);
	Mat4	view = lookAt(
		Vec3{this->_cameraPosition.x, this->_cameraPosition.y, this->_cameraPosition.z},
		Vec3{this->_cameraPosition.x + this->_cameraFront.x, this->_cameraPosition.y + this->_cameraFront.y, this->_cameraPosition.z + this->_cameraFront.z},
		this->_cameraUp
	);
	Mat4	model = mat4_mul(translate(this->_tx, this->_ty, this->_tz), rotate_y(0.0f));
	return mat4_mul(mat4_mul(proj, view), model);
}

void	Application::updateCameraDirection()
{
	float	yawRadian = this->_yaw * DEG2RAD;
	float	pitchRadian = this->_pitch * DEG2RAD;

	Vec3	front(
		cosf(yawRadian) * cosf(pitchRadian),
		sinf(pitchRadian),
		sinf(yawRadian) * cosf(pitchRadian)
	);
	this->_cameraFront = norm(front);
}
