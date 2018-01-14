#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <vector>
#include "variables.h"
#include "fonctions.h"

using namespace std;

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
    
    if (avancer == 1)
    {
		positionX += deplacement * sin(angleZ);
		positionY += deplacement * cos(angleZ);
	}
	else if (avancer == -1)
	{
		positionX -= deplacement * sin(angleZ);
		positionY -= deplacement * cos(angleZ);
	}
	
	if (strafer == 1)
    {
		positionX += deplacement * sin(angleZ);
		positionY += deplacement * cos(angleZ);
	}
	else if (strafer == -1)
	{
		positionX -= deplacement * sin(angleZ);
		positionY -= deplacement * cos(angleZ);
	}
	
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
