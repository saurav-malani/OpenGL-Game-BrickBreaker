
#include "config.hpp"

#include "wm.hpp"
#include "gm.hpp"

#ifndef GAME_CONTROL
#define GAME_CONTROL

class GameControl {

public:
	static bool ctrlKey, altKey;
	GLint selectedBucket;
	GLfloat panFactor;
	bool screenSelected;
	vec2 origMousePositon;
	GLint finalScore;
	Renderer *r;

	GameControl();
	static void keyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	void initSetup();
	void renderAll();
	void handleCollisions();
	bool checkInBucket(vector<vec2> &cube, bool isMouseCollider);
	bool checkCollision(vector<vec2> &objVerts, vec2 ballPosition, GLfloat radius, bool allSides);
	bool checkCollisionFaceSide(vector<vec2> &objVerts, vec2 ballPosition, GLfloat radius);
	bool checkCollisionOtherSides(vector<vec2> &objVerts, vec2 ballPosition, GLfloat radius);

};

#endif
