#include <vector>
#include "vecteur.h"
#include "rayon.h"
#include <algorithm>

#pragma once
class Boite
{
private:
	vector<Boite> boites;
	vector< vector<int> > triangles;
	vector< vector<int> > normaleSommet;
	vector<Vecteur>* points;
	vector<Vecteur>* normale;
	vector<double> maxPoints;
	vector<double> minPoints;
	

public:
	Boite(vector< vector<int> >, vector< vector<int> >, vector<Vecteur>*, vector<Vecteur>*, vector<double>, vector<double>, int);
	Boite();
	~Boite();
	bool pointInBoite(Vecteur);
	int indexMaxSize();
	vector<Vecteur> intersect(Rayon u);
	vector< vector<int> > getTriangles();
	Vecteur intersectPlan(Rayon u, int i);
	Vecteur barycentriqueCoord(Vecteur P, int i);
	vector< vector<int> > diffVector(vector< vector<int> >, vector< vector<int> >);
};

