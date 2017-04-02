#ifndef SPHERE_H
#define SPHERE_H
#include "vecteur.h"
#include "string"
#include "Objet.h"
#include "rayon.h"

using namespace std;

class Sphere : public Objet
{
private:
    Vecteur centre;
    double rayon;
    double color[3];
    bool mirroir;
    bool transparent;
    double indiceRefract;
    double emission;

public:
    Sphere();
    Sphere(Vecteur, double, double, double, double, bool, bool, double, double);
    Sphere(Vecteur, double, string, bool, bool, double, double);
	vector<Vecteur> intersect(Rayon);
    bool operator!= (Sphere);
	Vecteur normal(Vecteur p);
    Vecteur getCentre();
    double getColor(int);
    bool getMirroir();
    bool getTransparent();
    double getIndiceRefract();
    double getEmission();
};

#endif // SPHERE_H
