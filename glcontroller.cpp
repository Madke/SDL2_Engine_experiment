#include "glcontroller.h"

openGL::openGL(Config *config) {
  m_config = config;
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
}

openGL::~openGL() {}

void openGL::addVertexShader(const char *source) {
  vertexShader = glCreateShader(GL_VERTEX_SHADER);

  glShaderSource(vertexShader, 1, &source, nullptr);
  glCompileShader(vertexShader);
}
void openGL::addFragmentShader(const char *source) {
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(fragmentShader, 1, &source, nullptr);
  glCompileShader(fragmentShader);
}
void openGL::compileProgram() {
  GLuint shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void openGL::addTriangle(float *) {}

void openGL::draw() {}
