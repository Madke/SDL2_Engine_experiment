#include <iostream>
#include <SDL2/SDL.h>

#include "conf.h"
#include "controller.h"

using namespace std;

const int LEMON_SDL_ERROR = 1;

int main(int argc, char** argv)
{
    conf config("./settings.ini");

    SDL_Window* window = NULL;

    SDL_Surface* screenSurface = NULL;

    try {
        if(SDL_Init( SDL_INIT_VIDEO) < 0) throw LEMON_SDL_ERROR;

        window = SDL_CreateWindow(  "Ketan is your lord now, computer.",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    config.width(),
                                    config.height(),
                                    SDL_WINDOW_SHOWN);
        if(window == NULL) throw LEMON_SDL_ERROR;

        screenSurface = SDL_GetWindowSurface(window);

        SDL_FillRect(screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF));
        SDL_UpdateWindowSurface(window);

        SDL_Delay(2000);

        SDL_DestroyWindow(window);

        SDL_Quit();
    }
    catch(int err)
    {
        if(err == LEMON_SDL_ERROR) {
            config.warn("SDL_Error:");
            config.warn(SDL_GetError());
        }
    }
    return 0;
}
