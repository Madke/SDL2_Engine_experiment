#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>

#include "SDL_interface.h"
#include "conf.h"
//#include "entities.h"

enum State {
  STATE_EXIT = 0,
  STATE_FADE_IN,
  STATE_FADE_OUT,
  STATE_WAIT,
  STATE_MAIN,
  STATE_BLANK = 255
};

enum LemonError { LEMON_SDL_ERROR };

class conf;
class SDLInterface;
class EntityTable;

class controller {
public:
  controller(std::string);
  ~controller();

  int loop();
  int exit();

  void input(char);

  unsigned int timeLeft(unsigned int);

private:
  int loopState;
  int SDLState;

  int oldTicks;

  conf *config;
  SDLInterface *SDL;

  EntityTable *entities;
};
#endif // CONTROLLER_H
