#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "conf.h"
#include "SDLInterface.h"
#include <string>

class controller{
  public:
    controller(std::string);
    ~controller();

    int loop();
  private:
    conf* config;
    SDLInterface* SDL;
};
#endif //CONTROLLER_H
