#include "Mesh.h"


Mesh::Mesh()
{
}

Mesh::Mesh(string File){
	center = point3(0, 0, 0);

	ifstream fichier(File.c_str(), ios::in);

	if (fichier){
		cout << "Ouverture du fichier (lecture) : " << File.c_str() << endl;

		//****Lecture File****//
		fichier >> type >> nbSommets >> nbFacettes >> nbArretes;
		//cout<<nbSommets<<"//"<<nbFacettes<<endl;

		Sommets = new float[nbSommets * 3];
		Facettes = new int[nbFacettes * 3];
		Normals = new float[nbFacettes * 3];
		float x = 0;
		float y = 0;
		float z = 0;

		//To Put in the Sommets[] the value of the Sommets
		int index = 0;
		for (int i = 0; i<nbSommets; i++){
			fichier >> Sommets[index] >> Sommets[index + 1] >> Sommets[index + 2];

			x += Sommets[index];
			y += Sommets[index + 1];
			z += Sommets[index + 2];

			//Find the higher value
			if (abs(Sommets[index])>hight){
				hight = Sommets[index];
			}
			if (abs(Sommets[index + 1])>hight){
				hight = Sommets[index + 1];
			}
			if (abs(Sommets[index + 2])>hight){
				hight = Sommets[index + 2];
			}
			index += 3;
		}

		index = 0;
		//Put the value between -1 and 1
		for (int i = 0; i<nbSommets; i++){

			Sommets[index] /= hight*0.05;
			Sommets[index + 1] /= hight*0.05;
			Sommets[index + 2] /= hight*0.05;

			index += 3;

		}


		//***********PENSER A FAIRE LE CENTRAGE DU MODEL***************//
		center.x = x / nbSommets;
		center.y = y / nbSommets;
		center.z = z / nbSommets;

		//cout<<"Centre : "<<center.x<<"/"<<center.y<<"/"<<center.z<<endl;

		//To put in the Facettes[] the value of the Faces
		index = 0;
		int nb;//To put the number of Face

		for (int i = 0; i<nbFacettes; i++){
			fichier >> nbApF >> Facettes[index] >> Facettes[index + 1] >> Facettes[index + 2];

			index += 3;
		}


		cout << " Fermeture du fichier (lecture) : " << File.c_str() << endl;
		fichier.close();

	}
	else{
		cout << "Erreur d'ouverture du fichier (lecture) : " << File.c_str() << endl;
	}
}

void Mesh::normalFace(point3 _Sommets1, point3 _Sommets2, point3 _Sommets3, int _index){
	point3 v12(_Sommets2.x - _Sommets1.x, _Sommets2.y - _Sommets1.y, _Sommets2.z - _Sommets1.z);
	point3 v13(_Sommets3.x - _Sommets1.x, _Sommets3.y - _Sommets1.y, _Sommets3.z - _Sommets1.z);

	point3 normal(0., 0., 0.);
	normal.x = v12.y*v13.z - v12.z*v13.y;
	normal.y = v12.z*v13.x - v12.x*v13.z;
	normal.z = v12.x*v13.y - v12.y*v13.x;

	point3 norm = point3(normal.x, normal.y, normal.z)/**(1 / sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z))*/;

	Normals[_index] = norm.x;
	Normals[_index + 1] = norm.y;
	Normals[_index + 2] = norm.z;
}

void Mesh::SaveModel(string File){
	ofstream fichier(File, ios::out);

	if (fichier){
		cout << "Ouverture du fichier (Ecriture) : " << File.c_str() << endl;

		fichier << "OFF" << endl;
		fichier << nbSommets << " " << nbFacettes << " " << nbArretes << endl;

		//To write the x,y,z of each sommets
		int index = 0;
		for (int i = 0; i<nbSommets; i++){
			fichier << Sommets[index] * hight << " " << Sommets[index + 1] * hight << " " << Sommets[index + 2] * hight << endl;

			index += 3;
		}

		//To put in the Facettes[] the value of the Faces
		index = 0;
		int nb;//To put the number of Face

		for (int i = 0; i<nbFacettes; i++){
			fichier << nbApF << " " << Facettes[index] << " " << Facettes[index + 1] << " " << Facettes[index + 2] << endl;

			index += 3;
		}

		cout << "Fermeture du fichier (Ecriture) : " << File.c_str() << endl;
		fichier.close();
	}
	else{
		cout << "Echec Ouverture du fichier (Ecriture) : " << File.c_str() << endl;
	}
}

void Mesh::DrawMesh(){

	glPushMatrix();
	glBegin(GL_TRIANGLES);

	int index = 0;

	for (int i = 0; i<nbFacettes; i++){

		glColor3f(0.7, 0.7, 0.7);//couleur gris
		glVertex3f(Sommets[Facettes[index] * 3], Sommets[Facettes[index] * 3 + 1], Sommets[Facettes[index] * 3 + 2]);
		glVertex3f(Sommets[Facettes[index + 1] * 3], Sommets[Facettes[index + 1] * 3 + 1], Sommets[Facettes[index + 1] * 3 + 2]);
		glVertex3f(Sommets[Facettes[index + 2] * 3], Sommets[Facettes[index + 2] * 3 + 1], Sommets[Facettes[index + 2] * 3 + 2]);

		/*cout<<"sommet1 :"<<Sommets[Facettes[index]*3]<<"--"<<Sommets[Facettes[index]*3+1]<<"--"<<Sommets[Facettes[index]*3+2]<<endl;
		cout<<"sommet2 :"<<Sommets[Facettes[index+1]*3]<<"--"<<Sommets[Facettes[index+1]*3+1]<<"--"<<Sommets[Facettes[index+1]*3+2]<<endl;
		cout<<"sommet3 :"<<Sommets[Facettes[index+2]*3]<<"--"<<Sommets[Facettes[index+2]*3+1]<<"--"<<Sommets[Facettes[index+2]*3+2]<<endl;*/

		point3 Sommet1(Sommets[Facettes[index] * 3], Sommets[Facettes[index] * 3 + 1], Sommets[Facettes[index] * 3 + 2]);
		point3 Sommet2(Sommets[Facettes[index + 1] * 3], Sommets[Facettes[index + 1] * 3 + 1], Sommets[Facettes[index + 1] * 3 + 2]);
		point3 Sommet3(Sommets[Facettes[index + 2] * 3], Sommets[Facettes[index + 2] * 3 + 1], Sommets[Facettes[index + 2] * 3 + 2]);

		normalFace(Sommet1, Sommet2, Sommet3, index);

		index += 3;
	}

	glEnd();
	glPopMatrix();
}

Mesh::~Mesh()
{
	//**To Clean**//
	delete[] Sommets;
	delete[] Facettes;
}
