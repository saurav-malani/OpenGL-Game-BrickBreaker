
#include "config.hpp"

#include "globalVars.hpp"

bool GameControl::ctrlKey = false;
bool GameControl::altKey = false;

GameControl::GameControl()
{
	this->r = new Renderer();
	this->selectedBucket = -1;
	this->finalScore = 0;

	this->panFactor = 0.07f;
	this->origMousePositon = vec2(0.0f, 0.0f);
	this->screenSelected = false;
}

void GameControl::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	GLfloat final_xpos = xpos / 100.0f;
	GLfloat final_ypos = 7.68 - ypos / 100.0f;
	GLfloat screen_xpos = (final_xpos - 10.24/2) * 2 / 10.24;
	GLfloat screen_ypos = (final_ypos - 7.68/2) * 2 / 7.68;

	vec3 mouseVector = vec3(final_xpos, final_ypos, -5.0f);
	vec3 mouseVectorScreen = vec3(screen_xpos, screen_ypos, -5.0f);

	if(button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if(action == GLFW_PRESS)
		{
			gc->screenSelected = true;
			gc->origMousePositon = vec2(mouseVector);
		}
		else
		{
			gc->screenSelected = false;
		}
	}
	else if(button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if(action == GLFW_RELEASE)
		{
			gc->selectedBucket = -1;
			gc->screenSelected = false;
		}
		else if(action == GLFW_PRESS)
		{
			vector<vec2> mouseVectorVec;
			mouseVectorVec.push_back(vec2(mouseVectorScreen));
			mouseVectorVec.push_back(vec2(mouseVectorScreen));
			mouseVectorVec.push_back(vec2(mouseVectorScreen));
			mouseVectorVec.push_back(vec2(mouseVectorScreen));

			bool check = !gc->checkInBucket(mouseVectorVec, true);
			if(gc->selectedBucket == -1 && check)
			{
				vec3 position = gc->r->gameObjs->canon.position;
				vec3 direction = gc->r->gameObjs->canon.direction;

				vec3 newDirection = vec3(mouseVector) - position;
				GLfloat angle = atan2(newDirection.y, newDirection.x);

				gc->r->gameObjs->canon.direction = normalize(newDirection);
				gc->r->gameObjs->canon.rotation = angle * 3.1415925f / 180.0f;

				gc->r->gameObjs->initBall();
			}
		}
	}
	else if(button == GLFW_MOUSE_BUTTON_MIDDLE)
	{
		gc->r->gameObjs->resetProjectionAndView();
		gc->panFactor = 0.07f;
	}
}

void GameControl::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	GLfloat left, right, down, up;
	left = gc->r->gameObjs->left;
	right = gc->r->gameObjs->right;
	down = gc->r->gameObjs->down;
	up = gc->r->gameObjs->up;

	if(yoffset >=0)
	{
		GLfloat divFac = yoffset*5;
		gc->panFactor += 0.1f;

		left /= divFac;
		right /= divFac;
		down /= divFac;
		up /= divFac;
	}
	else
	{
		GLfloat mulFac = yoffset*5;
		gc->panFactor -= 0.1f;

		left *= -mulFac;
		right *= -mulFac;
		down *= -mulFac;
		up *= -mulFac;
	}

	gc->r->gameObjs->setProjection(left, right, down, up);
}

void GameControl::mouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
	GLfloat final_xpos = xpos / 100.0f;
	GLfloat final_ypos = 7.68f - ypos / 100.0f;
	vec2 mouseVector = vec2(final_xpos, final_ypos);

	if(gc->selectedBucket != -1)
	{
		gc->r->gameObjs->buckets[gc->selectedBucket].position.x = final_xpos;
	}
	else if(gc->screenSelected)
	{
		vec2 temp = mouseVector - gc->origMousePositon;
		vec3 final_vector = vec3(temp, 0.0f);

		mat4 viewMatrix;
		viewMatrix = gc->r->gameObjs->viewMatrix;
		viewMatrix = translate(viewMatrix, gc->panFactor * normalize(final_vector));

		gc->origMousePositon = mouseVector;
		gc->r->gameObjs->viewMatrix = viewMatrix;
	}
}

void GameControl::keyPressCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  	wm->close();
	else if(key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		gc->r->gameObjs->rotateCanon(true);
	}
	else if(key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		gc->r->gameObjs->rotateCanon(false);
	}
	else if(key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		gc->r->gameObjs->moveCanon(true);
	}
	else if(key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		gc->r->gameObjs->moveCanon(false);
	}
	else if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		gc->r->gameObjs->initBall();
	}
	else if(key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS)
	{
		ctrlKey = true;
	}
	else if(key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE)
	{
		ctrlKey = false;
	}
	else if(key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS)
	{
		altKey = true;
	}
	else if(key == GLFW_KEY_LEFT_ALT && action == GLFW_RELEASE)
	{
		altKey = false;
	}
	else if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		if(altKey)
			gc->r->gameObjs->moveRightBucket(true);
		else if(ctrlKey)
			gc->r->gameObjs->moveLeftBucket(true);
	}
	else if(key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		if(altKey)
			gc->r->gameObjs->moveRightBucket(false);
		else if(ctrlKey)
			gc->r->gameObjs->moveLeftBucket(false);
	}
}

void GameControl::initSetup()
{
	this->r->initShaders();
	this->r->initGameObjects();
}

void GameControl::renderAll()
{
	static GLfloat oldTime = 0.0f;
	GLfloat newTime = glfwGetTime();

	if(newTime - oldTime >= 2.0f)
	{
		GLfloat xPos = ((GLfloat)rand()) / RAND_MAX;
		xPos = (xPos == 0) ? 0.2f : xPos;
		this->r->gameObjs->initCube(vec3(10.0f * xPos, 7.68f, -6.0f));
		oldTime = newTime;
	}

	this->r->render();
}

void GameControl::handleCollisions()
{
	for(list<Ball>::iterator it=r->gameObjs->balls.begin();it!=r->gameObjs->balls.end();it++)
	{
		vec4 ballPosition = vec4(it->position, 1.0f);
		vec3 ballDirection = it->direction;

		for(list<Mirror>::iterator it2=r->gameObjs->mirrors.begin();it2!=r->gameObjs->mirrors.end();it2++)
		{
			vec4 mirrorPosition = vec4(it2->position, 1.0f);
			vec3 mirrorDirection = it2->direction;

			vector<vec2> mirrorVerts;
			for(int j=0;j<24;j+=6)
			{
				vec4 vert = vec4(it2->mirrorVerts[j], it2->mirrorVerts[j+1], it2->mirrorVerts[j+2], 1.0f);
				vert = it2->modelMatrix * vert;
				mirrorVerts.push_back(vec2(vert));
			}

			if(this->checkCollision(mirrorVerts, vec2(ballPosition), it->diameter/2.0f, false))
			{
				it->direction = reflect(ballDirection, normalize(mirrorDirection));
				it->position = it->position + it->direction;
			}
		}

		for(list<Cube>::iterator it2=r->gameObjs->cubes.begin();it2!=r->gameObjs->cubes.end();it2++)
		{
			vec4 cubePosition = vec4(it2->position, 1.0f);

			vector<vec2> cubeVerts;
			for(int j=0;j<24;j+=6)
			{
				vec4 vert = vec4(it2->cubeVerts[j], it2->cubeVerts[j+1], it2->cubeVerts[j+2], 1.0f);
				vert = it2->modelMatrix * vert;
				cubeVerts.push_back(vec2(vert));
			}

			if(this->checkCollision(cubeVerts, vec2(ballPosition), it->diameter/2.0f, true))
			{
				if(it2->type == 2)
					this->finalScore += 50;
				else
					this->finalScore -= 10;

				cout << "Game Score : " << this->finalScore << endl;
				r->gameObjs->cubes.erase(it2--);
			}
		}
	}

	for(list<Cube>::iterator it2=r->gameObjs->cubes.begin();it2!=r->gameObjs->cubes.end();it2++)
	{
		vec4 cubePosition = vec4(it2->position, 1.0f);

		vector<vec2> cubeVerts;
		for(int j=0;j<24;j+=6)
		{
			vec4 vert = vec4(it2->cubeVerts[j], it2->cubeVerts[j+1], it2->cubeVerts[j+2], 1.0f);
			vert = it2->modelMatrix * vert;
			cubeVerts.push_back(vec2(vert));
		}

		if(this->checkInBucket(cubeVerts, false))
		{
			if(it2->type == 2)
				exit(0);

			r->gameObjs->cubes.erase(it2--);
		}
	}
}

bool GameControl::checkInBucket(vector<vec2> &cube, bool isMouseCollider)
{
	GLint index = 0;

	for(vector<Bucket>::iterator it2=r->gameObjs->buckets.begin();it2!=r->gameObjs->buckets.end();it2++)
	{
		vector<vec2> bucketVerts;
		for(int j=0;j<24;j+=6)
		{
			vec4 vert = vec4(it2->bucketVerts[j], it2->bucketVerts[j+1], it2->bucketVerts[j+2], 1.0f);
			vert = (isMouseCollider) ? (r->gameObjs->projectionMatrix * r->gameObjs->viewMatrix * it2->modelMatrix *  vert) : (it2->modelMatrix *  vert);

			bucketVerts.push_back(vec2(vert));
		}

		if(cube[0].x <= bucketVerts[1].x && cube[1].x >= bucketVerts[0].x && cube[0].y >= bucketVerts[3].y && cube[3].y <= bucketVerts[0].y)
		{
			if(isMouseCollider)
				this->selectedBucket = index;

			return true;
		}
		index++;
	}

	return false;
}

bool GameControl::checkCollision(vector<vec2> &objVerts, vec2 ballPosition, GLfloat radius, bool allSides)
{
	bool cond1, cond2;
	if(allSides)
	{
		cond1 = checkCollisionFaceSide(objVerts, ballPosition, radius);
		cond2 = checkCollisionOtherSides(objVerts, ballPosition, radius);
	}
	else
	{
		cond1 = false;
		cond2 = checkCollisionFaceSide(objVerts, ballPosition, radius);
	}

	return cond1 || cond2;
}

bool GameControl::checkCollisionFaceSide(vector<vec2> &objVerts, vec2 ballPosition, GLfloat radius)
{
	vec2 top = objVerts[0];
	vec2 bottom = objVerts[3];

	vec2 um = top - bottom;
	vec2 tbDown = ballPosition - bottom;
	vec2 dm = bottom - top;
	vec2 tbUp = ballPosition - top;

	GLfloat d = length(cross(vec3(dm, 0.0f), vec3(tbUp, 0.0f)))/length(dm);
	GLfloat upperAngle = 180.0f/3.1415925f * acos(dot(tbUp, dm)/(length(tbUp)*length(dm)));
	GLfloat lowerAngle = 180.0f/3.1415925f * acos(dot(tbDown, um)/((length(tbDown)*length(um))));

	bool cond = (upperAngle <= 90.0f && lowerAngle <= 90.0f && d <= radius);
	return cond;
}

bool GameControl::checkCollisionOtherSides(vector<vec2> &objVerts, vec2 ballPosition, GLfloat radius)
{
	bool faceSide = this->checkCollisionFaceSide(objVerts, ballPosition, radius);

	bool retVal = false;
	for(int i=1;i<=3;i++)
	{
		vec2 top = objVerts[i%4];
		vec2 bottom = objVerts[(i+1)%4];

		vec2 um = top - bottom;
		vec2 tbDown = ballPosition - bottom;
		vec2 dm = bottom - top;
		vec2 tbUp = ballPosition - top;

		GLfloat d = length(cross(vec3(dm, 0.0f), vec3(tbUp, 0.0f)))/length(dm);
		GLfloat upperAngle = 180.0f/3.1415925f * acos(dot(tbUp, dm)/(length(tbUp)*length(dm)));
		GLfloat lowerAngle = 180.0f/3.1415925f * acos(dot(tbDown, um)/((length(tbDown)*length(um))));

		bool cond = (upperAngle <= 90.0f && lowerAngle <= 90.0f && d <= radius);
		retVal = retVal || cond;
	}

	return retVal || faceSide;
}
