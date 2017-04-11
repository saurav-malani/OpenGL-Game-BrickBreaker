
#include "config.hpp"

#ifndef GAME_OBJECTS
#define GAME_OBJECTS

//Game object structures

typedef struct Canon {
  GLuint canonObj;
  GLuint canonBuffer;
  GLuint canonElementBuffer;
  GLuint programId;
  GLfloat rotation;
  vec3 direction;
  vec3 position;
  vector<GLfloat> canonVerts;
  vector<GLuint> canonIndices;
} Canon;

typedef struct Ball {
  GLuint ballObj;
  GLuint ballBuffer;
  GLuint ballElementBuffer;
  GLuint programId;
  GLfloat diameter;
  GLfloat speed;
  mat4 modelMatrix;
  vec3 direction;
  vec3 position;
  vector<GLfloat> ballVerts;
  vector<GLuint> ballIndices;
} Ball;

typedef struct Mirror {
  GLuint mirrorObj;
  GLuint mirrorBuffer;
  GLuint mirrorElementBuffer;
  GLuint programId;
  GLfloat rotation;
  mat4 modelMatrix;
  vec3 direction;
  vec3 position;
  vector<GLfloat> mirrorVerts;
  vector<GLuint> mirrorIndices;
} Mirror;

typedef struct Cube {
  GLuint cubeObj;
  GLuint cubeBuffer;
  GLuint cubeElementBuffer;
  GLuint programId;
  GLint type;
  mat4 modelMatrix;
  vec3 direction;
  vec3 position;
  vector<GLfloat> cubeVerts;
  vector<GLuint> cubeIndices;
} Cube;

typedef struct Bucket {
  GLuint bucketObj;
  GLuint bucketBuffer;
  GLuint bucketElementBuffer;
  GLuint programId;
  GLint type;
  mat4 modelMatrix;
  vec3 position;
  vector<GLfloat> bucketVerts;
  vector<GLuint> bucketIndices;
} Bucket;

//GameObjects Class Definition

class GameObjects {

public:
  GLfloat left, right, down, up;

  mat4 viewMatrix;
  mat4 projectionMatrix;
  GameObjects();
  void setProjection(GLfloat left, GLfloat right, GLfloat down, GLfloat up);
  void resetProjectionAndView();
  void mvp(GLint programId, mat4 modelMatrix);
  void unbindAll();
  void bindVertexArrays(GLuint number, GLuint &id);
  void bindBuffers(GLenum target, GLuint number, GLuint &id, GLsizeiptr size, const GLvoid *data);

  //Canon
  Canon canon {0, 0, 0, 0, 0.0f};
  void getCanonAttributes(vector<GLfloat> &vertices, vector<GLuint> &indices);
  void initCanon();
  void renderCanon();
  void moveCanon(bool isUp);
  void rotateCanon(bool isUp);

  //Ball
  list<Ball> balls;
  void getBallAttributes(vector<GLfloat> &vertices, vector<GLuint> &indices);
  void initBall();
  void renderBalls();

  //Mirror
  list<Mirror> mirrors;
  void getMirrorAttributes(vector<GLfloat> &vertices, vector<GLuint> &indices);
  void initMirror(vec3 position, vec3 rotation, vec3 normal);
  void renderMirrors();

  //Cubes
  list<Cube> cubes;
  GLint getCubeAttributes(vector<GLfloat> &vertices, vector<GLuint> &indices);
  void initCube(vec3 position);
  void renderCubes();

  //Bucket
  vector<Bucket> buckets;
  GLint getBucketAttributes(vector<GLfloat> &vertices, vector<GLuint> &indices, string type);
  void initBucket(vec3 position, string type);
  void renderBuckets();
  void moveLeftBucket(bool right);
  void moveRightBucket(bool left);

};

#endif
