#ifndef VECTEUR_H
#define VECTEUR_H
#include <math.h>
#include <cstdlib>

using namespace std;

class Vecteur
{
public:
    double xyz[3];

    Vecteur(double, double, double);
    Vecteur();
    void normalize();
    double squareNorm();
    Vecteur cross(Vecteur);
    double &operator[](int);
    Vecteur operator+ (Vecteur);
    double operator* (Vecteur);
    Vecteur operator* (double);
    Vecteur operator- (Vecteur);
    Vecteur refract(Vecteur, double, double);
    Vecteur random(Vecteur);
};

#endif // VECTEUR_H
