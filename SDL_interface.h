#ifndef SDL_INTERFACE_H
#define SDL_INTERFACE_H

#include "conf.h"
#include<SDL2/SDL.h>

typedef SDL_Window Window;
typedef SDL_Surface Surface;
typedef conf Config;

class SDLInterface {
    public:
        SDLInterface(Config*);
        ~SDLInterface();

        void init();
        int tick();
        void updateWindow();

        int getError();
    private:
        Window* m_window;
        Surface* m_screenSurface;

        Config* m_config;
};
#endif //SDL_INTERFACE_H
