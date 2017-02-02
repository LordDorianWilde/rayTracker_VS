#include "rayon.h"

Rayon::Rayon()
{
    camera = Vecteur();
    u = Vecteur();
}

Rayon::Rayon(Vecteur cam, Vecteur v)
{
    camera = cam;
    u = v;
}

