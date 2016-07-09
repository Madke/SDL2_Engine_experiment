#include "controller.h"

#include <string>

using namespace std;
controller::controller(string pathToConfig) {
    config = new conf(pathToConfig);
    SDL = new SDLInterface(config);

    SDL->init();
    config->addLog("Controller online.", "Controller");
}

controller::~controller() {
    delete SDL;
    delete config;
}

int controller::loop() {
    try {
        SDLState = SDL->tick();
        SDL->updateWindow();
        if(SDLState == 1) loopState =1;
    }
    catch (int err) {
        loopState = SDL->getError();
    }
    return loopState;
}

int controller::exit() {
    return loopState;
}
