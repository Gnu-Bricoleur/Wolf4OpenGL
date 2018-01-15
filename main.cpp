#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
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
double positionX = 10;
double positionY = 10;
float distCollision = 0.1;

bool premierefois = true;
int avancer = 0;
int strafer = 0;
float frame = 0;
int etat_lampeRouge = 0;


//textures
GLuint texture;
int ind_texture_Drapeau = 2;
int ind_texture_Tableau = 1;
int ind_texture_Mur = 0;
int ind_texture_Brique = 3;// mur brique
int ind_texture_Briue_Couronne = 4;// mur brique couronne
int ind_texture_Bois = 5;// mur bois
int ind_texture_Bois_tableau = 6;// mur bois tableau
int ind_texture_Carrelage = 7;// carrelage
int ind_obj_Poulet = 10;
int ind_obj_Col = 9;
int ind_obj_Squel = 11;
int ind_obj_Table = 8;

struct coord{
	double x, y;
	int type;
	};
	
coord Murs[10000];
coord ennemis[100];
coord objets[1000];
coord Lampes[100];
coord Lampes_rouge[100];

void Cube(int x, int y, int z, int r, int g, int b, int ind_texture);
void Chargement_niveau();
//void Ini_Texture();
void Dessiner();
void Construction_niveau();
double modulo( double angle );
void nazi(float nazix, float naziy);
void objet(float posx, float posy, int type);


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
    SDL_ShowCursor(SDL_DISABLE);
	SDL_WarpMouse(320, 240);
    glEnable(GL_TEXTURE_2D);

    texture = loadTexture("images/texturesf.png");
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
	
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
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
					//angleZ += angleZ -(320 - (event.motion.x));
					angleZ += event.motion.x;
					/*
					//Supprimer la premiere valeur bizarze
					if (premierefois == true)
					{
							angleZ = 0;
							premierefois = false;
					}
					*/
					//angleZ = fmod(angleZ, 360);
					
					angleZ = (angleZ/640)*360;//conversion resolution degree
					//cout<<angleZ<<endl;
					if(angleZ <= 1)
					{
						SDL_WarpMouse(639, 240);
					}
					if(angleZ >= 360)
					{
						SDL_WarpMouse(1, 240);
					}
					angleZ = (angleZ/180)*PI;//conversion degre radian
					
					//cout<<event.motion.xrel<<endl;
										
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
    
    double deplacement =  sqrt(deplacementX*deplacementX + deplacementY*deplacementY);
    
    int collision = 0;
	//if ( premierefois == true)
	//{
		//gluLookAt(5,5,1,5+ 0.1 * sin(angleZ),5 + 0.1 * cos(angleZ) ,5,5,5,1);
		//premierefois = false;
	//}
    
    
    if (avancer == 1)
    {
		for(int i  = 0 ; i<10000 ; i++)
		{
			if ( (abs((positionX + deplacement * sin(angleZ)) - (Murs[i].x + 0.5)) < distCollision) || (abs((positionX + deplacement * sin(angleZ)) - (Murs[i].x - 0.5)) < distCollision)) //si distance faible en x
			{	
				if ((abs((positionY + deplacement * cos(angleZ)) - (Murs[i].y + 0.5)) < distCollision) || (abs((positionY + deplacement * cos(angleZ)) - (Murs[i].y - 0.5)) < distCollision))//si distance faible en y
				{	
					collision = 1;
					cout<<Murs[i].x<<endl; //mur qui prvoque la collision
					cout<<Murs[i].y<<endl;
					break;
				}
			}
		
		}
		if(collision == 0)
		{
			positionX += deplacement * sin(angleZ);
			positionY += deplacement * cos(angleZ);
		}
		cout<<collision<<endl;	
		
		//positionX += deplacement * sin(angleZ);
		//positionY += deplacement * cos(angleZ);
	}
	else if (avancer == -1)
	{
		for(int i  = 0 ; i<10000 ; i++)
		{
			if ( (abs((positionX - deplacement * sin(angleZ)) - (Murs[i].x + 0.5)) < distCollision) || (abs((positionX - deplacement * sin(angleZ)) - (Murs[i].x - 0.5)) < distCollision)) //si distance faible en x
			{	
				if ((abs((positionY - deplacement * cos(angleZ)) - (Murs[i].y + 0.5)) < distCollision) || (abs((positionY - deplacement * cos(angleZ)) - (Murs[i].y - 0.5)) < distCollision))//si distance faible en y
				{	
					collision = 1;
					cout<<Murs[i].x<<endl; //mur qui prvoque la collision
					cout<<Murs[i].y<<endl;
					break;
				}
			}
		
		}
		if(collision == 0)
		{
			positionX -= deplacement * sin(angleZ);
			positionY -= deplacement * cos(angleZ);
		}

		//positionX -= deplacement * sin(angleZ);
		//positionY -= deplacement * cos(angleZ);
	}
	
	if (strafer == 1)
    {
		for(int i  = 0 ; i<10000 ; i++)
		{
			if ( (abs((positionX + deplacement * sin(angleZ + PI/2)) - (Murs[i].x + 0.5)) < distCollision) || (abs((positionX + deplacement * sin(angleZ + PI/2)) - (Murs[i].x - 0.5)) < distCollision)) //si distance faible en x
			{	
				if ((abs((positionY + deplacement * cos(angleZ + PI/2)) - (Murs[i].y + 0.5)) < distCollision) || (abs((positionY + deplacement * cos(angleZ + PI/2)) - (Murs[i].y - 0.5)) < distCollision))//si distance faible en y
				{	
					collision = 1;
					cout<<Murs[i].x<<endl; //mur qui prvoque la collision
					cout<<Murs[i].y<<endl;
					break;
				}
			}
		
		}
		if(collision == 0)
		{
			positionX += deplacement * sin(angleZ+ PI/2);
			positionY += deplacement * cos(angleZ+ PI/2);
		}
		
		
		//positionX += deplacement * sin(angleZ+ PI/2);
		//positionY += deplacement * cos(angleZ+ PI/2);
	}
	else if (strafer == -1)
	{
		
		for(int i  = 0 ; i<10000 ; i++)
		{
			if ( (abs((positionX - deplacement * sin(angleZ+ PI/2)) - (Murs[i].x + 0.5)) < 0.5) || (abs((positionX - deplacement * sin(angleZ+ PI/2)) - (Murs[i].x - 0.5)) < 0.5)) //si distance faible en x
			{	
				if ((abs((positionY - deplacement * cos(angleZ+ PI/2)) - (Murs[i].y + 0.5)) < 0.5) || (abs((positionY - deplacement * cos(angleZ+ PI/2)) - (Murs[i].y - 0.5)) < 0.5))//si distance faible en y
				{	
					collision = 1;
					cout<<Murs[i].x<<endl; //mur qui prvoque la collision
					cout<<Murs[i].y<<endl;
					break;
				}
			}
		
		}
		if(collision == 0)
		{
			positionX -= deplacement * sin(angleZ+ PI/2);
			positionY -= deplacement * cos(angleZ+ PI/2);
		}
		
		//positionX -= deplacement * sin(angleZ+ PI/2);
		//positionY -= deplacement * cos(angleZ+ PI/2);
	}
	
	collision = 0;
    gluLookAt(positionX,positionY,1,positionX + 0.1 * sin(angleZ),positionY + 0.1 * cos(angleZ) ,1,0,0,1);
	
	//positionXini += deplacementX;
	deplacementX = 0;
	//positionYini += deplacementY;
	deplacementY = 0;


    //glRotated(angleZ,0,0,1);
    //glRotated(angleX,1,0,0);
    //glRotated(angleY,0,1,0);
    //glTranslated(positionX, positionY, 1);

    glBegin(GL_QUADS);
    
    glUseProgram(programID); //active l'utilisation du shader (remettre cette fonctiona avec un autre shader pour changer en cours de dessin)

	
	Construction_niveau(); //Construit vertex en suivant de la carte chargé
	
	float cross[4][4] = {
		{-0.05, 0, 0, 13},
		{+0.05, 0, 0, 13},
		{0, -0.05, 0, 13},
		{0, +0.05, 0, 13},
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof cross, cross, GL_DYNAMIC_DRAW);
	//glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_LINES, 0, 4);

	
	
    glEnd();

    glFlush();
    SDL_GL_SwapBuffers();
}


void Cube(int x, int y, int z, int r, int g, int b, int ind_texture)
{   
    //<<texture<<endl;
    //cout<<"    "<<endl;
    //GLuint text = 1;
    float u = 0.0;
    float v = 0.0;
    float w = 0.0;
    float t = 0.0;
    switch (ind_texture)
	{
		case 0://mur pierre gris
			u = 1/12.0;//droite
			v = 0.0;//gauche
			w = 1-0.0;//haut
			t = 1-1/19.0;//bas
            break;
		case 2://mur pierre drapeau
			u = 5/12.0; 
			v = 4/12.0;
			w = 1-1/19.0;
			t = 1-0.0;
			break;
		case 1:// mur pierre tableau
			u = 1/12.0;
			v = 0.0;
			w = 1-2/19.0;
			t = 1-1/19.0;
			break;
		case 3:// mur brique
			u = 3/12.0;
			v = 2/12.0;
			w = 1-6/19.0;
			t = 1-5/19.0;
			break;
		case 4:// mur brique couronne
			u = 5/12.0;
			v = 4/12.0;
			w = 1-6/19.0;
			t = 1-5/19.0;
			break;
		case 5:// mur bois
			u = 5/12.0-0.01;
			v = 4/12.0+0.01;
			w = 1-4/19.0+0.01;
			t = 1-3/19.0-0.01;
			break;
		case 6:// mur bois tableau
			u = 1/12.0;
			v = 0.0;
			w = 1-4/19.0;
			t = 1-3/19.0;
			break;
		case 7:// carrelage
			u = 2/12.0;
			v = 1/12.0;
			w = 1-1/19.0;
			t = 1-0/19.0;
			break;
   }

    glBindTexture(GL_TEXTURE_2D, texture);
	glColor3ub(r, g, b);
    glColor3ub(255,255,255);
    //glDisable(GL_LIGHTING);

    glTexCoord2d(v,t); glVertex3d(x+0.5, y+0.5, z+0.5);
    glTexCoord2d(v,w); glVertex3d(x+0.5, y+0.5, z-0.5);
    glTexCoord2d(u,w); glVertex3d(x-0.5, y+0.5, z-0.5);
    glTexCoord2d(u,t); glVertex3d(x-0.5, y+0.5, z+0.5);

    glTexCoord2d(v,t); glVertex3d(x+0.5, y-0.5, z+0.5);
    glTexCoord2d(v,w); glVertex3d(x+0.5, y-0.5, z-0.5);
    glTexCoord2d(u,w); glVertex3d(x+0.5, y+0.5, z-0.5);
    glTexCoord2d(u,t); glVertex3d(x+0.5, y+0.5, z+0.5);

    glTexCoord2d(v,t); glVertex3d(x-0.5, y-0.5, z+0.5);
    glTexCoord2d(v,w); glVertex3d(x-0.5, y-0.5, z-0.5);
    glTexCoord2d(u,w); glVertex3d(x+0.5, y-0.5, z-0.5);
    glTexCoord2d(u,t); glVertex3d(x+0.5, y-0.5, z+0.5);

    glTexCoord2d(v,t);glVertex3d(x-0.5, y+0.5, z+0.5);
    glTexCoord2d(v,w);glVertex3d(x-0.5, y+0.5, z-0.5);
    glTexCoord2d(u,w);glVertex3d(x-0.5, y-0.5, z-0.5);
    glTexCoord2d(u,t);glVertex3d(x-0.5, y-0.5, z+0.5);

    glTexCoord2d(v,t);glVertex3d(x+0.5, y+0.5, z-0.5);
    glTexCoord2d(v,w);glVertex3d(x+0.5, y-0.5, z-0.5);
    glTexCoord2d(u,w);glVertex3d(x-0.5, y-0.5, z-0.5);
    glTexCoord2d(u,t);glVertex3d(x-0.5, y+0.5, z-0.5);

    glTexCoord2d(v,t);glVertex3d(x+0.5, y-0.5, z+0.5);
    glTexCoord2d(v,w);glVertex3d(x+0.5, y+0.5, z+0.5);
    glTexCoord2d(u,w);glVertex3d(x-0.5, y+0.5, z+0.5);
    glTexCoord2d(u,t); glVertex3d(x-0.5, y-0.5, z+0.5);
    //glDisable(GL_TEXTURE_2D);
    
    
   
	
}

void Chargement_niveau()
{
    // Chargement niveau
    FILE* fichier = NULL;
    int caractereActuel = 0;
	int indexennemis = 0;
    fichier = fopen("map.lvl", "r");
    if(fichier != NULL)  // si l'ouverture a réussi
        {           
                fscanf(fichier, "%d %d", &largeur, &hauteur);
                cout << largeur << endl;
                cout << hauteur << endl;
                for (int i =0; i < largeur; i++)
                {
                        for (int j = 0; j < hauteur+1; j++)
                        {
                            caractereActuel = fgetc(fichier);
                            if (caractereActuel == '#')
                            {
                                Murs[indexlu].x = i;
                                Murs[indexlu].y = j;
                                Murs[indexlu].type = ind_texture_Mur;
                                indexlu += 1;
                            }
                            else if (caractereActuel == 't')
                            {
								Murs[indexlu].x = i;
                                Murs[indexlu].y = j;
                                Murs[indexlu].type = ind_texture_Tableau;
                                indexlu += 1;
							}
                            else if (caractereActuel == 'd')
                            {
								Murs[indexlu].x = i;
                                Murs[indexlu].y = j;
                                Murs[indexlu].type = ind_texture_Drapeau;
                                indexlu += 1;
							}
							else if (caractereActuel == 'b')
                            {
								Murs[indexlu].x = i;
                                Murs[indexlu].y = j;
                                Murs[indexlu].type = ind_texture_Bois;
                                indexlu += 1;
							}
							else if (caractereActuel == 'a')
                            {
								Murs[indexlu].x = i;
                                Murs[indexlu].y = j;
                                Murs[indexlu].type = ind_texture_Bois_tableau;
                                indexlu += 1;
							}
							else if (caractereActuel == 'c')
                            {
								Murs[indexlu].x = i;
                                Murs[indexlu].y = j;
                                Murs[indexlu].type = ind_texture_Brique;
                                indexlu += 1;
							}
							else if (caractereActuel == 'w')
                            {
								Murs[indexlu].x = i;
                                Murs[indexlu].y = j;
                                Murs[indexlu].type = ind_texture_Briue_Couronne;
                                indexlu += 1;
							}
							else if (caractereActuel == 'z')//table
                            {
								objets[indexlu].x = i;
                                objets[indexlu].y = j;
                                objets[indexlu].type = ind_obj_Table;
                                indexlu += 1;
							}
							else if (caractereActuel == 'e')//colonne
                            {
								objets[indexlu].x = i;
                                objets[indexlu].y = j;
                                objets[indexlu].type = ind_obj_Col;
                                indexlu += 1;
							}
							else if (caractereActuel == 'r')//poulet
                            {
								objets[indexlu].x = i;
                                objets[indexlu].y = j;
                                objets[indexlu].type = ind_obj_Poulet;
                                indexlu += 1;
							}
							else if (caractereActuel == 'y')//squellette
                            {
								objets[indexlu].x = i;
                                objets[indexlu].y = j;
                                objets[indexlu].type = ind_obj_Squel;
                                indexlu += 1;
							}
                            else if (caractereActuel == 'n')
                            {
								ennemis[indexennemis].x = i;
								ennemis[indexennemis].y = j;
								indexennemis += 1;
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
		if (Murs[i].type == ind_texture_Tableau)
		{
			Cube(Murs[i].x, Murs[i].y, 0, 125, 0, 0, ind_texture_Mur);
			Cube(Murs[i].x, Murs[i].y, 1, 125, 0, 0, Murs[i].type); 
			Cube(Murs[i].x, Murs[i].y, 2, 125, 0, 0, ind_texture_Mur);
		}
		else
		{
			Cube(Murs[i].x, Murs[i].y, 0, 125, 0, 0, Murs[i].type);
			Cube(Murs[i].x, Murs[i].y, 1, 125, 0, 0, Murs[i].type); 
			Cube(Murs[i].x, Murs[i].y, 2, 125, 0, 0, Murs[i].type);
		}
	}
	
    for (int i = 0; i<largeur; i++) 
    {
        for (int j = 0; j<hauteur; j++)
        {

            Cube(i,j,-1,128,128,128, ind_texture_Bois);
            Cube(i,j,3,50,128,50, ind_texture_Carrelage);
        }

    }
    
    for (int i =0; i< 100; i++)
    {
		nazi(ennemis[i].x, ennemis[i].y);
	}
    frame+=0.07;
	if (frame > 17)
	{
		frame = 0;
	}
	
	for (int i =0; i< 1000; i++)
    {
		objet(objets[i].x, objets[i].y, objets[i].type);
	}
}


void nazi(float nazix, float naziy)
{
	float u = 0.0;
    float v = 0.0;
    float w = 0.0;
    float t = 0.0;
    switch ((int)frame)
	{
		case 0:
			u = 8/12.0;
			v = 7/12.0;
			w = 1-0.0;
			t = 1-1/19.0;
            break;
		case 1:
			u = 8/12.0; 
			v = 7/12.0;
			w = 1-1/19.0;
			t = 1-2/19.0;
			break;
		case 2:
			u = 8/12.0;
			v = 7/12.0;
			w = 1-2/19.0;
			t = 1-3/19.0;
			break;
		case 3:
			u = 8/12.0;
			v = 7/12.0;
			w = 1-3/19.0;
			t = 1-4/19.0;
			break;
		case 4:
			u = 8/12.0;
			v = 7/12.0;
			w = 1-4/19.0;
			t = 1-5/19.0;
			break;
		case 5:
			u = 8/12.0;
			v = 7/12.0;
			w = 1-5/19.0;
			t = 1-6/19.0;
			break;
		case 6:
			u = 8/12.0;
			v = 7/12.0;
			w = 1-6/19.0;
			t = 1-7/19.0;
			break;
		case 7:
			u = 8/12.0;
			v = 7/12.0;
			w = 1-7/19.0;
			t = 1-8/19.0;
			break;
		case 8:
			u = 8/12.0;
			v = 7/12.0;
			w = 1-8/19.0;
			t = 1-9/19.0;
			break;
		case 9:
			u = 8/12.0;
			v = 7/12.0;
			w = 1-9/19.0;
			t = 1-10/19.0;
			break;
		case 10:
			u = 8/12.0;
			v = 7/12.0;
			w = 1-10/19.0;
			t = 1-11/19.0;
			break;
		case 11:
			u = 8/12.0;
			v = 7/12.0;
			w = 1-11/19.0;
			t = 1-12/19.0;
			break;
		case 12:
			u = 8/12.0;
			v = 7/12.0;
			w = 1-12/19.0;
			t = 1-13/19.0;
			break;
		case 13:
			u = 8/12.0;
			v = 7/12.0;
			w = 1-13/19.0;
			t = 1-14/19.0;
			break;
		case 14:
			u = 8/12.0;
			v = 7/12.0;
			w = 1-14/19.0;
			t = 1-15/19.0;
			break;
		case 15:
			u = 8/12.0;
			v = 7/12.0;
			w = 1-15/19.0;
			t = 1-16/19.0;
			break;
		case 16:
			u = 8/12.0;
			v = 7/12.0;
			w = 1-16/19.0;
			t = 1-17/19.0;
			break;
		case 17:
			u = 8/12.0;
			v = 7/12.0;
			w = 1-17/19.0;
			t = 1-18/19.0;
			break;
   }
	glBindTexture(GL_TEXTURE_2D, texture);//A corriger
    glBegin(GL_QUADS);
    glTexCoord2d(v,w);  glVertex3d(nazix+1*sin(angleZ+PI/2), naziy + 1*cos(angleZ+PI/2) ,2);
    glTexCoord2d(v,t);  glVertex3d(nazix+1*sin(angleZ+PI/2), naziy + 1*cos(angleZ+PI/2) ,-0.5);
    glTexCoord2d(u,t);  glVertex3d(nazix+1*sin(angleZ+PI/2), naziy - 1*cos(angleZ+PI/2) ,-0.5);
    glTexCoord2d(u,w);  glVertex3d(nazix+1*sin(angleZ+PI/2), naziy - 1*cos(angleZ+PI/2) ,2);
    glEnd();
   
}




void objet(float posx, float posy, int type)
{
	float u = 0.0;
    float v = 0.0;
    float w = 0.0;
    float t = 0.0;
    switch (type)
	{
		case 8:
			u = 7/12.0;
			v = 6/12.0;
			w = 1-0.0;
			t = 1-1/19.0;
            break;
		case 9:
			u = 7/12.0; 
			v = 6/12.0;
			w = 1-1/19.0;
			t = 1-2/19.0;
			break;
		case 10:
			u = 7/12.0;
			v = 6/12.0;
			w = 1-2/19.0;
			t = 1-3/19.0;
			break;
		case 20:
			u = 7/12.0;
			v = 6/12.0;
			w = 1-3/19.0;
			t = 1-4/19.0;
			break;
		case 11:
			u = 7/12.0;
			v = 6/12.0;
			w = 1-4/19.0;
			t = 1-5/19.0;
			break;
		}
	glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    glTexCoord2d(v,w);  glVertex3d(posx+1*sin(angleZ+PI/2), posy + 1*cos(angleZ+PI/2) ,2);
    glTexCoord2d(v,t);  glVertex3d(posx+1*sin(angleZ+PI/2), posy + 1*cos(angleZ+PI/2) ,-0.5);
    glTexCoord2d(u,t);  glVertex3d(posx+1*sin(angleZ+PI/2), posy - 1*cos(angleZ+PI/2) ,-0.5);
    glTexCoord2d(u,w);  glVertex3d(posx+1*sin(angleZ+PI/2), posy - 1*cos(angleZ+PI/2) ,2);
    glEnd();
   
}
