all: Wolf

Wolf: main shaders
	g++ -o Wolf main.o chargeshaders.o -lSDL -lGL -lGLU -lGLEW

main:
	g++ -c main.cpp -lSDL -lGL -lGLU -lGLEW

shaders:
	g++ -c chargeshaders.cpp -lSDL -lGL -lGLU -lGLEW
 
