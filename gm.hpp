
#include "config.hpp"

#include "go.hpp"

#ifndef RENDERER
#define RENDERER

class Renderer {

public:
  GLuint vertexShaderId;
  GLuint fragmentShaderId;
  GameObjects *gameObjs;

  char const *getSource(string fileName);
  void compileShaders();
  Renderer();
  void initGameObjects();
  void render();
  void initShaders();

};

#endif
