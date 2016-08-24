#include "SDL_interface.h"

const int LEMON_SDL_ERROR = 1;

SDLInterface::SDLInterface(Config* config, Controller* controller) {
    m_config = config;
    m_controller = controller;
    init();
}

SDLInterface::~SDLInterface() {
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void SDLInterface::init() {
    if(SDL_Init( SDL_INIT_VIDEO) < 0) throw LEMON_SDL_ERROR;

    m_config->warn("Setting up SDL window with parameters");
    m_config->addLog(m_config->width(), "width");
    m_config->addLog(m_config->height(), "height");

    m_window = SDL_CreateWindow(  "Ketan is your lord now, computer.",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                m_config->width(),
                                m_config->height(),
                                SDL_WINDOW_SHOWN);

    if(m_window == NULL) throw LEMON_SDL_ERROR;

    m_screenSurface = SDL_GetWindowSurface(m_window);

    SDL_FillRect(m_screenSurface, NULL, SDL_MapRGB( m_screenSurface->format, 0x00, 0x00, 0x00));
}

int SDLInterface::tick() {
    SDL_Delay(m_controller->timeLeft());
    int backColour = m_controller->getBackColour();
    SDL_FillRect(m_screenSurface, NULL, SDL_MapRGB( m_screenSurface->format, backColour, backColour, backColour));
    return 1;
}

void SDLInterface::updateWindow() {
    SDL_UpdateWindowSurface(m_window);
}

int SDLInterface::getError() {
    m_config->warn("SDL_Error:");
    m_config->warn(SDL_GetError());

    return 0;
}
