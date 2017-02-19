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

  void addVertexStream(float *, unsigned int);

  void setUniform4F(const char *name, float x, float y, float z, float w);
  void background(float);

  void windowResize(int, int);
  void draw();
  Config *m_config;

  GLuint VBO = 0;
  GLuint VAO = 0;
  GLuint vertexShader = 0;
  GLuint fragmentShader = 0;
  GLuint shaderProgram = 0;
};

#endif // GLCONTROLLER_H
