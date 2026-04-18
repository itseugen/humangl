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

	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// glfwSetInputMode(win, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	glfwSetWindowUserPointer(win, this);
	glfwSetCursorPosCallback(win, mouseCallback);

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
	// Cleanup
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

void	Application::push(const Mat4& mat)
{
	this->_stack.push_back(mat);
}

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

void	Application::drawCube(const Mat4& mvp, const Colour& colour)
{
	glUseProgram(this->_prog);
	glUniformMatrix4fv(this->_mpvLoc, 1, GL_FALSE, mvp.m);

	glUniform3f(this->_colLoc, colour.r, colour.g, colour.b);

	glBindVertexArray(this->_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

/**
 * @brief Setup all the keybinds to use
 */
void	Application::keybinds()
{
	if(glfwGetKey(this->_win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(this->_win,1);


	float	speed = 1.5f * this->_dt; // Adjust speed based on frame time
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
}

void	Application::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	if(app->_firstMouse)
	{
		app->_lastMouseX = (float)xpos;
		app->_lastMouseY = (float)ypos;
		app->_firstMouse = false;
	}

	float xoffset = (float)xpos - app->_lastMouseX;
	float yoffset = app->_lastMouseY - (float)ypos; // reversed since y-coordinates go from bottom to top

	app->_lastMouseX = (float)xpos;
	app->_lastMouseY = (float)ypos;

	float sensitivity = 0.001f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	std::cout << "Yaw: " << app->_yaw << " Pitch: " << app->_pitch << "\n";
	std::cout << "Camera Front: (" << app->_cameraFront.x << ", " << app->_cameraFront.y << ", " << app->_cameraFront.z << ")\n";
	std::cout << "oxffset: (" << xoffset << ", " << yoffset << ")\n";
	app->_yaw   += xoffset;
	app->_pitch += yoffset;

	if(app->_pitch > 89.0f) app->_pitch = 89.0f;
	if(app->_pitch < -89.0f) app->_pitch = -89.0f;

	//update the direction
	Vec3 front;
	front.x = cosf(app->_yaw * 3.14159265f / 180.0f) * cosf(app->_pitch * 3.14159265f / 180.0f);
	front.y = sinf(app->_pitch * 3.14159265f / 180.0f);
	front.z = sinf(app->_yaw * 3.14159265f / 180.0f) * cosf(app->_pitch * 3.14159265f / 180.0f);
	app->_cameraFront = norm(front);
}