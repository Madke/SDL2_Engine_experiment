#include "glcontroller.h"

// Debug
#include <iostream>

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

  GLint success;
  GLchar infoLog[2048];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 2048, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
    glGetShaderSource(vertexShader, 2048, nullptr, infoLog);
    std::cout << "vs:______________________" << std::endl << infoLog;
  }
}
void openGL::addFragmentShader(const char *source) {
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(fragmentShader, 1, &source, nullptr);
  glCompileShader(fragmentShader);

  GLint success;
  GLchar infoLog[2048];
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 2048, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
    glGetShaderSource(fragmentShader, 2048, nullptr, infoLog);
    std::cout << "fs:______________________" << std::endl << infoLog;
  }
}
void openGL::compileProgram() {
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  GLint success;
  GLchar infoLog[2048];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 2048, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }
}

void openGL::addVertexStream(float triangle[], unsigned int size) {
  if (VBO == 0)
    glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), triangle, GL_STATIC_DRAW);

  if (VAO == 0)
    glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
  glEnableVertexAttribArray(0);
}

void openGL::background(float colour) {
  glClearColor(colour, colour, colour, colour);
  glClear(GL_COLOR_BUFFER_BIT);
}

void openGL::setUniform4F(const char *name, float x, float y, float z,
                          float w) {
  GLint vertexColorLocation = glGetUniformLocation(shaderProgram, name);
  glUniform4f(vertexColorLocation, x, y, z, w);
}

void openGL::draw() {
  glUseProgram(shaderProgram);

  // triangles

  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
