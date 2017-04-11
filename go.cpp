
#include "config.hpp"

#include "globalVars.hpp"

GameObjects::GameObjects()
{
  left = 0.0f;
  right = 10.24f;
  down = 0.0f;
  up = 7.68f;

  projectionMatrix = ortho(left, right, down, up, 0.1f, 100.0f);
}

void GameObjects::setProjection(GLfloat left, GLfloat right, GLfloat down, GLfloat up)
{
  this->left = left;
  this->right = right;
  this->down = down;
  this->up = up;

  projectionMatrix = ortho(left, right, down, up, 0.1f, 100.f);
}

void GameObjects::resetProjectionAndView()
{
  left = 0.0f;
  right = 10.24f;
  down = 0.0f;
  up = 7.68f;

  projectionMatrix = ortho(left, right, down, up, 0.1f, 100.0f);

  mat4 newView;
  viewMatrix = newView;
}

void GameObjects::initBucket(vec3 position, string type)
{
  Bucket newBucket {0, 0, 0, 0};
  this->bindVertexArrays(1, newBucket.bucketObj);
  newBucket.programId = canon.programId;

  newBucket.type = this->getBucketAttributes(newBucket.bucketVerts, newBucket.bucketIndices, type);
  newBucket.position = position;

  this->bindBuffers(GL_ARRAY_BUFFER, 1, newBucket.bucketBuffer, newBucket.bucketVerts.size()*sizeof(GLfloat), newBucket.bucketVerts.data());
  this->bindBuffers(GL_ELEMENT_ARRAY_BUFFER, 1, newBucket.bucketElementBuffer, newBucket.bucketIndices.size()*sizeof(GLuint), newBucket.bucketIndices.data());

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  this->unbindAll();
  this->buckets.push_back(newBucket);
}

void GameObjects::renderBuckets()
{
  for(vector<Bucket>::iterator it=buckets.begin(); it!=buckets.end();it++)
  {
    glBindVertexArray(it->bucketObj);
    glUseProgram(it->programId);

    mat4 modelMatrix;
    modelMatrix = translate(modelMatrix, it->position);
    modelMatrix = scale(modelMatrix, vec3(0.4f, 0.4f, 0.4f));
    it->modelMatrix = modelMatrix;
    this->mvp(it->programId, modelMatrix);

    glDrawElements(GL_TRIANGLES, it->bucketIndices.size(), GL_UNSIGNED_INT, (void*)0);
    this->unbindAll();
  }
}

void GameObjects::moveRightBucket(bool right)
{
  int mul = right ? 1 : -1;
  buckets[1].position = buckets[1].position + vec3(0.2f*mul, 0.0f, 0.0f);
}

void GameObjects::moveLeftBucket(bool right)
{
  int mul = right ? 1 : -1;
  buckets[0].position = buckets[0].position + vec3(0.2f*mul, 0.0f, 0.0f);
}

void GameObjects::initCube(vec3 position)
{
  Cube newCube {0, 0, 0, 0};
  this->bindVertexArrays(1, newCube.cubeObj);
  newCube.programId = canon.programId;

  newCube.type = this->getCubeAttributes(newCube.cubeVerts, newCube.cubeIndices);
  newCube.position = position;
  newCube.direction = vec3(0.0f, -0.005f, 0.0f);

  this->bindBuffers(GL_ARRAY_BUFFER, 1, newCube.cubeBuffer, newCube.cubeVerts.size()*sizeof(GLfloat), newCube.cubeVerts.data());
  this->bindBuffers(GL_ELEMENT_ARRAY_BUFFER, 1, newCube.cubeElementBuffer, newCube.cubeIndices.size()*sizeof(GLuint), newCube.cubeIndices.data());

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  this->unbindAll();
  this->cubes.push_back(newCube);
}

void GameObjects::renderCubes()
{
  for(list<Cube>::iterator it=cubes.begin(); it!=cubes.end();it++)
  {
    glBindVertexArray(it->cubeObj);
    glUseProgram(it->programId);

    it->position = it->position + it->direction;
    mat4 modelMatrix;
    modelMatrix = translate(modelMatrix, it->position);
    modelMatrix = scale(modelMatrix, vec3(0.1f, 0.1f, 0.1f));
    it->modelMatrix = modelMatrix;
    this->mvp(it->programId, modelMatrix);

    glDrawElements(GL_TRIANGLES, it->cubeIndices.size(), GL_UNSIGNED_INT, (void*)0);
    this->unbindAll();
  }
}

void GameObjects::initMirror(vec3 position, vec3 rotation, vec3 normal)
{
  Mirror newMirror {0, 0, 0};
  this->bindVertexArrays(1, newMirror.mirrorObj);
  newMirror.programId = canon.programId;

  this->getMirrorAttributes(newMirror.mirrorVerts, newMirror.mirrorIndices);
  newMirror.position = position;

  GLfloat angle = acos(dot(normal, rotation)/(length(normal) * length(rotation)));
  newMirror.direction = rotation;
  GLint mul = (rotation.x < 0 && rotation.y > 0) ? -1 : 1;
  newMirror.rotation = 180.0f / 3.1415925f * angle * mul;

  this->bindBuffers(GL_ARRAY_BUFFER, 1, newMirror.mirrorBuffer, newMirror.mirrorVerts.size()*sizeof(GLfloat), newMirror.mirrorVerts.data());
  this->bindBuffers(GL_ELEMENT_ARRAY_BUFFER, 1, newMirror.mirrorElementBuffer, newMirror.mirrorIndices.size()*sizeof(GLuint), newMirror.mirrorIndices.data());

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  this->unbindAll();
  this->mirrors.push_back(newMirror);
}

void GameObjects::renderMirrors()
{
  for(list<Mirror>::iterator it=mirrors.begin(); it!=mirrors.end();it++)
  {
    glBindVertexArray(it->mirrorObj);
    glUseProgram(it->programId);

    mat4 modelMatrix;
    modelMatrix = translate(modelMatrix, it->position);
    modelMatrix = rotate(modelMatrix, it->rotation, vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = scale(modelMatrix, vec3(1.0f, 0.7f, 1.5f));
    it->modelMatrix = modelMatrix;
    this->mvp(it->programId, modelMatrix);

    glDrawElements(GL_TRIANGLES, it->mirrorIndices.size(), GL_UNSIGNED_INT, (void*)0);
    this->unbindAll();
  }
}

void GameObjects::initBall()
{
  static GLfloat oldTime = 0.0f;
  GLfloat newTime = glfwGetTime();

  if(newTime - oldTime >= 1.0f)
  {
    Ball newBall {0, 0, 0, 0};
    newBall.diameter = 0.05f;
    newBall.speed = 0.05f;
    this->bindVertexArrays(1, newBall.ballObj);
    newBall.programId = canon.programId;

    this->getBallAttributes(newBall.ballVerts, newBall.ballIndices);
    newBall.direction = newBall.speed * canon.direction;
    newBall.position = canon.position;

    this->bindBuffers(GL_ARRAY_BUFFER, 1, newBall.ballBuffer, newBall.ballVerts.size()*sizeof(GLfloat), newBall.ballVerts.data());
    this->bindBuffers(GL_ELEMENT_ARRAY_BUFFER, 1, newBall.ballElementBuffer, newBall.ballIndices.size()*sizeof(GLuint), newBall.ballIndices.data());

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);
    this->unbindAll();
    this->balls.push_back(newBall);

    oldTime = newTime;
  }
}

void GameObjects::renderBalls()
{
  for(list<Ball>::iterator it=balls.begin(); it!=balls.end();it++)
  {
    glBindVertexArray(it->ballObj);
    glUseProgram(it->programId);

    it->position = it->position + it->direction;
    mat4 modelMatrix;
    modelMatrix = translate(modelMatrix, it->position);
    modelMatrix = scale(modelMatrix, vec3(it->diameter, it->diameter, it->diameter));
    it->modelMatrix = modelMatrix;
    this->mvp(it->programId, modelMatrix);

    glDrawElements(GL_TRIANGLES, it->ballIndices.size(), GL_UNSIGNED_INT, (void*)0);
    this->unbindAll();
  }
}

void GameObjects::initCanon()
{
  this->bindVertexArrays(1, canon.canonObj);

  this->getCanonAttributes(canon.canonVerts, canon.canonIndices);
  canon.direction = vec3(1.0f, 0.0f, 0.0f);
  canon.position = vec3(0.0f, 3.84f, -5.0f);

  this->bindBuffers(GL_ARRAY_BUFFER, 1, canon.canonBuffer, canon.canonVerts.size()*sizeof(GLfloat), canon.canonVerts.data());
  this->bindBuffers(GL_ELEMENT_ARRAY_BUFFER, 1, canon.canonElementBuffer, canon.canonIndices.size()*sizeof(GLuint), canon.canonIndices.data());

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat),(GLvoid*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  this->unbindAll();
}

void GameObjects::moveCanon(bool isUp)
{
  int mul = isUp ? 1 : -1;
  canon.position = canon.position + vec3(0.0f, 0.2f*mul, 0.0f);
}

void GameObjects::rotateCanon(bool isUp)
{
  int mul = isUp ? 1 : -1;
  canon.rotation += 5.0f * mul;
  canon.direction = rotate(canon.direction, 5*mul*3.1415925f/180.0f, vec3(0.0f, 0.0f, 1.0f));
}

void GameObjects::renderCanon()
{
  glBindVertexArray(canon.canonObj);
  glUseProgram(canon.programId);

  GLfloat angle = atan2(canon.direction.y, canon.direction.x);

  mat4 modelMatrix;
  modelMatrix = translate(modelMatrix, canon.position);
  modelMatrix = rotate(modelMatrix, angle, vec3(0.0f, 0.0f, 1.0f));
  modelMatrix = scale(modelMatrix, vec3(0.4f, 0.4f, 0.4f));
  this->mvp(canon.programId, modelMatrix);

  glDrawElements(GL_TRIANGLES, canon.canonIndices.size(), GL_UNSIGNED_INT, (void*)0);
  this->unbindAll();
}

void GameObjects::mvp(GLint programId, mat4 modelMatrix)
{
  GLint modelLoc = glGetUniformLocation(programId, "model");
  GLint viewLoc = glGetUniformLocation(programId, "view");
  GLint projLoc = glGetUniformLocation(programId, "projection");

  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(modelMatrix));
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(viewMatrix));
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projectionMatrix));
}

void GameObjects::bindVertexArrays(GLuint number, GLuint &id)
{
  glGenVertexArrays(number, &(id));
  glBindVertexArray(id);
}

void GameObjects::bindBuffers(GLenum target, GLuint number, GLuint &id, GLsizeiptr size, const GLvoid *data)
{
  glGenBuffers(number, &(id));
  glBindBuffer(target, id);
  glBufferData(target, size, data, GL_STATIC_DRAW);
}

void GameObjects::unbindAll()
{
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLint GameObjects::getBucketAttributes(vector<GLfloat> &vertices, vector<GLuint> &indices, string tp)
{
  GLint type;
  GLfloat red = (tp == "red") ? 255.0f : 0.0f;
  GLfloat green = (tp == "green") ? 255.0f : 0.0f;
  GLfloat blue = 0.0f;

  type = (red == 255.0f) ? 0 : 1;

  vertices.push_back(-1.5f);
  vertices.push_back(1.0f);
  vertices.push_back(0.0f);
  vertices.push_back(red);
  vertices.push_back(green);
  vertices.push_back(blue);

  vertices.push_back(1.5f);
  vertices.push_back(1.0f);
  vertices.push_back(1.0f);
  vertices.push_back(red);
  vertices.push_back(green);
  vertices.push_back(blue);

  vertices.push_back(1.5f);
  vertices.push_back(-1.0f);
  vertices.push_back(1.0f);
  vertices.push_back(red);
  vertices.push_back(green);
  vertices.push_back(blue);

  vertices.push_back(-1.5f);
  vertices.push_back(-1.0f);
  vertices.push_back(1.0f);
  vertices.push_back(red);
  vertices.push_back(green);
  vertices.push_back(blue);

  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(2);

  indices.push_back(0);
  indices.push_back(2);
  indices.push_back(3);

  return type;
}

GLint GameObjects::getCubeAttributes(vector<GLfloat> &vertices, vector<GLuint> &indices)
{
  GLint randomNum = rand();
  GLint type;

  GLfloat red =  (randomNum%2 == 0) ? 255.0f : 0.0f;
  GLfloat green = (randomNum%2 != 0 && randomNum%5 == 0) ? 255.0f : 0.0f;
  GLfloat blue = 0.0f;

  type = (red == 255.0f) ? 0 : (green == 255.0f ? 1 : 2);

  vertices.push_back(-1.0f);
  vertices.push_back(1.5f);
  vertices.push_back(0.0f);
  vertices.push_back(red);
  vertices.push_back(green);
  vertices.push_back(blue);

  vertices.push_back(1.0f);
  vertices.push_back(1.5f);
  vertices.push_back(1.0f);
  vertices.push_back(red);
  vertices.push_back(green);
  vertices.push_back(blue);

  vertices.push_back(1.0f);
  vertices.push_back(-1.5f);
  vertices.push_back(1.0f);
  vertices.push_back(red);
  vertices.push_back(green);
  vertices.push_back(blue);

  vertices.push_back(-1.0f);
  vertices.push_back(-1.5f);
  vertices.push_back(1.0f);
  vertices.push_back(red);
  vertices.push_back(green);
  vertices.push_back(blue);

  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(2);

  indices.push_back(0);
  indices.push_back(2);
  indices.push_back(3);

  return type;
}

void GameObjects::getBallAttributes(vector<GLfloat> &vertices, vector<GLuint> &indices)
{
  vertices.push_back(0.0f);
  vertices.push_back(0.0f);
  vertices.push_back(0.0f);

  for(int i=0; i<=360; i++)
  {
    float y = 1.0f * cos(i*3.1415925/180);
    float x = 1.0f * sin(i*3.1415925/180);
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(0.0f);
  }

  for(int i=1;i<=360;i++)
  {
    indices.push_back(0);
    indices.push_back(i);
    indices.push_back(i+1);
  }
}

void GameObjects::getMirrorAttributes(vector<GLfloat> &vertices, vector<GLuint> &indices)
{
  vertices.push_back(-0.05f);
  vertices.push_back(1.0f);
  vertices.push_back(0.0f);
  vertices.push_back(1.0f);
  vertices.push_back(1.0f);
  vertices.push_back(1.0f);

  vertices.push_back(0.05f);
  vertices.push_back(1.0f);
  vertices.push_back(0.0f);
  vertices.push_back(0.1f);
  vertices.push_back(0.1f);
  vertices.push_back(0.1f);

  vertices.push_back(0.05f);
  vertices.push_back(-1.0f);
  vertices.push_back(0.0f);
  vertices.push_back(0.1f);
  vertices.push_back(0.1f);
  vertices.push_back(0.1f);

  vertices.push_back(-0.05f);
  vertices.push_back(-1.0f);
  vertices.push_back(0.0f);
  vertices.push_back(1.0f);
  vertices.push_back(1.0f);
  vertices.push_back(1.0f);

  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(2);

  indices.push_back(0);
  indices.push_back(2);
  indices.push_back(3);
}

void GameObjects::getCanonAttributes(vector<GLfloat> &vertices, vector<GLuint> &indices)
{
  vertices.push_back(0.0f);
  vertices.push_back(0.0f);
  vertices.push_back(0.0f);
  vertices.push_back(204.0f/255.0f);
  vertices.push_back(102.0f/255.0f);
  vertices.push_back(0.0f/255.0f);

  for(int i=0; i<=360; i++)
  {
    float y = 1 * cos(i*3.1415925/180);
    float x = 1 * sin(i*3.1415925/180);
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(0.0f);
    vertices.push_back(255.0f/255.0f);
    vertices.push_back(153.0f/255.0f);
    vertices.push_back(51.0f/255.0f);
  }
  int s = (vertices.size())/6 - 1;

  //main turret
  vertices.push_back(0.7f);
  vertices.push_back(0.3f);
  vertices.push_back(0.0f);
  vertices.push_back(166.0f/255.0f);
  vertices.push_back(166.0f/255.0f);
  vertices.push_back(166.0f/255.0f);

  vertices.push_back(0.7f);
  vertices.push_back(-0.3f);
  vertices.push_back(0.0f);
  vertices.push_back(51.0f/255.0f);
  vertices.push_back(51.0f/255.0f);
  vertices.push_back(51.0f/255.0f);

  vertices.push_back(1.5f);
  vertices.push_back(0.3f);
  vertices.push_back(0.0f);
  vertices.push_back(166.0f/255.0f);
  vertices.push_back(166.0f/255.0f);
  vertices.push_back(166.0f/255.0f);

  vertices.push_back(1.5f);
  vertices.push_back(-0.3f);
  vertices.push_back(0.0f);
  vertices.push_back(51.0f/255.0f);
  vertices.push_back(51.0f/255.0f);
  vertices.push_back(51.0f/255.0f);

  //sub turret
  vertices.push_back(1.5f);
  vertices.push_back(0.15f);
  vertices.push_back(0.0f);
  vertices.push_back(166.0f/255.0f);
  vertices.push_back(166.0f/255.0f);
  vertices.push_back(166.0f/255.0f);

  vertices.push_back(1.5f);
  vertices.push_back(-0.15f);
  vertices.push_back(0.0f);
  vertices.push_back(51.0f/255.0f);
  vertices.push_back(51.0f/255.0f);
  vertices.push_back(51.0f/255.0f);

  vertices.push_back(2.5f);
  vertices.push_back(0.15f);
  vertices.push_back(0.0f);
  vertices.push_back(166.0f/255.0f);
  vertices.push_back(166.0f/255.0f);
  vertices.push_back(166.0f/255.0f);

  vertices.push_back(2.5f);
  vertices.push_back(-0.15f);
  vertices.push_back(0.0f);
  vertices.push_back(51.0f/255.0f);
  vertices.push_back(51.0f/255.0f);
  vertices.push_back(51.0f/255.0f);

  for(int i=1;i<=360;i++)
  {
    indices.push_back(0);
    indices.push_back(i);
    indices.push_back(i+1);
  }

  //main turret
  indices.push_back(s+1);
  indices.push_back(s+2);
  indices.push_back(s+3);

  indices.push_back(s+2);
  indices.push_back(s+3);
  indices.push_back(s+4);

  s += 4;
  //sub turret
  indices.push_back(s+1);
  indices.push_back(s+2);
  indices.push_back(s+3);

  indices.push_back(s+2);
  indices.push_back(s+3);
  indices.push_back(s+4);
}
