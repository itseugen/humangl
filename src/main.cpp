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
		app._dt = now - last;
		last = now;

		app.keybinds();

		// Actual window size
		int w,h; glfwGetFramebufferSize(app._win, &w, &h);
		glViewport(0,0,w,h);
		// Background colour
		glClearColor(0.12f,0.12f,0.14f,1.0f);
		// Cloar buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Mat4 MVP = app.calcMVP(app._body.torso);

		// app.drawCube(MVP, Colour(1.0f, 0.0f, 0.0f), TextureType::None);
		app.drawCube(MVP, app._body.torso.colour, app._body.torso.tex);

		Mat4	torsoWorld = app._body.torso.local;
		Mat4	headWorld = mat4_mul (torsoWorld, translate(0.0f, 4.0f, 0.0f));
		app._body.head.local = headWorld;

		MVP = app.calcMVP(app._body.head);
		app.drawCube(MVP, app._body.head.colour, app._body.head.tex);
		// Display rendered stuff
		glfwSwapBuffers(app._win);
		// Mouse and keyboard events
		glfwPollEvents();
	}
	return (0);
}
