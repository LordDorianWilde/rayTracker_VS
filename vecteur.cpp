#include "vecteur.h"

double pi = 3.14159;

Vecteur::Vecteur(double a, double b, double c)
{
    xyz[0] = a;
    xyz[1] = b;
    xyz[2] = c;
}

Vecteur::Vecteur()
{
    xyz[0] = 0;
    xyz[1] = 0;
    xyz[2] = 0;
}

void Vecteur::normalize()
{
    double n = sqrt(xyz[0]*xyz[0] + xyz[1]*xyz[1] + xyz[2]*xyz[2]);
	if (n != 0)
	{
		xyz[0] /= n;
		xyz[1] /= n;
		xyz[2] /= n;
	}
}

double Vecteur::squareNorm()
{
    return (xyz[0]*xyz[0] + xyz[1]*xyz[1] + xyz[2]*xyz[2]);
}

double & Vecteur::operator[] (const int i)
{
    return xyz[i];
}

Vecteur Vecteur::operator+(Vecteur b)
{
    Vecteur c(xyz[0]+b[0], xyz[1]+b[1], xyz[2]+b[2]);
    return c;
}

double Vecteur::operator* (Vecteur b)
{
    double c = xyz[0]*b[0] + xyz[1]*b[1] + xyz[2]*b[2];
    return c;
}

Vecteur Vecteur::operator* (double a)
{
    Vecteur c(xyz[0]*a, xyz[1]*a, xyz[2]*a);
    return c;
}

Vecteur Vecteur::operator/ (double a)
{
	Vecteur c(xyz[0] / a, xyz[1] / a, xyz[2] / a);
	return c;
}

Vecteur Vecteur::operator- (Vecteur b)
{
    Vecteur c(xyz[0]-b[0], xyz[1]-b[1], xyz[2]-b[2]);
    return c;
}

bool Vecteur::operator== (Vecteur b)
{
	bool a = xyz[0] == b[0] && xyz[1] == b[1] && xyz[2] == b[2];
	return a;
}

Vecteur Vecteur::cross (Vecteur b)
{
    Vecteur c(xyz[1]*b[2]-b[1]*xyz[2], xyz[2]*b[0]-b[2]*xyz[0], xyz[0]*b[1]-b[0]*xyz[1]);
    return c;
}

Vecteur Vecteur::refract(Vecteur n, double n1, double n2)
{
    double cosThetai = (*this)*n;
    double d = 1 - (n1*n1/(n2*n2)*(1 - cosThetai*cosThetai));

    if(d > 0)
    {
        Vecteur r = (*this)*(n1/n2) - n*(n1/n2*cosThetai + sqrt(d));
        return r;
    }
    else
    {
        return Vecteur();
    }

}

Vecteur Vecteur::random(Vecteur n, double u, double v)
{
	double sqrt1v = sqrt(1 - v);
    double x = cos(2*pi*u)*sqrt1v;
    double y = sin(2*pi*u)*sqrt1v;
    double z = sqrt(v);

	Vecteur t1;
	if (n[0] != 0 || n[1] != 0)
		t1 = Vecteur(n[1], -n[0], 0);
	else
		t1 = Vecteur(0, n[2], -n[1]);
	t1.normalize();
    Vecteur t2 = n.cross(t1);
    Vecteur c = t1*x + t2*y + n*z;
    return c;
}
