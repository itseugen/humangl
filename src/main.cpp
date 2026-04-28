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

		static float posX = 0.0f;
		posX -= 0.5f * app._dt;

		// If I want to rotate stuff
		angle += 4.0f * app._dt;
		if (angle >= 360.0f) angle -= 360.0f;
		app.push(mat4_identity());
		app.push(app.top() * translate(posX, 0.0f, 0.0f) * app._body.torso.local); // Not necessary to push the identity but it shows how the stack works
		app.draw(app._body.torso, app.top());

		Mat4 headNod = applyJointRoation(app._body.head, nod(now, 1.5f));
		app.push(app.top() * app._body.head.local * headNod);
		app.draw(app._body.head, app.top());
		app.pop(); // head

		Mat4 armNod = applyJointRoation(app._body.upperLeftArm, nod(now -0.5f, 0.5f));
		app.push(app.top() * app._body.upperLeftArm.local * armNod);
		app.draw(app._body.upperLeftArm, app.top());

		app.push(app.top() * app._body.lowerLeftArm.local);
		app.draw(app._body.lowerLeftArm, app.top());
		app.pop(); // lower left arm
		app.pop(); // upper left arm

		armNod = applyJointRoation(app._body.upperRightArm, nod(now -0.75f, 0.5f));
		app.push(app.top() * app._body.upperRightArm.local * armNod);
		app.draw(app._body.upperRightArm, app.top());

		armNod = applyJointRoation(app._body.lowerRightArm, nod(now -1.25f, 0.75f));
		app.push(app.top() * app._body.lowerRightArm.local * armNod);
		app.draw(app._body.lowerRightArm, app.top());
		app.pop(); // lower right arm
		app.pop(); // upper right arm

		Mat4 legNod = applyJointRoation(app._body.upperLeftLeg, nod(now -1.0f, 0.5f));
		app.push(app.top() * app._body.upperLeftLeg.local * legNod);
		app.draw(app._body.upperLeftLeg, app.top());
		app.pop(); // upper left leg

		legNod = applyJointRoation(app._body.upperRightLeg, nod(now -1.5f, 0.5f));
		app.push(app.top() * app._body.upperRightLeg.local * legNod);
		app.draw(app._body.upperRightLeg, app.top());
		app.pop(); // upper right leg
		app.pop(); // torso
		app.pop(); // identity

		// Display rendered stuff
		glfwSwapBuffers(app._win);
		// Mouse and keyboard events
		glfwPollEvents();
	}
	return (0);
}
