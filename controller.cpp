#include "conf.h"
#include "SDLInterface.h"

#include <string>

using namespace std;
controller::controller(string pathToConfig) {
    config = new conf(pathToConfig);

    SDL = new SDLInterface(config);
}

controller::~controller() {
    delete SDL;

    delete config;
}

int controller::loop() {
    SDL->tick();
    SDL->updateWindow();
}
