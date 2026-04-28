#include "Application.hpp"

Application::Application()
{
	initWindow();
	if(this->_win == nullptr) return;
	// setupImGui();
	setupBuffers();
	this->_mpvLoc = glGetUniformLocation(this->_prog, "uMVP");
	this->_colLoc = glGetUniformLocation(this->_prog, "uColor");
	this->_useTextureLoc = glGetUniformLocation(_prog, "uUseTexture");
	this->_texLoc = glGetUniformLocation(this->_prog, "uTexture");

	int w, h;
	this->_textures[TextureType::Unicorn] = loadPPM("assets/unicorn.ppm", w, h);
	if (this->_textures[TextureType::Unicorn] == 0)
		std::cerr << "Loading Unicorn failed" << std::endl;
	this->_textures[TextureType::Dirt] = loadPPM("assets/dirt.ppm", w, h);
	if (this->_textures[TextureType::Dirt] == 0)
		std::cerr << "Loading Dirt failed" << std::endl;
	
	this->initBody();
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
		layout(location = 0) in vec3 aPos;
		layout(location = 1) in vec2 aTex;

		uniform mat4 uMVP;

		out vec2 vTex;

		void main()
		{
			vTex = aTex;
			gl_Position = uMVP * vec4(aPos, 1.0);
		}
		)";
		fs_src = R"(#version 330 core
		in vec2 vTex;

		uniform sampler2D uTexture;
		uniform bool uUseTexture;
		uniform vec3 uColor;

		out vec4 FragColor;

		void main()
		{
			vec2 flippedUV = vec2(vTex.x, 1.0 - vTex.y);
			vec4 texColor = texture(uTexture, flippedUV);

			FragColor = mix(
				vec4(uColor, 1.0),
				texColor,
				float(uUseTexture)
			);
		}
		)";
		// VAO -> Vertex Array Object; VBO -> Vertex Buffer Object, stores data in GPU
	glGenVertexArrays(1, &this->_VAO);
	glGenBuffers(1, &this->_VBO);

	glBindVertexArray(this->_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);

	static const GLfloat cube[] =
	{
	// Front face (z = 0.5)
	-0.5f,-0.5f, 0.5f,  0.0f, 0.0f,
	0.5f,-0.5f, 0.5f,  1.0f, 0.0f,
	0.5f, 0.5f, 0.5f,  1.0f, 1.0f,

	-0.5f,-0.5f, 0.5f,  0.0f, 0.0f,
	0.5f, 0.5f, 0.5f,  1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f,  0.0f, 1.0f,

	// Back face (z = -0.5)
	-0.5f,-0.5f,-0.5f,  1.0f, 0.0f,
	0.5f, 0.5f,-0.5f,  0.0f, 1.0f,
	0.5f,-0.5f,-0.5f,  0.0f, 0.0f,

	-0.5f,-0.5f,-0.5f,  1.0f, 0.0f,
	-0.5f, 0.5f,-0.5f,  1.0f, 1.0f,
	0.5f, 0.5f,-0.5f,  0.0f, 1.0f,

	// Left face (x = -0.5)
	-0.5f,-0.5f,-0.5f,  0.0f, 0.0f,
	-0.5f,-0.5f, 0.5f,  1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,  1.0f, 1.0f,

	-0.5f,-0.5f,-0.5f,  0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,  1.0f, 1.0f,
	-0.5f, 0.5f,-0.5f,  0.0f, 1.0f,

	// Right face (x = 0.5)
	0.5f,-0.5f,-0.5f,  1.0f, 0.0f,
	0.5f, 0.5f, 0.5f,  0.0f, 1.0f,
	0.5f,-0.5f, 0.5f,  0.0f, 0.0f,

	0.5f,-0.5f,-0.5f,  1.0f, 0.0f,
	0.5f, 0.5f,-0.5f,  1.0f, 1.0f,
	0.5f, 0.5f, 0.5f,  0.0f, 1.0f,

	// Top face (y = 0.5)
	-0.5f, 0.5f,-0.5f,  0.0f, 1.0f,
	-0.5f, 0.5f, 0.5f,  0.0f, 0.0f,
	0.5f, 0.5f, 0.5f,  1.0f, 0.0f,

	-0.5f, 0.5f,-0.5f,  0.0f, 1.0f,
	0.5f, 0.5f, 0.5f,  1.0f, 0.0f,
	0.5f, 0.5f,-0.5f,  1.0f, 1.0f,

	// Bottom face (y = -0.5)
	-0.5f,-0.5f,-0.5f,  1.0f, 1.0f,
	0.5f,-0.5f, 0.5f,  0.0f, 0.0f,
	-0.5f,-0.5f, 0.5f,  1.0f, 0.0f,

	-0.5f,-0.5f,-0.5f,  1.0f, 1.0f,
	0.5f,-0.5f,-0.5f,  0.0f, 1.0f,
	0.5f,-0.5f, 0.5f,  0.0f, 0.0f
	};

	// // Allows GPU to access the basic cube building block (uploads it)
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	// position (vertex)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)0);
	glEnableVertexAttribArray(0);

	// texture cordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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
	glUseProgram(this->_prog);
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

Mat4&	Application::top()
{
	if(this->_stack.empty())
	{
		throw std::runtime_error("Matrix stack is empty!");
	}
	return this->_stack.back();
}

const Mat4&	Application::top() const
{
	if(this->_stack.empty())
	{
		throw std::runtime_error("Matrix stack is empty!");
	}
	return this->_stack.back();
}

/// @brief Draws a 1x1x1 cube centered at the origin using the body part and the world matrix
/// @param bodyPart the body part, contains the shape, colour and texture
/// @param world the world matrix, contains the position and rotation of the body part in the world
void	Application::draw(const BodyPart& bodyPart, const Mat4& world)
{
	Mat4	proj = perspective(DEG2RADFOV, (float)this->_winWidth/(float)this->_winHeight, 0.1f, 100.f);
	Mat4	view = lookAt(
		Vec3{this->_cameraPosition.x, this->_cameraPosition.y, this->_cameraPosition.z},
		Vec3{this->_cameraPosition.x + this->_cameraFront.x, this->_cameraPosition.y + this->_cameraFront.y, this->_cameraPosition.z + this->_cameraFront.z},
		this->_cameraUp
	);
	Mat4	model = world * bodyPart.shape;
	Mat4	mvp = proj * view * model;

	glUseProgram(this->_prog);

	glUniformMatrix4fv(this->_mpvLoc, 1, GL_FALSE, mvp.m);
	glUniform3f(this->_colLoc, bodyPart.colour.r, bodyPart.colour.g, bodyPart.colour.b);

	bool	useTexture = (bodyPart.tex != TextureType::None);

	glUniform1i(this->_useTextureLoc, useTexture);
	glUniform1i(this->_texLoc, 0);

	glActiveTexture(GL_TEXTURE0);

	if (useTexture)
		glBindTexture(GL_TEXTURE_2D, this->_textures[bodyPart.tex]);
	else
		glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(this->_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

/**
 * @brief Setup all the keybinds that could be used continuously (like holding down W to move forward)
 */
void	Application::keybinds()
{
	float	speed = this->_moveSpeed * this->_dt; // Adjust speed based on frame time
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
	// std::cout << "Pitch: " << this->_pitch << " Yaw: " << this->_yaw << std::endl;
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
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
	{
		app->_moveSpeed *= 4.0f;
	}
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
	{
		app->_moveSpeed *= 0.25f;
	}

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

/**
 * @brief Loads a .ppm texture into opengl
 * @param path texture path
 * @param width returns the width
 * @param height returns the height
 * @return returns the texture
 */
GLuint	Application::loadPPM(const std::string& path, int& width, int&height)
{
	std::ifstream file(path, std::ios::binary);
	if (!file)
	{
		return 0;
	}
	std::string format;
	file >> format;
	if (format != "P6")
	{
		return 0;
	}

	file >> width >> height;
	int maxVal;
	file >> maxVal;
	file.ignore(256, '\n');

	int dataSize = width * height * 3;
	std::vector<unsigned char> data(dataSize);
	file.read(reinterpret_cast<char*>(data.data()), dataSize);

	if (!file)
	{
		return 0;
	}

	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
	glGenerateMipmap(GL_TEXTURE_2D);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return texID;
}

void	Application::initBody()
{
	/* TORSO */
	this->_body.torso.local = mat4_identity();
	this->_body.torso.size = Vec3{1.0f, 6.0f, 4.0f};
	this->_body.torso.jointPivot = Vec3{0.0f, 0.0f, 0.0f};
	this->_body.torso.colour = Colour(1.0f, 0.0f, 0.0f);
	this->_body.torso.tex = TextureType::Unicorn;

	/* HEAD */
	this->_body.head.size = Vec3{2.5f, 2.5f, 2.5f};
	this->_body.head.colour = Colour(0.0f, 1.0f, 0.0f);
	this->_body.head.tex = TextureType::Dirt;

	/* UPPER LEFT ARM */
	this->_body.upperLeftArm.size = Vec3{1.0f, 3.5f, 1.0f};
	this->_body.upperLeftArm.colour = Colour(1.0f, 0.0f, 0.0f);
	this->_body.upperLeftArm.tex = TextureType::None;

	/* UPPER RIGHT ARM */
	this->_body.upperRightArm.size = Vec3{1.0f, 3.5f, 1.0f};
	this->_body.upperRightArm.colour = Colour(1.0f, 0.0f, 0.0f);
	this->_body.upperRightArm.tex = TextureType::None;

	/* LOWER LEFT ARM */
	this->_body.lowerLeftArm.size = Vec3{1.0f, 3.0f, 1.0f};
	this->_body.lowerLeftArm.colour = Colour(1.0f, 0.4f, 0.0f);
	this->_body.lowerLeftArm.tex = TextureType::None;

	/* LOWER RIGHT ARM */
	this->_body.lowerRightArm.size = Vec3{1.0f, 3.0f, 1.0f};
	this->_body.lowerRightArm.colour = Colour(1.0f, 0.4f, 0.0f);
	this->_body.lowerRightArm.tex = TextureType::None;

	/* UPPER LEFT LEG */
	this->_body.upperLeftLeg.size = Vec3{1.0f, 3.5f, 2.0f};
	this->_body.upperLeftLeg.colour = Colour(0.0f, 0.0f, 1.0f);
	this->_body.upperLeftLeg.tex = TextureType::Dirt;

	/* UPPER RIGHT LEG */
	this->_body.upperRightLeg.size = Vec3{1.0f, 3.5f, 2.0f};
	this->_body.upperRightLeg.colour = Colour(0.0f, 0.0f, 1.0f);
	this->_body.upperRightLeg.tex = TextureType::Unicorn;

	/* LOWER LEFT LEG */
	this->_body.lowerLeftLeg.size = Vec3{1.0f, 3.5f, 2.0f};
	this->_body.lowerLeftLeg.colour = Colour(0.0f, 0.0f, 1.0f);
	this->_body.lowerLeftLeg.tex = TextureType::Unicorn;

	/* LOWER RIGHT LEG */
	this->_body.lowerRightLeg.size = Vec3{1.0f, 3.5f, 2.0f};
	this->_body.lowerRightLeg.colour = Colour(0.0f, 0.0f, 1.0f);
	this->_body.lowerRightLeg.tex = TextureType::Dirt;

	this->resizeBody();
}

void	Application::resizeBody()
{
	float halfX, halfY, halfZ;
	/* TORSO */
	this->_body.torso.shape = mat4_scale(this->_body.torso.size);

	/* HEAD */
	this->_body.head.shape = mat4_scale(this->_body.head.size);
	halfY = this->_body.head.size.y * 0.5f;
	this->_body.head.local = translate(0.0f, (this->_body.torso.size.y * 0.5f) + halfY, 0.0f);
	this->_body.head.jointPivot = Vec3{0.0f, -halfY, 0.0f};

	/* UPPER LEFT ARM */
	this->_body.upperLeftArm.shape = mat4_scale(this->_body.upperLeftArm.size);
	halfX = this->_body.upperLeftArm.size.x * 0.5f;
	halfY = this->_body.upperLeftArm.size.y * 0.5f;
	halfZ = this->_body.upperLeftArm.size.z * 0.5f;
	Vec3 torsoShoulderLeft = Vec3{
		halfX,
		this->_body.torso.size.y * 0.5f, 
		this->_body.torso.size.z * 0.5f + halfZ
	};
	this->_body.upperLeftArm.local = translate(torsoShoulderLeft) * translate(-halfX, -halfY, 0.0f);
	this->_body.upperLeftArm.jointPivot = Vec3{0.0f, halfY, 0.0f};

	/* UPPER RIGHT ARM */
	this->_body.upperRightArm.shape = mat4_scale(this->_body.upperRightArm.size);
	halfX = this->_body.upperRightArm.size.x * 0.5f;
	halfY = this->_body.upperRightArm.size.y * 0.5f;
	halfZ = this->_body.upperRightArm.size.z * 0.5f;
	Vec3 torsoShoulderRight = Vec3{
		halfX,
		this->_body.torso.size.y * 0.5f, 
		-(this->_body.torso.size.z * 0.5f + halfZ)
	};
	this->_body.upperRightArm.local = translate(torsoShoulderRight) * translate(-halfX, -halfY, 0.0f);
	this->_body.upperRightArm.jointPivot = Vec3{0.0f, halfY, 0.0f};

	/* LOWER LEFT ARM */
	this->_body.lowerLeftArm.shape = mat4_scale(this->_body.lowerLeftArm.size);
	halfX = this->_body.lowerLeftArm.size.x * 0.5f;
	halfY = this->_body.lowerLeftArm.size.y * 0.5f;
	halfZ = this->_body.lowerLeftArm.size.z * 0.5f;
	Vec3 elbowLeft = Vec3{
		halfX,
		-this->_body.upperLeftArm.size.y * 0.5f,
		0.0f
	};
	this->_body.lowerLeftArm.local = translate(elbowLeft) * translate(-halfX, -halfY, 0.0f);
	this->_body.lowerLeftArm.jointPivot = Vec3{0.0f, halfY, 0.0f};

	/* LOWER RIGHT ARM */
	this->_body.lowerRightArm.shape = mat4_scale(this->_body.lowerRightArm.size);
	halfX = this->_body.lowerRightArm.size.x * 0.5f;
	halfY = this->_body.lowerRightArm.size.y * 0.5f;
	halfZ = this->_body.lowerRightArm.size.z * 0.5f;
	Vec3 elbowRight = Vec3{
		halfX,
		-this->_body.upperRightArm.size.y * 0.5f,
		0.0f
	};
	this->_body.lowerRightArm.local = translate(elbowRight) * translate(-halfX, -halfY, 0.0f);
	this->_body.lowerRightArm.jointPivot = Vec3{0.0f, halfY, 0.0f};

	/* UPPER LEFT LEG */
	this->_body.upperLeftLeg.shape = mat4_scale(this->_body.upperLeftLeg.size);
	halfX = this->_body.upperLeftLeg.size.x * 0.5f;
	halfY = this->_body.upperLeftLeg.size.y * 0.5f;
	halfZ = this->_body.upperLeftLeg.size.z * 0.5f;
	Vec3 hipLeft = Vec3{
		halfX,
		-this->_body.torso.size.y * 0.5f,
		this->_body.torso.size.z * 0.5f - halfZ
	};
	this->_body.upperLeftLeg.local = translate(hipLeft) * translate(-halfX, -halfY, 0.0f);
	this->_body.upperLeftLeg.jointPivot = Vec3{0.0f, halfY, 0.0f};

	/* UPPER RIGHT LEG */
	this->_body.upperRightLeg.shape = mat4_scale(this->_body.upperRightLeg.size);
	halfX = this->_body.upperRightLeg.size.x * 0.5f;
	halfY = this->_body.upperRightLeg.size.y * 0.5f;
	halfZ = this->_body.upperRightLeg.size.z * 0.5f;
	Vec3 hipRight = Vec3{
		halfX,
		-this->_body.torso.size.y * 0.5f,
		-(this->_body.torso.size.z * 0.5f - halfZ)
	};
	this->_body.upperRightLeg.local = translate(hipRight) * translate(-halfX, -halfY, 0.0f);
	this->_body.upperRightLeg.jointPivot = Vec3{0.0f, halfY, 0.0f};

	/* LOWER LEFT LEG */
	this->_body.lowerLeftLeg.shape = mat4_scale(this->_body.lowerLeftLeg.size);
	halfX = this->_body.lowerLeftLeg.size.x * 0.5f;
	halfY = this->_body.lowerLeftLeg.size.y * 0.5f;
	halfZ = this->_body.lowerLeftLeg.size.z * 0.5f;
	Vec3 kneeLeft = Vec3{
		halfX,
		-this->_body.upperLeftLeg.size.y * 0.5f,
		0.0f
	};
	this->_body.lowerLeftLeg.local = translate(kneeLeft) * translate(-halfX, -halfY, 0.0f);
	this->_body.lowerLeftLeg.jointPivot = Vec3{0.0f, halfY, 0.0f};

	/* LOWER RIGHT LEG */
	this->_body.lowerRightLeg.shape = mat4_scale(this->_body.lowerRightLeg.size);
	halfX = this->_body.lowerRightLeg.size.x * 0.5f;
	halfY = this->_body.lowerRightLeg.size.y * 0.5f;
	halfZ = this->_body.lowerRightLeg.size.z * 0.5f;
	Vec3 kneeRight = Vec3{
		halfX,
		-this->_body.upperRightLeg.size.y * 0.5f,
		0.0f
	};
	this->_body.lowerRightLeg.local = translate(kneeRight) * translate(-halfX, -halfY, 0.0f);
	this->_body.lowerRightLeg.jointPivot = Vec3{0.0f, halfY, 0.0f};
}