#include "SDL_interface.h"

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

    backColour = 0x00;

    m_config->warn("Setting up SDL window with parameters");
    m_config->addLog(m_config->width(), "width");
    m_config->addLog(m_config->height(), "height");

    m_window = SDL_CreateWindow("Ketan is your lord now, computer.",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                m_config->width(),
                                m_config->height(),
                                SDL_WINDOW_SHOWN);
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

    if(m_window == NULL) throw LEMON_SDL_ERROR;

    SDL_SetRenderDrawColor(m_renderer, backColour, backColour, backColour, 255);
    SDL_RenderFillRect(m_renderer, NULL);

    //m_screenSurface = SDL_GetWindowSurface(m_window);

    //SDL_FillRect(m_screenSurface, NULL, SDL_MapRGB( m_screenSurface->format, backColour, backColour, backColour));
}

int SDLInterface::tick(int &state) {
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        if(e.type == SDL_QUIT) {
            return 0;
        }
    }

    unsigned int timeLeft = m_controller->timeLeft( SDL_GetTicks() );

    SDL_Delay(timeLeft);

    switch(state) {
        case STATE_FADE_IN:
        return fadeIn(2.0, state);
        case STATE_FADE_OUT:
        return fadeOut(2.0, state);
        case STATE_BLANK:
        return STATE_BLANK;
        default:
        return STATE_EXIT;
    }
}

int SDLInterface::fadeIn(float time, int& state) {
    short int delta = 256.0 / ( time * m_config->fps());
    if(backColour > 0xFF - delta) backColour = 0xFF - delta;
    backColour += delta;
    SDL_SetRenderDrawColor(m_renderer, backColour, backColour, backColour, 255);
    SDL_RenderFillRect(m_renderer, NULL);
    //SDL_FillRect(m_screenSurface, NULL, SDL_MapRGB( m_screenSurface->format, backColour, backColour, backColour));
    if(backColour == 0xFF) {
        wait(2.0, state, STATE_FADE_OUT);
        return STATE_BLANK;
    }
    return STATE_FADE_IN;
}

int SDLInterface::fadeOut(float time, int& state) {
    short int delta = 256.0 / ( time * m_config->fps());
    if(backColour < delta) backColour = delta;
    backColour -= delta;
    SDL_SetRenderDrawColor(m_renderer, backColour, backColour, backColour, 255);
    SDL_RenderFillRect(m_renderer, NULL);
    //SDL_FillRect(m_screenSurface, NULL, SDL_MapRGB( m_screenSurface->format, backColour, backColour, backColour));
    if(backColour == 0x00) {
        return STATE_EXIT;
    }
    return STATE_FADE_OUT;
}

typedef struct {
    int* state;
    int  nextState;
} transition;

transition next = {nullptr, STATE_BLANK};

unsigned int changeState(unsigned int interval, void* next) {
    transition* next_struct = (transition*)next;
    (*next_struct->state) = next_struct->nextState;
    return(0);
}

void SDLInterface::wait(float time, int& state, int nextState) {
    next.state = &state;
    next.nextState = nextState;
    SDL_AddTimer((unsigned int)(time * 1000), changeState, &next);
}

void SDLInterface::updateWindow() {
    SDL_RenderPresent(m_renderer);
    SDL_UpdateWindowSurface(m_window);
}

int SDLInterface::getError() {
    m_config->warn("SDL_Error:");
    m_config->warn(SDL_GetError());

    return STATE_EXIT;
}
