#ifndef _VAR
#define _VAR


#define PI 3.1415926535898


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
bool premierefois = true;
int avancer = 0;
int strafer = 0;




//textures
GLuint texture;
int ind_texture_Mur = 2;
int ind_texture_Plafond = 1;
int ind_texture_Sol = 0;




struct coord{
	double x, y;
	};
	
coord Murs[1000];

#endif
