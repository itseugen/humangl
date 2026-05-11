#include "humangl.h"
#include "Application.hpp"
#include "Animation.hpp"

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

		switch(app._animation)
		{
			case FullBodyAnimation::None:
				app._animationController.clearAnimation(app._body);
				break;
			case FullBodyAnimation::Walk:
				app._animationController.walk(app._body, now, app._dt);
				break;
			case FullBodyAnimation::Jump:
				app._animationController.jump(app._body, now, app._dt);
				if (app._animationController.getLanded() == true)
				{
					app._animationController.clearAnimation(app._body);
					app._animation = FullBodyAnimation::None;
				}
				break;
			default:
				app._animationController.clearAnimation(app._body);
				break;
		}
		app.push(mat4_identity()); // Not necessary to push the identity but it shows how the stack works
		app.push(app.top() * app._body.torso.local * app._body.torso.animation);
		app.draw(app._body.torso, app.top());

		app.push(app.top() * app._body.head.local * app._body.head.animation);
		app.draw(app._body.head, app.top());
		app.pop(); // head

		app.push(app.top() * app._body.upperLeftArm.local * app._body.upperLeftArm.animation);
		app.draw(app._body.upperLeftArm, app.top());

		app.push(app.top() * app._body.lowerLeftArm.local * app._body.lowerLeftArm.animation);
		app.draw(app._body.lowerLeftArm, app.top());
		app.pop(); // lower left arm
		app.pop(); // upper left arm

		app.push(app.top() * app._body.upperRightArm.local * app._body.upperRightArm.animation);
		app.draw(app._body.upperRightArm, app.top());

		app.push(app.top() * app._body.lowerRightArm.local * app._body.lowerRightArm.animation);
		app.draw(app._body.lowerRightArm, app.top());
		app.pop(); // lower right arm
		app.pop(); // upper right arm

		app.push(app.top() * app._body.upperLeftLeg.local * app._body.upperLeftLeg.animation);
		app.draw(app._body.upperLeftLeg, app.top());

		app.push(app.top() * app._body.lowerLeftLeg.local * app._body.lowerLeftLeg.animation);
		app.draw(app._body.lowerLeftLeg, app.top());
		app.pop(); // lower left leg
		app.pop(); // upper left leg

		app.push(app.top() * app._body.upperRightLeg.local * app._body.upperRightLeg.animation);
		app.draw(app._body.upperRightLeg, app.top());

		app.push(app.top() * app._body.lowerRightLeg.local * app._body.lowerRightLeg.animation);
		app.draw(app._body.lowerRightLeg, app.top());
		app.pop(); // lower right leg
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
