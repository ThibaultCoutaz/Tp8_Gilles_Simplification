#include "Pixel.h"
#include <string>
#include <fstream>

#pragma once
class Mesh
{
public:
	//The Array for the Values
	float *Sommets;
	int *Facettes;
	float *Normals;

	int nbSommets, nbFacettes, nbArretes;
	string type;
	float hight = 0;
	int nbApF = 0; // Nb Arretes par Face

	//Middle of the Obj
	point3 center;


	Mesh();
	Mesh(string File);
	void normalFace(point3,point3,point3,int);
	void SaveModel(string File);
	void DrawMesh();
	~Mesh();
};

