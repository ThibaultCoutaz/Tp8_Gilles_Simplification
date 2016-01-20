
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "Pixel.h"
#include "BEnglobante.h"
#include "Mesh.h"

/* au cas ou M_PI ne soit defini */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

#define WIDTH  600
#define HEIGHT 600

#define KEY_ESC 27

float angleX=0.0f; //angle de rotation en Y de la scene
float angleY=0.0f; //angle de rotation en X de la scene

float pasDeplacement=1.25;


//position lumiere
float xLitePos=0;
float yLitePos=10;
float zLitePos=-100;

float tx=0.0;
float ty=0.0;
float tz=0.0;


int LightPos[4] = { 15, 15, 15, 1 };

const int step = 1;
int nbCircle = 2;

Circle *tabCircle;
BEnglobante *AABB,*AABBMesh;
Mesh mesh("buddha.off");


void InitCircles(){
	tabCircle = new Circle[nbCircle];

	tabCircle[0] = Circle(point3(0, 0, 0), 2);
	tabCircle[1] = Circle(point3(2, 5, 2), 5);

	float xMax = 0, yMax = 0, zMax = 0;
	float xMin = 0, yMin = 0, zMin = 0;
	float RxMax, RyMax, RzMax, RxMin, RyMin, RzMin;

	for (int i = 0; i < nbCircle; i++){
		//*******For Verification**********//
		if (tabCircle[i].center.x == 0){
			RxMax = tabCircle[i].rayon;
			RxMin = tabCircle[i].rayon;
		}
		if (tabCircle[i].center.y == 0){
			RyMax = tabCircle[i].rayon;
			RyMin = tabCircle[i].rayon;
		}
		if (tabCircle[i].center.z == 0){
			RzMax = tabCircle[i].rayon;
			RzMin = tabCircle[i].rayon;
		}

		//*******For Max**********//
		if (tabCircle[i].center.x>xMax){
			xMax = tabCircle[i].center.x;
			RxMax = tabCircle[i].rayon;
		}
		if (tabCircle[i].center.y > yMax){
			yMax = tabCircle[i].center.y;
			RyMax = tabCircle[i].rayon;
		}
		if (tabCircle[i].center.z > zMax){
			zMax = tabCircle[i].center.z;
			RzMax = tabCircle[i].rayon;
		}

		//*******For Min**********//
		if (tabCircle[i].center.x < xMin){
			xMin = tabCircle[i].center.x;
			RxMin = tabCircle[i].rayon;
		}
		if (tabCircle[i].center.y < yMin){
			yMin = tabCircle[i].center.y;
			RyMin = tabCircle[i].rayon;
		}
		if (tabCircle[i].center.z < zMin){
			zMin = tabCircle[i].center.z;
			RzMin = tabCircle[i].rayon;
		}
	}


	float _nbStepx = (std::abs(xMin - RxMin) + (std::abs(xMax + RxMax))) / step;
	float _nbStepy = (std::abs(yMin - RyMin) + (std::abs(yMax + RyMax))) / step;
	float _nbStepz = (std::abs(zMin - RzMin) + (std::abs(zMax + RzMax))) / step;
	AABB = new BEnglobante(point3(xMin - RxMin, yMin - RyMin, zMin - RzMin), point3(xMax + RxMax, yMax + RyMax, zMax + RzMax), step, _nbStepx, _nbStepy, _nbStepz, tabCircle, nbCircle);

}

void InitMesh(){
	float xMax = mesh.Sommets[0], yMax = mesh.Sommets[1], zMax = mesh.Sommets[2];
	float xMin = mesh.Sommets[0], yMin = mesh.Sommets[1], zMin = mesh.Sommets[2];
	int index = 0;
	for (int i = 0; i < mesh.nbSommets; i++){
		//*******For Max**********//
		if (mesh.Sommets[index]>xMax){
			xMax = mesh.Sommets[index];
		}
		if (mesh.Sommets[index+1]>yMax){
			yMax = mesh.Sommets[index+1];
		}
		if (mesh.Sommets[index+2]>zMax){
			zMax = mesh.Sommets[index+2];
		}

		//*******For Min**********//
		if (mesh.Sommets[index] < xMin){
			xMin = mesh.Sommets[index];
		}
		if (mesh.Sommets[index+1] < yMin){
			yMin = mesh.Sommets[index+1];
		}
		if (mesh.Sommets[index+2] < zMin){
			zMin = mesh.Sommets[index+2];
		}

		index += 3;
	}
	cout << "xMax : " << xMax << endl;
	cout << "yMax : " << yMax << endl;
	cout << "zMax : " << zMax << endl;

	cout << "xMin : " << xMin << endl;
	cout << "yMin : " << yMin << endl;
	cout << "zMin : " << zMin << endl;

	float _nbStepx = (std::abs(xMin - 1) + (std::abs(xMax + 1))) / step;
	float _nbStepy = (std::abs(yMin - 1) + (std::abs(yMax + 1))) / step;
	float _nbStepz = (std::abs(zMin - 1) + (std::abs(zMax + 1))) / step;
	AABBMesh = new BEnglobante(point3(xMin - 1, yMin - 1, zMin - 1), point3(xMax + 1, yMax + 1, zMax + 1), step, _nbStepx,_nbStepy,_nbStepz);

}

/* initialisation d'OpenGL*/
static void init()
{
	glEnable(GL_DEPTH_TEST);	// Active le test de profondeur
	glEnable(GL_LIGHTING);	// Active l'éclairage
	//glEnable(GL_LIGHT0);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	//glLightiv(GL_LIGHT0, GL_POSITION, LightPos);
	glEnable(GL_COLOR_MATERIAL);
	// Si vous avez des choses à initialiser, c est ici.

	//InitCircles();
	InitMesh();
}



/* Dessin */
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //effacement du buffer

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	float rotate = glutGet(GLUT_ELAPSED_TIME) / (50 % 360);
	//Description de la scenev
	glLoadIdentity();
	//rotation de la scene suivant les mouvements de la souris
	//glRotatef(-angleY,0.0f,0.0f,1.0f);
	//glRotatef(angleX,0.0f,1.0f,0.0f);
	//glTranslatef(tx,ty,tz);

	//glRotatef(-angleY,0.0f,0.0f,1.0f);
	//glRotatef(angleX,0.0f,1.0f,0.0f);

	glRotated(rotate, 0, 1, 1);
	//AABB->DrawEB(point3(1, 0, 1));

	//glScalef(15, 15, 15);
	mesh.DrawMesh();
	AABBMesh->DrawMesh();
	//AABB->DrawCircles(true);


	//affiche les axes du repere
		glColor3f(0.0,1.0,0.0); //Y vert
		glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(0,20,0);
		glEnd();

		glColor3f(0.0,0.0,1.0); //Z bleu
		glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(0,0,20);
		glEnd();
		
		glColor3f(1.0,0.0,0.0); //X rouge
		glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(20,0,0);
		glEnd();


		glFlush();
		glutSwapBuffers();
}



/* Au cas ou la fenetre est modifiee ou deplacee */
void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-20, 20, -20, 20, -10, 10);
   glMatrixMode(GL_MODELVIEW);
   //glLoadIdentity();
}

//GLvoid gestionSouris(int x, int y)
//{
//	angleX=x*720/WIDTH; //gere l'axe Oy
//	angleY=-(y*180/HEIGHT-90)*4; //gere l'axe Ox
//
//	glutPostRedisplay();
//}

GLvoid gestionFleche(int key, int x, int y) 
{
	switch (key) {
		case GLUT_KEY_UP :
			tz+=int(pasDeplacement);glutPostRedisplay();
		break;

		case GLUT_KEY_DOWN :
			tz-=int(pasDeplacement);glutPostRedisplay();
		break;
	glutPostRedisplay();
	}

}

GLvoid window_key_down(unsigned char key, int x, int y)  //appuie des touches
{  
switch (key) 
{

//deplacement de la camera
	case 'z':
		ty+=int(pasDeplacement);glutPostRedisplay();
	break;

	case 's':
		ty-=int(pasDeplacement);glutPostRedisplay();
	break;

	case 'q':
		tx-=int(pasDeplacement);glutPostRedisplay();
	break;

	case 'd':
		tx+=int(pasDeplacement);glutPostRedisplay();
	break;

//sortie
	case KEY_ESC:
		//exit(0);
		break;

	default:
		printf ("La touche %d non active.\n", key);
	break;
	}
glutPostRedisplay();
}

GLvoid window_idle()
{

	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	srand(time(NULL));
   glutInitWindowSize(1000, 1000);
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutCreateWindow("Mon cylindre");
   init();
   glutReshapeFunc(reshape);
   //glutKeyboardFunc(&window_key_down);
   glutDisplayFunc(display);
   glutIdleFunc(&window_idle);
   //glutPassiveMotionFunc(gestionSouris);
   //glutSpecialFunc(&gestionFleche);
   glutMainLoop();
   return 0;
}
