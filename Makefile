# INC = -IC:/Dev/glad/include -IC:/Dev/glfw-3.3.3.bin.WIN64/include -IC:/Dev/glm
LIBS = -lopengl32 -lgdi32 -luser32 -lkernel32 -lglu32 -lglew32
SRC = main.cpp
CC = g++ -std=c++17
INC = C:\Dev\glad\src\glad.c -IC:\Dev\glm -IC:\Dev\glad\include \
		-IC:\mingw64\x86_64-w64-mingw32\include -LC:\mingw64\x86_64-w64-mingw32\lib \
		-IC:\Dev\SDL2-2.0.10\x86_64-w64-mingw32\include \
		-LC:\Dev\SDL2-2.0.10\x86_64-w64-mingw32\lib \
		-IC:\Dev\glew-2.1.0\include \
		-LC:\Dev\glew-2.1.0\lib\Release\x64 \
		-w -lmingw32 -lSDL2main -lSDL2 -DLOG_USE_COLOR -lgdi32 -lopengl32 \
		-IC:\Dev\inc
all:
	$(CC) $(SRC) $(INC) $(LIBS)
# CC=g++ -g
# CFLAGS=-std=c++17
# SRCS=$(wildcard Engine_OpenGL/src/*.cpp)
# INCL=$(wildcard Engine_OpenGL\src\*.hpp)
# OBJS=$(patsubst Engine_OpenGL\src\%.cpp,Engine_OpenGL\build\%.o,$(SRCS))
# ENGINE=CowEngine

# .PHONY: $(info $$SRCS is [${SRCS}]) all clean default setup
# default: $(ENGINE)
# all: setup $(ENGINE)
# setup:
# 	mkdir build;\

# build\%.o: src\%.cpp $(INCL)
# 	$(CC) $(CFLAGS) -c $< -o $@

# $(ENGINE): $(OBJS)
# 	$(CC) $(CFLAGS) $(OBJS) -o $(ENGINE) $(LIB)

# clean:
# 	del /q build\*
# 	del /q *.exe



















#OBJS = Main.cpp
#BINS = $(GAMEDATA) $(ENGINE) $(RECTANGLE) $(RENDEROPTIONS)
#GAMEDATA = build\GameData.o
#ENGINE =  build\Engine.o
#RECTANGLE = build\Rectangle.o
#RENDEROPTIONS = build\RenderOptions.o
#CC = g++ -std=c++14
#LIB = -IC:\Dev\SDL2-2.0.10\x86_64-w64-mingw32\include -LC:\Dev\SDL2-2.0.10\x86_64-w64-mingw32\lib -w -lmingw32 -lSDL2main -lSDL2 -DLOG_USE_COLOR
#SRC = $(wildcard Engine_CPP\src\*.cpp)
#INCL = $(wildcard Engine_CPP\inc\*.hpp)
#
#
#all: $(OBJS)
#	mingw32-make.exe build
#	$(CC) $(OBJS) $(BINS) -o a -IC:\Dev\SDL2-2.0.10\x86_64-w64-mingw32\include -LC:\Dev\SDL2-2.0.10\x86_64-w64-mingw32\lib -w -lmingw32 -lSDL2main -lSDL2 -DLOG_USE_COLOR
#
#build: $(BINS)
#	mingw32-make.exe $(GAMEDATA)
#	mingw32-make.exe $(ENGINE)
#	mingw32-make.exe $(RECTANGLE)
#	mingw32-make.exe $(RENDEROPTIONS)
#
#$(ENGINE): Engine_CPP\Engine.cpp Engine_CPP\Engine.hpp
#	$(CC) Engine_CPP\Engine.cpp -c -o build\Engine.o $(LIB)
#
#$(GAMEDATA): Engine_CPP\GameData.cpp Engine_CPP\GameData.hpp
#	$(CC) Engine_CPP\GameData.cpp -c -o $(GAMEDATA) $(LIB)
#
#$(RECTANGLE): Engine_CPP\Graphics\Shape\Rectangle.cpp Engine_CPP\Graphics\Shape\Rectangle.hpp
#	$(CC) Engine_CPP\Graphics\Shape\Rectangle.cpp -c -o $(RECTANGLE) $(LIB)
#
#$(RENDEROPTIONS): Engine_CPP\Graphics\RenderOptions.cpp Engine_CPP\Graphics\RenderOptions.hpp
#	$(CC) Engine_CPP\Graphics\RenderOptions.cpp -c -o $(RENDEROPTIONS) $(LIB)
#
#clean:
#	del /q build\*
#	del /q *.exe
#
