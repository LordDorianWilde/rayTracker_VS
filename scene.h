#ifndef SCENE_H
#define SCENE_H
#include "vecteur.h"
#include <vector>
#include "sphere.h"
#include <random>
#include <math.h>
#include "rayon.h"
#include <algorithm>

using namespace std;

class Scene
{
public:
	std::default_random_engine engine;
    Vecteur camera;
    double intensity;
    std::vector<Sphere> spheres;
	std::uniform_real_distribution<double> distribe;

    Scene(Sphere);
    void addSphere(Sphere);
    double intersecSphere(Sphere, Rayon);
    bool objetBetweenHiddingLight(Sphere, Rayon, double);
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
