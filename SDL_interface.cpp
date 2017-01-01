#include "SDL_interface.h"

//debug
#include <iostream>

SDLInterface::SDLInterface(Config* config, Controller* controller) {
    m_config = config;
    m_controller = controller;
    init();
}

SDLInterface::~SDLInterface() {
    SDL_DestroyWindow(m_window);
    SDL_GL_DeleteContext(m_glContext);

    SDL_Quit();
}

void SDLInterface::init() {
    if(SDL_Init( SDL_INIT_VIDEO) < 0) throw LEMON_SDL_ERROR;

    backColour = 0.0;

    m_config->warn("Setting up SDL window with parameters");
    m_config->addLog(m_config->width(), "width");
    m_config->addLog(m_config->height(), "height");

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

    SDL_GL_SetSwapInterval(1);

    SDL_GL_LoadLibrary(NULL);

    m_window = SDL_CreateWindow("Ketan is your lord now, computer.",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                m_config->width(),
                                m_config->height(),
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

    if(m_window == nullptr) getError();

    m_glContext = SDL_GL_CreateContext(m_window);

    if(m_glContext == nullptr) getError();

    SDL_GL_MakeCurrent(m_window, m_glContext);

    gladLoadGLLoader(SDL_GL_GetProcAddress);

    glClearColor(backColour, backColour, backColour, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    //SDL_SetRenderDrawColor(m_renderer, backColour, backColour, backColour, 255);
    //SDL_RenderFillRect(m_renderer, nullptr);

    GLfloat vertices[] = {
      -0.5, -0.5, 0.0,
       0.5, -0.5, 0.0,
       0.0, 1.0, 0.0
    };

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    const char* vsSource = m_config->vertexShader();
    glShaderSource(vertexShader, 1, &vsSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char* fsSource = m_config->fragmentShader();
    glShaderSource(fragmentShader, 1, &fsSource, nullptr);
    glCompileShader(fragmentShader);


    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    glUseProgram(shaderProgram);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

int SDLInterface::tick(int &state) {
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
      switch(e.type) {
        case SDL_QUIT:
          return 0;
          break;
        case SDL_KEYDOWN:
          input(e.key.keysym.sym);
          break;
      }
    }

    unsigned int timeLeft = m_controller->timeLeft( SDL_GetTicks() );

    SDL_Delay(timeLeft);

    switch(state) {
        case STATE_FADE_IN:
          return fadeIn(2.0, state);
        case STATE_FADE_OUT:
          return fadeOut(1.0, state);
        case STATE_MAIN:
          return draw();
        case STATE_WAIT:
        case STATE_BLANK:
          return STATE_WAIT;
        default:
          return STATE_EXIT;
    }
}

int SDLInterface::draw() {

    return STATE_MAIN;
}

int SDLInterface::fadeIn(float time, int& state) {
    float delta = 0.8 / ( time * m_config->fps());
    if(backColour > 0.8 - delta) backColour = 0.8 - delta;
    backColour += delta;
    //SDL_SetRenderDrawColor(m_renderer, backColour, backColour, backColour, backColour);
    //SDL_RenderFillRect(m_renderer, nullptr);

    glClearColor(backColour, backColour, backColour, backColour);
    glClear(GL_COLOR_BUFFER_BIT);

    if(backColour >= 0.8) {
        //wait(2.0, state, STATE_FADE_OUT);
        return STATE_MAIN;
    }
    return STATE_FADE_IN;
}

int SDLInterface::fadeOut(float time, int& state) {
    float delta = 0.8 / ( time * m_config->fps());
    if(backColour < delta) backColour = delta;
    backColour -= delta;

    //SDL_SetRenderDrawColor(m_renderer, backColour, backColour, backColour, 255);
    //SDL_RenderFillRect(m_renderer, nullptr);

    glClearColor(backColour, backColour, backColour, backColour);
    glClear(GL_COLOR_BUFFER_BIT);

    if(backColour <= delta) {
        //wait(2.0, state, STATE_FADE_IN);
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

void SDLInterface::input(SDL_Keycode input) {
    std::cout<<input<<std::endl;
    m_controller->input((char)input);
}

void SDLInterface::updateWindow() {
    SDL_GL_SwapWindow(m_window);
}

void SDLInterface::getError() {
    m_config->warn("SDL_Error:");
    m_config->warn(SDL_GetError());

    std::cout << SDL_GetError() << std::endl;

    throw LEMON_SDL_ERROR;
}
