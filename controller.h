#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "conf.h"
#include "SDL_interface.h"
#include <string>

class conf;
class SDLInterface;

class controller
{
  public:
    controller(std::string);
    ~controller();

    int loop();
    int exit();

    unsigned int timeLeft(unsigned int);
  private:
    int loopState;
    int SDLState;

    int oldTicks;

    conf* config;
    SDLInterface* SDL;
};
#endif //CONTROLLER_H
