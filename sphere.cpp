#include "sphere.h"

Sphere::Sphere(Vecteur c, double r, double red , double green, double blue, bool mir, bool transp, double n)
{
    centre = c;
    rayon = r;
    color[0] = red;
    color[1] = green;
    color[2] = blue;
    mirroir = mir;
    transparent = transp;
    indiceRefract = n;
}

Sphere::Sphere(Vecteur c, double r, string couleur, bool mir, bool transp, double n)
{
    centre = c;
    rayon = r;
    mirroir = mir;
    transparent = transp;
    indiceRefract = n;

    if(couleur == "blue")
    {
        color[0] = 0.;
        color[1] = 0.;
        color[2] = 1.;
    }
    else if(couleur == "green")
    {
        color[0] = 0.;
        color[1] = 1.;
        color[2] = 0.;
    }
    else if(couleur == "red")
    {
        color[0] = 1.;
        color[1] = 0.;
        color[2] = 0.;
    }
    else
    {
        color[0] = 1.;
        color[1] = 1.;
        color[2] = 1.;
    }
}

bool Sphere::operator!= (Sphere s)
{
    if(centre[0] != s.centre[0] || centre[1] != s.centre[1] || centre[2] != s.centre[2] || rayon != s.rayon)
        return true;
    return false;
}
