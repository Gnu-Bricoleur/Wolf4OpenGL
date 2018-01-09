#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
//#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

void Dessiner();

double angleZ = 0;
double angleX = 0;
double angleY = 0;
double deplacementX = 0;
double deplacementY = 0;
double deplacementZ = 0;
float vitesse = 0.1;
int indexlu = 0;


struct coord{
	double x, y;
	};
	
coord Murs[1000];

void Cube(int x, int y, int z, int r, int g, int b);

int main(int argc, char *argv[])
{
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);
    
    freopen( "CON", "w", stdout );
	freopen( "CON", "w", stderr );
    
    atexit(SDL_Quit);
    SDL_WM_SetCaption("SDL GL Application", NULL);
    SDL_SetVideoMode(640, 480, 32, SDL_OPENGL);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(70,(double)640/480,1,1000);

    glEnable(GL_DEPTH_TEST);

    Dessiner();

    Uint32 last_time = SDL_GetTicks();
    Uint32 current_time,ellapsed_time;
    Uint32 start_time;

	SDL_EnableKeyRepeat(10, 10);


	// Chargement niveau
	FILE* fichier = NULL;
    int caractereActuel = 0;

    fichier = fopen("map.lvl", "r");
	if(fichier != NULL)  // si l'ouverture a réussi
        {			
				int largeur, hauteur;
				//fscanf(fichier, "%d %d", &largeur, &hauteur);
				hauteur = 20;
				largeur = 20;
				for (int i =0; i < largeur; i++)
				{
						for (int j = 0; j < hauteur; j++)
						{
							caractereActuel = fgetc(fichier);
							//caractereActuel = '#';
							if (caractereActuel == '#')
							{
								Murs[indexlu].x = i;
								Murs[indexlu].y = j;
								indexlu += 1;
							}
						}
				}
				
                fclose(fichier);  // on ferme le fichier
        }
        else
                cout << "Impossible d'ouvrir le fichier !" << endl;

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
					switch(event.key.keysym.sym)
					{
						case SDLK_UP: // Flèche haut
							deplacementY += vitesse;
							break;
						case SDLK_DOWN: // Flèche bas
							deplacementY -= vitesse;
							break;
						case SDLK_RIGHT: // Flèche droite
							deplacementX += vitesse;
							break;
						case SDLK_LEFT: // Flèche gauche
							deplacementX -= vitesse;
							break;
					}
					break;
				case SDL_MOUSEMOTION:
					angleZ = event.motion.x/3;
					break;
            }
        }

		//SDL_WarpMouse(320, 240);
		
        current_time = SDL_GetTicks();
        ellapsed_time = current_time - last_time;
        last_time = current_time;


        //deplacementX += 0.0005 * ellapsed_time;

        Dessiner();

        ellapsed_time = SDL_GetTicks() - start_time;
        if (ellapsed_time < 10)
        {
            SDL_Delay(10 - ellapsed_time);
        }

    }

    return 0;
}

void Dessiner()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

    gluLookAt(3,4,2,0,0,0,0,0,1);

    glRotated(angleZ,0,0,1);
    glRotated(angleX,1,0,0);
    glRotated(angleY,0,1,0);
    glTranslated(deplacementX, deplacementY, deplacementZ);

    glBegin(GL_QUADS);

    Cube(0, 0, 0, 0, 0, 255);
    Cube(1, 1, 1, 125, 125, 0);
	
	for (int i = 0; i<200; i++)
	{
		Cube(Murs[i].x, Murs[i].y, 0, 125, 0, 0);
	}
	
    glEnd();

    glFlush();
    SDL_GL_SwapBuffers();
}


void Cube(int x, int y, int z, int r, int g, int b)
{
	glColor3ub(r, g, b);
    glVertex3d(x+0.5, y+0.5, z+0.5);
    glVertex3d(x+0.5, y+0.5, z-0.5);
    glVertex3d(x-0.5, y+0.5, z-0.5);
    glVertex3d(x-0.5, y+0.5, z+0.5);

    glVertex3d(x+0.5, y-0.5, z+0.5);
    glVertex3d(x+0.5, y-0.5, z-0.5);
    glVertex3d(x+0.5, y+0.5, z-0.5);
    glVertex3d(x+0.5, y+0.5, z+0.5);

    glVertex3d(x-0.5, y-0.5, z+0.5);
    glVertex3d(x-0.5, y-0.5, z-0.5);
    glVertex3d(x+0.5, y-0.5, z-0.5);
    glVertex3d(x+0.5, y-0.5, z+0.5);

    glVertex3d(x-0.5, y+0.5, z+0.5);
    glVertex3d(x-0.5, y+0.5, z-0.5);
    glVertex3d(x-0.5, y-0.5, z-0.5);
    glVertex3d(x-0.5, y-0.5, z+0.5);

    glVertex3d(x+0.5, y+0.5, z-0.5);
    glVertex3d(x+0.5, y-0.5, z-0.5);
    glVertex3d(x-0.5, y-0.5, z-0.5);
    glVertex3d(x-0.5, y+0.5, z-0.5);

    glVertex3d(x+0.5, y-0.5, z+0.5);
    glVertex3d(x+0.5, y+0.5, z+0.5);
    glVertex3d(x-0.5, y+0.5, z+0.5);
    glVertex3d(x-0.5, y-0.5, z+0.5);
	
}
