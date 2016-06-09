OBJS = main.cpp conf.cpp

OBJ_NAME = a.out

all: $(OBJS)
	clang++ $(OBJS) -lSDL2 -o $(OBJ_NAME)
