#include "controller.h"

#include <string>

using namespace std;
controller::controller(string pathToConfig) {
    config = new conf(pathToConfig);
    SDL = new SDLInterface(config, this);

    backColour = 0;

    config->addLog("Controller online.", "Controller");
}

controller::~controller() {
    delete SDL;
    delete config;
}

int controller::loop() {
    try {
        backColour++;
        SDLState = SDL->tick();
        SDL->updateWindow();
        if(SDLState == 1) loopState = 1;
        if(backColour == 0xFF) loopState = 0;
    }
    catch (int err) {
        loopState = SDL->getError();
    }
    return loopState;
}

int controller::exit() {
    return loopState;
}

unsigned short int controller::getBackColour() {
    return backColour;
}

unsigned int controller::timeLeft() {
    int target = (1000/config->fps());
    int timeElapsed = 0;
    return (target - timeElapsed);
}
