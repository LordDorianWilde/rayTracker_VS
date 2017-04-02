#include "Objet.h"
#include "string"
#include <iostream>
#include <fstream>
#include <sstream>
#include "vecteur.h"
#include "Boite.h"
#include <algorithm>

#pragma once
class Mesh : public Objet
{
private:
	Vecteur centre;
	double color[3];
	bool mirroir;
	bool transparent;
	double indiceRefract;
	double emission;
	vector< vector<int> > triangles;
	vector< vector<int> > normaleSommet;
	vector<Vecteur> points;
	vector<Vecteur> normale;
	vector<double> maxPoints;
    vector<double> minPoints;
	Boite boite;
	int maxTriangleOnSommet;

public:
	Mesh();
	vector<string> split(string str, char delimiter);
	void parseFichier(string, double);
	Vecteur normal(Vecteur p);
	vector<Vecteur> intersect(Rayon u);
    Mesh(string, double, Vecteur, double, double, double, bool, bool, double, double);
	~Mesh();
    Vecteur getCentre();
    double getColor(int);
    bool getMirroir();
    bool getTransparent();
    double getIndiceRefract();
    double getEmission();
};

