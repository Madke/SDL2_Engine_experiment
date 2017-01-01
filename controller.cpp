#include "controller.h"

#include <string>

using namespace std;
controller::controller(string pathToConfig) {
    config = new conf(pathToConfig);
    SDL = new SDLInterface(config, this);

    loopState = STATE_MAIN;
    oldTicks = 0;

    config->addLog("Controller online.", "Controller");
}

controller::~controller() {
    delete SDL;
    delete config;
}

int controller::loop() {
    try {
        int oldState = loopState;
        SDLState = SDL->tick(loopState);
        SDL->updateWindow();
        loopState = (loopState == oldState)? SDLState : loopState;
    }
    catch (int err) {
        if(err == LEMON_SDL_ERROR)
          loopState = STATE_EXIT;
        else loopState = STATE_BLANK;
    }
    return loopState;
}

int controller::exit() {
    return loopState;
}

void controller::input(char input) {
    if (input == '\033') {
        loopState = STATE_FADE_OUT;
    }
}

unsigned int controller::timeLeft(unsigned int tick) {
    int target = (1000/config->fps());
    int timeElapsed = (tick > oldTicks) ? tick - oldTicks : 0;
    oldTicks = tick;
    return (target > timeElapsed) ? target - timeElapsed : 0;
}
