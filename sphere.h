#ifndef SPHERE_H
#define SPHERE_H
#include "vecteur.h"
#include "string"

using namespace std;

class Sphere
{
public:
    Vecteur centre;
    double rayon;
    double color[3];
    bool mirroir;
    bool transparent;
    double indiceRefract;

    Sphere(Vecteur, double, double, double, double, bool, bool, double);
    Sphere(Vecteur, double, string, bool, bool, double);
    bool operator!= (Sphere);
};

#endif // SPHERE_H
