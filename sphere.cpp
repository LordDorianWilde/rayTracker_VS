#include "sphere.h"

Sphere::Sphere()
{
    centre = Vecteur();
    rayon = 0.0;
    color[0] = 0.0;
    color[1] = 0.0;
    color[2] = 0.0;
    mirroir = false;
    transparent = false;
    indiceRefract = 1.0;
    emission = 0.0;
}

Sphere::Sphere(Vecteur c, double r, double red , double green, double blue, bool mir, bool transp, double n, double em)
{
    centre = c;
    rayon = r;
    color[0] = red;
    color[1] = green;
    color[2] = blue;
    mirroir = mir;
    transparent = transp;
    indiceRefract = n;
	emission = em;
}

Sphere::Sphere(Vecteur c, double r, string couleur, bool mir, bool transp, double n, double em)
{
    centre = c;
    rayon = r;
    mirroir = mir;
    transparent = transp;
    indiceRefract = n;
	emission = em;

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

vector<Vecteur> Sphere::intersect(Rayon r)
{
	double a = 1;
	double b = (r.u*(r.camera - centre)) * 2;
	double c = (r.camera - centre).squareNorm() - rayon*rayon;
	double delta = b*b - 4 * a*c;
	if (delta >= 0)
	{
		double t1 = (-b - sqrt(delta)) / (2.*a);
		if (t1 > 0)
		{
			Vecteur p = r.camera + r.u*t1;
			Vecteur n = normal(p);
			vector<Vecteur> rep = { p, n };
			return rep;
		}
		else
		{
			double t2 = (-b + sqrt(delta)) / (2.*a);
			if (t2 > 0)
			{
				Vecteur p = r.camera + r.u*t2;
				Vecteur n = normal(p);
				vector<Vecteur> rep = { p, n };
				return rep;
			}
			else
			{
				vector<Vecteur> rep = { Vecteur(), Vecteur() };
				return rep;
			}
		}
	}
	else
	{
		vector<Vecteur> rep = { Vecteur(), Vecteur() };
		return rep;
	}
}


Vecteur Sphere::normal(Vecteur p)
{
	Vecteur n = p - centre;
	return n;
}


Vecteur Sphere::getCentre()
{
    return centre;
}

double Sphere::getColor(int i)
{
    return color[i];
}

bool Sphere::getMirroir()
{
    return mirroir;
}

bool Sphere::getTransparent()
{
    return transparent;
}

double Sphere::getIndiceRefract()
{
    return indiceRefract;
}

double Sphere::getEmission()
{
    return emission;
}
