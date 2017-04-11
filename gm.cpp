
#include "config.hpp"

#include "globalVars.hpp"

Renderer::Renderer()
{
  this->gameObjs = new GameObjects();
}

void Renderer::initGameObjects()
{
  this->gameObjs->initCanon();

  this->gameObjs->initMirror(vec3(5.12f, 6.84f, -5.0f), vec3(-1.0f, -1.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f));
  this->gameObjs->initMirror(vec3(5.12f, 3.84f, -5.0f), vec3(-1.0f, 1.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f));

  this->gameObjs->initBucket(vec3(4.12f, 0.5f, -5.0f), "red");
  this->gameObjs->initBucket(vec3(6.12, 0.5f, -5.0f), "green");
}

void Renderer::initShaders()
{
  this->vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
  this->fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
  this->compileShaders();

  gameObjs->canon.programId = glCreateProgram();
  glAttachShader(gameObjs->canon.programId, this->vertexShaderId);
  glAttachShader(gameObjs->canon.programId, this->fragmentShaderId);
  glLinkProgram(gameObjs->canon.programId);

  GLint success;
  GLchar infoLog[512];
  glGetProgramiv(gameObjs->canon.programId, GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(gameObjs->canon.programId, 512, NULL, infoLog);
    std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
  }

  glDeleteShader(this->vertexShaderId);
  glDeleteShader(this->fragmentShaderId);
}

void Renderer::compileShaders()
{
  //char const * vertexShaderCode = this->getSource("Vertex");
  //char const * fragmentShaderCode = this->getSource("Fragment");

  const GLchar* vertexShaderCode = "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "layout (location = 1) in vec3 color;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "out vec3 outColor;\n"
    "void main()\n"
    "{\n"
    "gl_Position = projection * view * model * vec4(position, 1.0f);\n"
    "outColor = color;\n"
    "}\0";
    const GLchar* fragmentShaderCode = "#version 330 core\n"
    "in vec3 outColor;\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "color = vec4(outColor, 1.0f);\n"
    "}\n\0";

  GLint success;
  GLchar infoLog[512];

  glShaderSource(this->vertexShaderId, 1, &vertexShaderCode, NULL);
  glCompileShader(this->vertexShaderId);
  glGetShaderiv(this->vertexShaderId, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(this->vertexShaderId, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  glShaderSource(this->fragmentShaderId, 1, &fragmentShaderCode, NULL);
  glCompileShader(this->fragmentShaderId);
  glGetShaderiv(this->fragmentShaderId, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(this->fragmentShaderId, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }
}

char const *Renderer::getSource(string fileName)
{
  string code = "";
	ifstream stream(fileName, ios::in);
	if(stream.is_open()){
		string line = "";
		while(getline(stream, line))
			code += "\n" + line;
		stream.close();
	}

  return code.c_str();
}

void Renderer::render()
{
  gameObjs->renderCanon();
  gameObjs->renderBalls();
  gameObjs->renderMirrors();
  gameObjs->renderCubes();
  gameObjs->renderBuckets();
}
