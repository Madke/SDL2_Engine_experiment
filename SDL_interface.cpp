#include "SDL_interface.h"

// debug
#include <iostream>
float initTriangle[] = {0.0,     1.0, 0.0, -2.0 / 3, 0.0,  0.0,
                        2.0 / 3, 0.0, 0.0, 0.0,      -1.0, 0.0};
float finalTriangle[] = {0.0, 2.0 / 3, 0.0, -1.0, 0.0,      0.0,
                         1.0, 0.0,     0.0, 0.0,  -2.0 / 3, 0.0};

SDLInterface::SDLInterface(Config *config, Controller *controller) {
  m_config = config;
  m_controller = controller;
  init();
}

SDLInterface::~SDLInterface() {
  SDL_DestroyWindow(m_window);
  SDL_GL_DeleteContext(m_glContext);

  delete m_openGL;
  m_openGL = nullptr;

  SDL_Quit();
}

void SDLInterface::init() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    throw LEMON_SDL_ERROR;

  backColour = 0.0;

  m_config->warn("Setting up SDL window with parameters");
  m_config->addLog(m_config->width(), "width");
  m_config->addLog(m_config->height(), "height");

  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

  SDL_GL_SetSwapInterval(1);

  SDL_GL_LoadLibrary(nullptr);

  this->width = m_config->width();
  this->height = m_config->height();

  m_window = SDL_CreateWindow(
      "Ketan is your lord now, computer.", SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED, m_config->width(), m_config->height(),
      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
  m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

  if (m_window == nullptr)
    getError();

  m_glContext = SDL_GL_CreateContext(m_window);

  if (m_glContext == nullptr)
    getError();

  SDL_GL_MakeCurrent(m_window, m_glContext);

  gladLoadGLLoader(SDL_GL_GetProcAddress);

  glClearColor(backColour, backColour, backColour, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  m_openGL = new OpenGL(m_config);

  // m_openGL->init();

  const char *vsSource = m_config->vertexShader();
  const char *fsSource = m_config->fragmentShader();
  m_openGL->addVertexShader(vsSource);
  m_openGL->addFragmentShader(fsSource);
  m_openGL->compileProgram();
  m_openGL->addVertexStream(initTriangle, 12);
}

int SDLInterface::tick(int &state) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
    case SDL_QUIT:
      return 0;
      break;
    case SDL_KEYDOWN:
      input(e.key.keysym.sym);
      break;
    case SDL_WINDOWEVENT:
      if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
        resizeWindow(e.window.data1, e.window.data2);
      }
      break;
    }
  }

  if (resizedWindow) {
    int w, h;
    SDL_GetWindowSize(m_window, &w, &h);
    SDL_SetWindowSize(m_window, w, h);
    m_openGL->windowResize(w, h);
  }

  unsigned int timeLeft = m_controller->timeLeft(SDL_GetTicks());

  SDL_Delay(timeLeft);

  switch (state) {
  case STATE_FADE_IN:
    return fadeIn(2.0, state);
  case STATE_FADE_OUT:
    return fadeOut(1.0, state);
  case STATE_MAIN:
    return draw();
  case STATE_WAIT:
  case STATE_BLANK:
    return STATE_WAIT;
  default:
    return STATE_EXIT;
  }
}

int SDLInterface::draw() {
  Uint32 ticks = SDL_GetTicks();
  const float pi = acos(-1);
  float tickAngle = ticks * (2 * pi) / 1000;
  m_openGL->background(backColour);
  m_openGL->setUniform4F("time", sin(tickAngle), sin(tickAngle + (2 * pi / 3)),
                         sin(tickAngle + (4 * pi / 3)), 0.0f);
  m_openGL->draw();
  return STATE_MAIN;
}

int SDLInterface::fadeIn(float time, int &state) {
  float delta = 0.8 / (time * m_config->fps());
  if (backColour > 0.8 - delta)
    backColour = 0.8 - delta;
  backColour += delta;

  m_openGL->background(backColour);
  draw();
  if (backColour >= 0.8) {
    return STATE_MAIN;
  }
  return STATE_FADE_IN;
}

int SDLInterface::fadeOut(float time, int &state) {
  float delta = 0.8 / (time * m_config->fps());
  if (backColour < delta)
    backColour = delta;
  backColour -= delta;

  // SDL_SetRenderDrawColor(m_renderer, backColour, backColour, backColour,
  // 255);
  // SDL_RenderFillRect(m_renderer, nullptr);

  m_openGL->background(backColour);
  m_openGL->addVertexStream(finalTriangle, 12);

  draw();
  if (backColour <= delta) {
    return STATE_EXIT;
  }
  return STATE_FADE_OUT;
}

typedef struct {
  int *state;
  int nextState;
} transition;

transition next = {nullptr, STATE_BLANK};

unsigned int changeState(unsigned int interval, void *next) {
  transition *next_struct = (transition *)next;
  (*next_struct->state) = next_struct->nextState;
  return (0);
}

void SDLInterface::wait(float time, int &state, int nextState) {
  next.state = &state;
  next.nextState = nextState;
  SDL_AddTimer((unsigned int)(time * 1000), changeState, &next);
}

void SDLInterface::input(SDL_Keycode input) {
  std::cout << input << std::endl;
  m_controller->input((char)input);
}

void SDLInterface::updateWindow() { SDL_GL_SwapWindow(m_window); }

unsigned int resizeWindowTimerCallback(unsigned int interval,
                                       void *resizedWindow) {
  bool *flag = (bool *)resizedWindow;

  *flag = false;

  std::cout << "stopped forcing window resize" << std::endl;
  return 0;
}

void SDLInterface::resizeWindow(float width, float height) {
  SDL_RemoveTimer(resizeTimer);
  std::cout << "================================================" << std::endl;
  std::cout << "RESIZE:" << std::endl;
  std::cout << "================================================" << std::endl;
  std::cout << "new: " << width << " x " << height << std::endl;
  float origHeight = this->height;
  float origWidth = this->width;
  float aspect = (float)m_config->width() / m_config->height();

  float changeWidth = std::max(width, origWidth) / std::min(width, origWidth);
  float changeHeight =
      std::max(height, origHeight) / std::min(height, origHeight);

  std::cout << "original:" << origWidth << " x " << origHeight << std::endl;

  std::cout << "change:" << changeWidth << " x " << changeHeight << std::endl;

  std::cout << "keeping " << ((changeWidth > changeHeight) ? "width" : "height")
            << std::endl;

  int w, h;
  if (changeWidth > changeHeight) {
    w = width;
    h = floor(width / aspect);
  } else {
    w = floor(height * aspect);
    h = height;
  }

  SDL_SetWindowSize(m_window, w, h);

  if (!resizedWindow) {
    this->width = w;
    this->height = h;
  } // not currently resizing

  resizedWindow = true;
  SDL_TimerCallback callback = resizeWindowTimerCallback;
  resizeTimer = SDL_AddTimer(2000, callback, &resizedWindow);
}

void SDLInterface::getError() {
  m_config->warn("SDL_Error:");
  m_config->warn(SDL_GetError());

  std::cout << SDL_GetError() << std::endl;

  throw LEMON_SDL_ERROR;
}
