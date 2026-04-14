#include "humangl.h"

int main()
{
	Application	app;

	if(!app._win) return (-1);

	glEnable(GL_DEPTH_TEST);

	float last = (float)glfwGetTime();

	while(!glfwWindowShouldClose(app._win))
	{
		float now = (float)glfwGetTime();
		// Time since last frame
		// app._dt = now - last;
		last = now;

		// Actual window size
		int w,h; glfwGetFramebufferSize(app._win, &w, &h);
		glViewport(0,0,w,h);
		// Background colour
		glClearColor(0.12f,0.12f,0.14f,1.0f);
		// Cloar buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render your scene here

		// Display rendered stuff
		glfwSwapBuffers(app._win);
		// Mouse and keyboard events
		glfwPollEvents();
	}
	return (0);
}
