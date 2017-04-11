
#include "config.hpp"

#ifndef WINDOW_MANAGER
#define WINDOW_MANAGER

class WindowManager {

public:
	GLFWwindow *window;
	string name;
	int width;
	int height;

	void setWindowOptions();
	WindowManager(string windowName, int width, int height);
	void setKeyCallback(GLFWkeyfun cbfunc);
	void setMouseCallback(GLFWcursorposfun keyboardCallbackFunc, GLFWmousebuttonfun mouseCallbackFunc, GLFWscrollfun scrollCallbackFunc);

	void setCurrentContext();
	void close();
	GLFWwindow* getCurrentWindow();
	bool checkWindowClosed();
	void pollEvents();
	void swapBuffers();

};

#endif
