#ifndef GLCONTROLLER_H
#define GLCONTROLLER_H

#include "conf.h"
#include "glad.h"

class conf;
typedef conf Config;

class openGL {
public:
  openGL(Config *);
  ~openGL();

  void init(){};

  void addVertexShader(const char *);
  void addFragmentShader(const char *);
  void compileProgram();

  void addTriangle(float *, unsigned int);

  void draw();
  Config *m_config;

  GLuint VBO;
  GLuint VAO;
  GLuint vertexShader;
  GLuint fragmentShader;
  GLuint shaderProgram;
};

#endif // GLCONTROLLER_H
