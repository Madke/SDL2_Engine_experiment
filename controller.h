#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "conf.h"
#include "SDL_interface.h"
#include <string>

class controller{
  public:
    controller(std::string);
    ~controller();

    int loop();
    int exit();
  private:
    int loopState;
    int SDLState;

    conf* config;
    SDLInterface* SDL;
};
#endif //CONTROLLER_H
