#include "humangl.h"

int main()
{
	Application	app;

	if(!app._win) return (-1);

	glEnable(GL_DEPTH_TEST);

	float last = (float)glfwGetTime();
	float angle = 0.0f;
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


		// Moves the body on the x axis (shows attachment)
		// static float posX = 0.0f;
		// posX -= 0.5f * app._dt;
		// app._body.torso.base = translate(posX, 0.0f, 0.0f);

		// if (last >= 5.0f)
		// {
		// 	app._body.torso.shape = mat4_scale(Vec3{10.0f, 10.0f, 10.0f});
		// }
		angle += 4.0f * app._dt;
		if (angle >= 360.0f) angle -= 360.0f;

		static float posX = 0.0f;
		posX -= 0.5f * app._dt;
		app._body.torso.base = translate(posX, 0.0f, 0.0f) * translate(0.0f, 0.0f, 0.0f) * rotate_y(angle * (3.14159265f / 180.0f));
		Mat4 torsoMVP = app.calcMVP(app._body.torso);
		app.drawCube(torsoMVP, app._body.torso.colour, app._body.torso.tex);

		float torsoHalf = app._body.torso.shape.m[5] * 0.5f;
		float headHalf  = app._body.head.shape.m[5]  * 0.5f;
		app._body.head.base = app._body.torso.base * translate(0.0f, torsoHalf + headHalf, 0.0f) * nod(now, 2.0f);
		Mat4 headMVP = app.calcMVP(app._body.head);
		app.drawCube(headMVP, app._body.head.colour, app._body.head.tex);
		// Display rendered stuff
		glfwSwapBuffers(app._win);
		// Mouse and keyboard events
		glfwPollEvents();
	}
	return (0);
}
