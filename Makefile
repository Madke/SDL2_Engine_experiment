OBJS =  glad.c main.cpp conf.cpp controller.cpp SDL_interface.cpp

OBJ_NAME = engine.app

all: $(OBJS)
	clang++ -std=c++11 $(OBJS) -lSDL2 -o $(OBJ_NAME)

Debug: $(OBJS)
	clang++ -std=c++11 $(OBJS) -lSDL2 -o $(OBJ_NAME)

Run: $(OBJ_NAME)
	$(OBJ_NAME)
