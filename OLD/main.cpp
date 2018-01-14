#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <vector>
#include "chargeshaders.h"
#include "sdlglutils.h" // textures
#include "variables.h"
#include "fonctions.h"

using namespace std;

int main(int argc, char *argv[])
{
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);
    
    freopen( "CON", "w", stdout );
	freopen( "CON", "w", stderr );
    
    atexit(SDL_Quit);
    SDL_WM_SetCaption("Wolfenstein", NULL);
    
    SDL_SetVideoMode(640, 480, 32, SDL_OPENGL);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(70,(double)640/480,1,1000);

    glEnable(GL_DEPTH_TEST);

    Uint32 last_time = SDL_GetTicks();
    Uint32 current_time,ellapsed_time;
    Uint32 start_time;

	SDL_EnableKeyRepeat(10, 10);

	SDL_WM_GrabInput(SDL_GRAB_ON);									// A REMETRRE POUR UTILISER GBD CONFORTABLEMENT §§§§§§§§!!!!!!!!!!!!!!!!!!!!
    //SDL_ShowCursor(SDL_DISABLE);
	SDL_WarpMouse(320, 240);
    glEnable(GL_TEXTURE_2D);

    texture = loadTexture("textures.jpg");

    Chargement_niveau();

	//mise en place des shaders
	glewInit();
	programID = LoadShaders( "vertex.glsl", "fragment.glsl" );
	
	
	for (;;)
    {
        start_time = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {

            switch(event.type)
            {
                case SDL_QUIT:
					exit(0);
					break;
                case SDL_KEYDOWN:
					avancer = 0;
					strafer = 0;
					switch(event.key.keysym.sym)
					{
						case SDLK_z:
						case SDLK_UP: // Flèche haut
							deplacementX += vitesse;
							avancer = 1;
							break;
						case SDLK_s:
						case SDLK_DOWN: // Flèche bas
							deplacementX -= vitesse;
							avancer = -1;
							break;
						case SDLK_d:
						case SDLK_RIGHT: // Flèche droite
							deplacementY -= vitesse;
							strafer = 1;
							break;
						case SDLK_q:
						case SDLK_LEFT: // Flèche gauche
							deplacementY += vitesse;
							strafer = -1;
							break;
						case SDLK_ESCAPE:
							exit(0);
					}
					break;
				case SDL_MOUSEMOTION:
					angleZ += event.motion.x;					
					angleZ = (angleZ/640)*360;//conversion resolution degree
					cout<<angleZ<<endl;
					if(angleZ <= 1)
					{
						SDL_WarpMouse(639, 240);
					}
					if(angleZ >= 360)
					{
						SDL_WarpMouse(1, 240);
					}
					angleZ = (angleZ/180)*PI;//conversion degre radian									
					break;
            }
        }

        current_time = SDL_GetTicks();
        ellapsed_time = current_time - last_time;
        last_time = current_time;

        Dessiner();

        ellapsed_time = SDL_GetTicks() - start_time;
        if (ellapsed_time < 10)
        {
            SDL_Delay(10 - ellapsed_time);
        }

    }
    return 0;
}
