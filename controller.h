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

    unsigned short int getBackColour();
    unsigned int timeLeft();
  private:
    int loopState;
    int SDLState;

    unsigned short int backColour;

    conf* config;
    SDLInterface* SDL;
};
#endif //CONTROLLER_H
