#include "Application.hpp"

Application::Application()
{
	initWindow();
	if(this->_win == nullptr) return;
	// int w, h;
	// this->_textureID = loadPPM("assets/unicorn.ppm", w, h);
	// if (this->_textureID == 0)
	// 	std::cerr << "Loading texture failed" << std::endl;
	// setupImGui();
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

	GLFWwindow*	win = glfwCreateWindow(1280, 720, "HumanGL", nullptr, nullptr);
	if(!win)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(win);

	// 	if(!loadGLFunctions())
	// {
	// 	std::cerr<<"Failed loading GL functions\n";
	// 	return;
	// }
	this->_win = win;
}

int	Application::setupBuffers()
{
	const char* vs_src;
	const char* fs_src;

			vs_src = R"(#version 330 core
		layout(location=0) in vec3 aPos;
		layout(location=1) in vec3 aColor;
		uniform mat4 uMVP;
		out vec3 vColor;
		void main(){ vColor = aColor; gl_Position = uMVP * vec4(aPos,1.0); }
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

	// // Allows GPU to access Vertex data (uploads it)
	// glBufferData(GL_ARRAY_BUFFER, (ptrdiff_t)(this->_verts.size()*sizeof(Vertex)), this->_verts.data(), GL_STATIC_DRAW);

	// // First 3 floats are coordinates
	// glEnableVertexAttribArray(0);
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)0);

	// // Second 3 floats are colors
	// glEnableVertexAttribArray(1);
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(3*sizeof(float)));

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