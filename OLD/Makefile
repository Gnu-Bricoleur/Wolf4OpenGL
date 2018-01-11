all: Wolf

Wolf: main shaders
	g++ -o Wolf -g main.o chargeshaders.o -lSDL -lGL -lGLU -lGLEW

main:
	g++ -c -g main.cpp -lSDL -lGL -lGLU -lGLEW

shaders:
	g++ -c -g chargeshaders.cpp -lSDL -lGL -lGLU -lGLEW
 
