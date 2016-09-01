#include "controller.h"

#include <string>

using namespace std;
controller::controller(string pathToConfig) {
    config = new conf(pathToConfig);
    SDL = new SDLInterface(config, this);

    loopState = 1;
    oldTicks = 0;

    config->addLog("Controller online.", "Controller");
}

controller::~controller() {
    delete SDL;
    delete config;
}

int controller::loop() {
    try {
        SDLState = SDL->tick(loopState);
        SDL->updateWindow();
        loopState = SDLState;
    }
    catch (int err) {
        loopState = SDL->getError();
    }
    return loopState;
}

int controller::exit() {
    return loopState;
}

unsigned int controller::timeLeft(unsigned int tick) {
    int target = (1000/config->fps());
    int timeElapsed = (tick > oldTicks) ? tick - oldTicks : 0;
    oldTicks = tick;
    config->addLog(tick, "tick");
    return (target > timeElapsed) ? target - timeElapsed : 0;
}
