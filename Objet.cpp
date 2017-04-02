#include "Objet.h"


Objet::Objet()
{
    centre = Vecteur();
    color[0] = 0.0;
    color[1] = 0.0;
    color[2] = 0.0;
    mirroir = false;
    transparent = false;
    indiceRefract = 1.0;
    emission = 0.0;
}


Objet::~Objet()
{
}
