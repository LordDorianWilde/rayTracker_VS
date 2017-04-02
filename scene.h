#ifndef SCENE_H
#define SCENE_H
#include "vecteur.h"
#include <vector>
#include "Objet.h"
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
    std::vector<Objet*> spheres;
	std::uniform_real_distribution<double> distribe;

    Scene(Objet*);
    void addObjet(Objet*);
    Vecteur intensityLight(int, Rayon, vector<Vecteur>, int);
	Vecteur lightPixel(Rayon, int occ);
	Vecteur lightPixel(Rayon);
};

#endif // SCENE_H
