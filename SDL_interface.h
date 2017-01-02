#ifndef SDL_INTERFACE_H
#define SDL_INTERFACE_H

#include "conf.h"
#include "controller.h"
#include "glad.h"
#include "glcontroller.h"
#include <SDL2/SDL.h>

class controller;
class conf;

typedef SDL_Window Window;
typedef SDL_Renderer Renderer;
typedef SDL_Surface Surface;
typedef SDL_GLContext GLContext;

typedef conf Config;
typedef controller Controller;
typedef openGL OpenGL;

class SDLInterface
{
    public:
        SDLInterface(Config*, Controller*);
        ~SDLInterface();

        void init();
        int tick(int&);

        //SDL States
        int fadeIn(float, int&);
        int fadeOut(float, int&);
        int draw();

        //utility functions
        void wait(float, int&, int);
        void input(SDL_Keycode);

        void updateWindow();

        void getError();
    private:
        Window* m_window;
        Renderer* m_renderer;
        Surface* m_screenSurface;
        GLContext m_glContext;

        Config* m_config;
        Controller* m_controller;
        OpenGL* m_openGL;

        float backColour;
};
#endif //SDL_INTERFACE_H
