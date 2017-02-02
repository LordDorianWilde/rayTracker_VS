#ifndef SCENE_H
#define SCENE_H
#include "vecteur.h"
#include <vector>
#include "sphere.h"
#include <math.h>
#include "rayon.h"
#include <algorithm>

using namespace std;

class Scene
{
public:
    Vecteur camera;
    Vecteur lumiere;
    double intensity;
    std::vector<Sphere> spheres;

    Scene(Vecteur);
    void addSphere(Sphere);
    double intersecSphere(Sphere, Rayon);
    bool objetBetweenHiddingLight(Sphere, Rayon);
	Vecteur intensityLight(Sphere, Rayon, double, int);
	Vecteur lightPixel(Rayon, int occ);
	Vecteur lightPixel(Rayon);
    double f(double);
    double fmult(vector<double>);
    double pdf(double, double, double);
    double pdfmult(vector<double>, double, double);
    double monteCarlo(double, double);
};

#endif // SCENE_H
