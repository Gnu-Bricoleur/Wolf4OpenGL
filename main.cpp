#include <SDL/SDL.h>
//#include <GL/gl.h>
#include <GL/glew.h>
#include <GL/glu.h>
//#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <vector>
#include "chargeshaders.h"
#include "sdlglutils.h" // textures
#define PI 3.1415926535898

using namespace std;

GLuint vertexshader;
GLuint fragmentshader;
GLuint program;
GLuint programID;


double angleZ = 0;
double angleX = 0;
double angleY = 0;
double deplacementX = 0;
double deplacementY = 0;
double deplacementZ = 0;
float vitesse = 0.1;
int indexlu = 0;
int largeur, hauteur;
double positionXini = 0;
double positionYini = 0;
double positionX, positionY;

//textures
GLuint texture;
int ind_texture_Mur = 2;
int ind_texture_Plafond = 1;
int ind_texture_Sol = 0;




struct coord{
	double x, y;
	};
	
coord Murs[1000];



void Cube(int x, int y, int z, int r, int g, int b, int ind_texture);
void Chargement_niveau();
//void Ini_Texture();
void Dessiner();
void Construction_niveau();
double modulo( double angle );




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

    Uint32 last_time = SDL_GetTicks();
    Uint32 current_time,ellapsed_time;
    Uint32 start_time;

	SDL_EnableKeyRepeat(10, 10);

	SDL_WM_GrabInput(SDL_GRAB_ON);									// A REMETRRE POUR UTILISER GBD CONFORTABLEMENT §§§§§§§§!!!!!!!!!!!!!!!!!!!!
    SDL_ShowCursor(SDL_DISABLE);
    //SDL_SetRelativeMouseMode(enable);

    glEnable(GL_TEXTURE_2D);

    texture = loadTexture("textures.jpg");
    

    Chargement_niveau();

	//Ini_Texture();

	//mise en place des shaders
	/*
	vertexshader = glCreateShader(GL_VERTEX_SHADER);
	LoadShader(vertexshader, "vertex.glsl");
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	LoadShader(fragmentshader, "fragment.glsl");
	program = glCreateProgram();
	
	
	
	glAttachShader(program, vertexshader);
	glAttachShader(program, fragmentshader);

	glLinkProgram(program);
	GLint isLinked = 0;
	
	glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		cout << "erreur linkage des shadeers :-/"<<endl;
		
		// The maxLength includes the NULL character
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
		
		for(int unsigned i=0;i<infoLog.size();i++)
		{
			cout << infoLog[i];
		}
	}
	*/
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
					switch(event.key.keysym.sym)
					{
						case SDLK_z:
						case SDLK_UP: // Flèche haut
							deplacementX += vitesse;
							break;
						case SDLK_s:
						case SDLK_DOWN: // Flèche bas
							deplacementX -= vitesse;
							break;
						case SDLK_d:
						case SDLK_RIGHT: // Flèche droite
							deplacementY -= vitesse;
							break;
						case SDLK_q:
						case SDLK_LEFT: // Flèche gauche
							deplacementY += vitesse;
							break;
						case SDLK_ESCAPE:
							exit(0);
					}
					break;
				case SDL_MOUSEMOTION:
					//angleZ += angleZ -(320 - (event.motion.x));
					angleZ += event.motion.xrel*10;
					//angleZ = (angleZ/180)*PI;//conversion degre radian
					//angleZ = modulo(angleZ);
				
					//cout<<angleZ<<endl;					
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


double modulo( double angle )
{
    double tot = 2.0 * PI;
    return angle - tot * floor( angle / tot );
}



void Dessiner()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

	positionX = positionXini + deplacementX;
	positionY = positionYini + deplacementY;

    gluLookAt(positionX,positionY,1,positionX + 1 + sin(angleZ),positionY + cos(angleZ),1,0,0,1);

    //glRotated(angleZ,0,0,1);
    //glRotated(angleX,1,0,0);
    //glRotated(angleY,0,1,0);
    //glTranslated(deplacementX, deplacementY, deplacementZ);

    glBegin(GL_QUADS);
    
    glUseProgram(programID); //active l'utilisation du shader (remettre cette fonctiona avec un autre shader pour changer en cours de dessin)

    
	
	Construction_niveau(); //Construit vertex en suivant de la carte chargé
	
    glEnd();

    glFlush();
    SDL_GL_SwapBuffers();
}


void Cube(int x, int y, int z, int r, int g, int b, int ind_texture)
{   
    cout<<texture<<endl;
    cout<<"    "<<endl;
    //GLuint text = 1;
    float u = 0.0;
    float v = 0.0;
    switch (ind_texture)

  {

    case 0:

        u = 0.33;
        v = 0.0;
       

       break;

    case 1:
        u = 0.66; 
        v = 0.34;
       

       break;

    case 2:
        
        u = 1.0;
        v = 0.67;

       

       break;
   }

    glBindTexture(GL_TEXTURE_2D, texture);
	glColor3ub(r, g, b);
    glColor3ub(255,255,255);
    //glDisable(GL_LIGHTING);

    glTexCoord2d(v,1); glVertex3d(x+0.5, y+0.5, z+0.5);
    glTexCoord2d(v,0); glVertex3d(x+0.5, y+0.5, z-0.5);
    glTexCoord2d(u,0); glVertex3d(x-0.5, y+0.5, z-0.5);
    glTexCoord2d(u,1); glVertex3d(x-0.5, y+0.5, z+0.5);

    glTexCoord2d(v,1); glVertex3d(x+0.5, y-0.5, z+0.5);
    glTexCoord2d(v,0); glVertex3d(x+0.5, y-0.5, z-0.5);
    glTexCoord2d(u,0); glVertex3d(x+0.5, y+0.5, z-0.5);
    glTexCoord2d(u,1); glVertex3d(x+0.5, y+0.5, z+0.5);

    glTexCoord2d(v,1); glVertex3d(x-0.5, y-0.5, z+0.5);
    glTexCoord2d(v,0); glVertex3d(x-0.5, y-0.5, z-0.5);
    glTexCoord2d(u,0); glVertex3d(x+0.5, y-0.5, z-0.5);
    glTexCoord2d(u,1); glVertex3d(x+0.5, y-0.5, z+0.5);

    glTexCoord2d(v,1);glVertex3d(x-0.5, y+0.5, z+0.5);
    glTexCoord2d(v,0);glVertex3d(x-0.5, y+0.5, z-0.5);
    glTexCoord2d(u,0);glVertex3d(x-0.5, y-0.5, z-0.5);
    glTexCoord2d(u,1);glVertex3d(x-0.5, y-0.5, z+0.5);

    glTexCoord2d(v,1);glVertex3d(x+0.5, y+0.5, z-0.5);
    glTexCoord2d(v,0);glVertex3d(x+0.5, y-0.5, z-0.5);
    glTexCoord2d(u,0);glVertex3d(x-0.5, y-0.5, z-0.5);
    glTexCoord2d(u,1);glVertex3d(x-0.5, y+0.5, z-0.5);

    glTexCoord2d(v,1);glVertex3d(x+0.5, y-0.5, z+0.5);
    glTexCoord2d(v,0);glVertex3d(x+0.5, y+0.5, z+0.5);
    glTexCoord2d(u,0);glVertex3d(x-0.5, y+0.5, z+0.5);
    glTexCoord2d(u,1); glVertex3d(x-0.5, y-0.5, z+0.5);
    //glDisable(GL_TEXTURE_2D);
	
}

void Chargement_niveau()
{
    // Chargement niveau
    FILE* fichier = NULL;
    int caractereActuel = 0;

    fichier = fopen("map.lvl", "r");
    if(fichier != NULL)  // si l'ouverture a réussi
        {           
                fscanf(fichier, "%d %d", &largeur, &hauteur);
                for (int i =0; i < largeur; i++)
                {
                        for (int j = 0; j < hauteur; j++)
                        {
                            caractereActuel = fgetc(fichier);
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
}

void Construction_niveau()
{
	for (int i = 0; i<largeur*hauteur; i++)
	{   
		Cube(Murs[i].x, Murs[i].y, 0, 125, 0, 0, ind_texture_Mur);
        Cube(Murs[i].x, Murs[i].y, 1, 125, 0, 0, ind_texture_Mur); 

	}
	
    for (int i = 0; i<largeur; i++) 
    {
        for (int j = 0; j<hauteur; j++)
        {

            Cube(i,j,-1,128,128,128, ind_texture_Sol);
            Cube(i,j,2,50,128,50, ind_texture_Plafond);
        }

    }
}
