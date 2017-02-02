#ifndef RAYON_H
#define RAYON_H
#include "vecteur.h"


class Rayon
{
public:
    Vecteur camera;
    Vecteur u;

    Rayon();
    Rayon(Vecteur, Vecteur);
};

#endif // RAYON_H
