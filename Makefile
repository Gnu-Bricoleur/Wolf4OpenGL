all: Wolf

Wolf: main shaders textures
	g++ -o Wolf -g main.o sdlglutils.o chargeshaders.o -lSDL -lGL -lGLU -lGLEW -lSDL_image

main:
	g++ -c -g main.cpp -lSDL -lGL -lGLU -lGLEW

shaders:
	g++ -c -g chargeshaders.cpp -lSDL -lGL -lGLU -lGLEW

textures: 
	g++ -c -g sdlglutils.cpp -lSDL -lGL -lGLU -lGLEW -lSDL_image
 
