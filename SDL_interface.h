#ifndef SDL_INTERFACE_H
#define SDL_INTERFACE_H

#include "conf.h"
#include "controller.h"
#include<SDL2/SDL.h>

class controller;
class conf;

typedef SDL_Window Window;
typedef SDL_Surface Surface;
typedef conf Config;
typedef controller Controller;

class SDLInterface
{
    public:
        SDLInterface(Config*, Controller*);
        ~SDLInterface();

        void init();
        int tick(int);

        int fadeIn(float);
        int fadeOut(float);

        void updateWindow();

        int getError();
    private:
        Window* m_window;
        Surface* m_screenSurface;

        Config* m_config;
        Controller* m_controller;

        unsigned short int backColour;
};
#endif //SDL_INTERFACE_H
