SRC = bootman_III.cpp
OBJ = $(SRC:.cpp=.o)
CC = g++
COMPILER_FLAGS = -w #-Wl,-subsystem,windows
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

OBJ_NAME = bootmanIII

$(OBJ_NAME) : $(OBJ)
	$(CC) $^ $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $@