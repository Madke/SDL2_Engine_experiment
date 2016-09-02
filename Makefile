OBJS = main.cpp conf.cpp controller.cpp SDL_interface.cpp

OBJ_NAME = engine.app

all: $(OBJS)
	clang++ -std=c++11 $(OBJS) -lSDL2 -o $(OBJ_NAME)
