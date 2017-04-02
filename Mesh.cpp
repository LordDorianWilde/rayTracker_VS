#include "Mesh.h"


Mesh::Mesh()
{
	centre = Vecteur();
	color[0] = 0.0;
	color[1] = 0.0;
	color[2] = 0.0;
	mirroir = false;
	transparent = false;
	indiceRefract = 1.0;
	emission = 0.0;
	maxTriangleOnSommet = 0;
}

Mesh::Mesh(string filename, double ratio, Vecteur c, double red, double green, double blue, bool mir, bool transp, double n, double em)
{
    maxPoints = {-10000,-10000,-10000};
    minPoints = {10000,10000,10000};
	centre = c;
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	mirroir = mir;
	transparent = transp;
	indiceRefract = n;
	emission = em;
	points = vector<Vecteur>();
	points.push_back(Vecteur());
	normale = vector<Vecteur>();
	normale.push_back(Vecteur());
	triangles = vector< vector<int> >();
	normaleSommet = vector< vector<int> >();
	maxTriangleOnSommet = 0;
	parseFichier(filename, ratio);

	cout << "Parsing file fait, " << points.size() << " points, " << triangles.size() << " triangles, " << maxTriangleOnSommet << " Triangle max par sommet." << endl;
	boite = Boite(triangles, normaleSommet, &points, &normale, minPoints, maxPoints, maxTriangleOnSommet);
}

Mesh::~Mesh()
{
}

void Mesh::parseFichier(string fileName, double ratio)
{
	ifstream fichier(fileName, ios::in);  // on ouvre en lecture
	vector<int> triangleSommet = vector<int>();

	if (fichier)  // si l'ouverture a fonctionné
	{
		string contenu;
		while (getline(fichier, contenu))
		{
			vector<string> parsedString = split(contenu, ' ');

			if (parsedString.size() == 0)
			{
				continue;
			}
			else if (parsedString[0] == "v")
			{
				Vecteur coord = Vecteur(stod(parsedString[1])*ratio, stod(parsedString[2])*ratio - 35, stod(parsedString[3])*ratio + 5);
				points.push_back(coord);
				triangleSommet.push_back(0);
                if(coord[0] < minPoints[0])
                {
                    minPoints[0] = coord[0];
                }
                if(coord[1] < minPoints[1])
                {
                    minPoints[1] = coord[1];
                }
                if(coord[2] < minPoints[2])
                {
                    minPoints[2] = coord[2];
                }

                if(coord[0] > maxPoints[0])
                {
                    maxPoints[0] = coord[0];
                }
                if(coord[1] > maxPoints[1])
                {
                    maxPoints[1] = coord[1];
                }
                if(coord[2] > maxPoints[2])
                {
                    maxPoints[2] = coord[2];
                }
			}
			else if (parsedString[0] == "vn")
			{
				Vecteur coord = Vecteur(stod(parsedString[1]), stod(parsedString[2]), stod(parsedString[3]));
                normale.push_back(coord);
			}
			else if (parsedString[0] == "f")
			{
				vector<int> coord = { abs(stoi(split(parsedString[1], '/')[0])), abs(stoi(split(parsedString[2], '/')[0])), abs(stoi(split(parsedString[3], '/')[0]))};
				triangles.push_back(coord);
				triangleSommet[-1*stoi(split(parsedString[1], '/')[0])] += 1;
				triangleSommet[-1*stoi(split(parsedString[2], '/')[0])] += 1;
				triangleSommet[-1*stoi(split(parsedString[3], '/')[0])] += 1;
				vector<int> norm = { stoi(split(parsedString[1], '/')[2]), stoi(split(parsedString[2], '/')[2]), stoi(split(parsedString[3], '/')[2]) };
				normaleSommet.push_back(norm);
			}
				
		}

		fichier.close();
	}
	else
		cerr << "Impossible d'ouvrir le fichier !" << endl;

	fichier.close();

	for (int i = 0; i < triangleSommet.size(); i++)
	{
		if (maxTriangleOnSommet < triangleSommet[i])
		{
			maxTriangleOnSommet = triangleSommet[i];
		}
	}
}

vector<string> Mesh::split(string str, char delimiter) {
	vector<string> internal;
	stringstream ss(str); // Turn the string into a stream.
	string tok;

	while (getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}

	return internal;
}

Vecteur Mesh::normal(Vecteur p)
{
	return p;
}

vector<Vecteur> Mesh::intersect(Rayon u)
{
	vector<Vecteur> rep = boite.intersect(u);
	
	return rep;
}

Vecteur Mesh::getCentre()
{
    return centre;
}

double Mesh::getColor(int i)
{
    return color[i];
}

bool Mesh::getMirroir()
{
    return mirroir;
}

bool Mesh::getTransparent()
{
    return transparent;
}

double Mesh::getIndiceRefract()
{
    return indiceRefract;
}

double Mesh::getEmission()
{
    return emission;
}
