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

		// Render your scene here
		Mat4 proj = perspective(45.0f * 3.14159265f / 180.0f, (float)w/(float)h, 0.1f, 100.f);
		Mat4 view = lookAt( Vec3{0,0.6f,0}, Vec3{0,0, -1}, Vec3{0,1,0} );
		Mat4 model = mat4_mul( translate(app._tx, app._ty, app._tz), rotate_y(0.0f) );
		Mat4 MVP = mat4_mul( mat4_mul(proj, view), model);

		app.drawCube(MVP, Colour(1.0f, 0.0f, 0.0f));
		// Display rendered stuff
		glfwSwapBuffers(app._win);
		// Mouse and keyboard events
		glfwPollEvents();
	}
	return (0);
}
