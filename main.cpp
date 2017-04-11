
#include "config.hpp"

#include "globalVars.hpp"

WindowManager *wm;
GameControl *gc;

int main()
{
	wm = new WindowManager("Game", 1024, 768);
	wm->setCurrentContext();
	wm->setKeyCallback(GameControl::keyPressCallback);
	wm->setMouseCallback(GameControl::mouseMoveCallback, GameControl::mouseButtonCallback, GameControl::scrollCallback);

	/* initialise glew */
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
		return -1;

	/* initialise game control */
	gc = new GameControl();
	gc->initSetup();
	glEnable(GL_DEPTH_TEST);

	while(!wm->checkWindowClosed())
	{
		wm->pollEvents();

		/* replace */
		glClearColor(217.0f/255.0f, 217.0f/255.0f, 217.0f/255.0f, 1.0f);
  	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		/* replace */

		gc->renderAll();
		gc->handleCollisions();

		wm->swapBuffers();
	}
}
