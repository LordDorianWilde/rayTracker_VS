#include "rayon.h"
#include "vecteur.h"

#pragma once
class Objet
{
private:
    Vecteur centre;
    double color[3];
    bool mirroir;
    bool transparent;
    double indiceRefract;
    double emission;
public:

    Objet();
    ~Objet();
    virtual Vecteur getCentre() = 0;
    virtual double getColor(int) = 0;
    virtual bool getMirroir() = 0;
    virtual bool getTransparent() = 0;
    virtual double getIndiceRefract() = 0;
    virtual double getEmission() = 0;
    virtual vector<Vecteur> intersect(Rayon) = 0;
    virtual Vecteur normal(Vecteur p) = 0;

};

