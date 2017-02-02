#include "scene.h"

Scene::Scene(Vecteur b)
{
    lumiere = b;
    intensity = 70000;
}

void Scene::addSphere(Sphere s)
{
    spheres.push_back(s);
}

double Scene::intersecSphere(Sphere s, Rayon r)
{
    double a = 1;
    double b = (r.u*(r.camera-s.centre))*2;
    double c = (r.camera-(s.centre)).squareNorm() - (s.rayon)*(s.rayon);
    double delta = b*b - 4*a*c;
    if(delta >= 0)
    {
        double t1 = (-b - sqrt(delta))/(2.*a);
		if (t1 > 0)
			return t1;
		else
		{
			double t2 = (-b + sqrt(delta)) / (2.*a);
			if (t2 > 0)
				return t2;
			else
				return 1000000.;
		}       
    }
    else
        return 1000000.;
}

bool Scene::objetBetweenHiddingLight(Sphere s, Rayon r)
{
    double a = 1;
    double d = sqrt((lumiere-r.camera).squareNorm());
    double b = (r.u*(r.camera-s.centre))*2;
    double c = (r.camera-(s.centre)).squareNorm() - (s.rayon)*(s.rayon);
    double delta = b*b - 4*a*c;
    if(delta >= 0)
    {
        double t1 = (-b - sqrt(delta))/(2.*a);
        double t2 = (-b + sqrt(delta))/(2.*a);
        if((t1 > 0 && t1 <= d) || (t2 > 0 && t2 <= d))
            return true;
        else
            return false;
    }
    else
        return false;
}

Vecteur Scene::intensityLight(Sphere s, Rayon r, double t1, int occ)
{
    if(occ == 5)
    {
        Vecteur inten(0., 0., 0.);
        return inten;
    }
    else if(s.mirroir)
    {
        Vecteur p = (r.camera+(r.u*t1));
        Vecteur n = (p - s.centre);
        n.normalize();
        p = p + n*0.001;
        Vecteur u1 = r.u - (n*(r.u*n))*2;

        Rayon r1(p, u1);
        return lightPixel(r1, occ +1);
    }
    else if(s.transparent)
    {
        Vecteur p = (r.camera+(r.u*t1));
        Vecteur n = (p - s.centre);
        n.normalize();

        if(n*r.u < 0)
        {
            p = p - n*0.0001;
            Vecteur v = (r.u).refract(n, 1, s.indiceRefract);
            v.normalize();
            if(v.squareNorm() > 0)
            {
                Rayon r1(p, v);
                return lightPixel(r1, occ +1);
            }
            else
            {
                Vecteur inten(0., 0., 0.);
                return inten;
            }
        }
        else
        {
            p = p + n*0.0001;
            Vecteur v = (r.u).refract(n - (n*2), s.indiceRefract, 1);
            v.normalize();
            if(v.squareNorm() > 0)
            {
                Rayon r1(p, v);
                return lightPixel(r1, occ +1);
            }
            else
            {
                Vecteur inten(0., 0., 0.);
                return inten;
            }
        }

    }
    else
    {
        Vecteur p = (r.camera+(r.u*t1));
        Vecteur l = lumiere-p;
		Vecteur n = (p - s.centre);
		n.normalize();
        double d = l.squareNorm();
        l.normalize();
        p = p - s.centre;
        p.normalize();
        double a = min(255., (l*p)*intensity/(d));
        for(unsigned int i = 0; i<spheres.size(); i++)
        {
            if(spheres[i] != s && objetBetweenHiddingLight(spheres[i], Rayon((r.camera+(r.u*t1)), l)))
            {
                a = 0.;
            }
        }

		Vecteur inten(a, a, a);
        Vecteur random_dir;
        random_dir = random_dir.random(p);
        Rayon r1(p + n*0.001, random_dir);
		Vecteur reflexion = lightPixel(r1, occ +1);
        for(int j = 0; j<3; j++)
        {
            inten[j] = inten[j]* s.color[j] + reflexion[j]*s.color[j];
        }

        return inten;
    }

}

Vecteur Scene::lightPixel(Rayon u, int occ)
{
    double d = 10000.;
    Vecteur inten(0.,0.,0.);
    int indexMin = -1;
    for(unsigned int i = 0; i<spheres.size(); i++)
    {
        double a = intersecSphere(spheres[i], u);
        if( a < d )
        {
            indexMin = i;
            d = a; 
        }
    }
    if(indexMin > -1)
    {
        inten = intensityLight(spheres[indexMin], u, d, occ);
    }
    return inten;
}

Vecteur Scene::lightPixel(Rayon u)
{
    return lightPixel(u, 0);
}

double Scene::f(double x)
{
    double rep = 0;
    if(x > 3.1415/2 || x < -3.1415/2)
    {
        rep = 0;
    }
    else
    {
        rep = pow(cos(x), 20);
    }

    return rep;
}

double Scene::pdf(double x, double mu, double sigma)
{
    double rep = exp(-(x-mu)*(x-mu)/(2*sigma*sigma))/(sigma*sqrt(2*3.1415));
    return rep;
}

double Scene::fmult(vector<double> x)
{
    double a = 1;
    for(unsigned int i = 0; i<x.size(); i++)
    {
        a *= f(x[i]);
    }
    return a;
}

double Scene::pdfmult(vector<double> x, double mu, double sigma)
{
    double a = 1;
    for(unsigned int i = 0; i<x.size(); i++)
    {
        a *= pdf(x[i], mu, sigma);
    }
    return a;
}

double Scene::monteCarlo(double mu, double sigma)
{
    int maxIter = 10000;
    int i = 0;
    double rep = 0.;
    while(i < maxIter)
    {
        vector<double> val;
        double u = ((double) rand() / (RAND_MAX));
        double v = ((double) rand() / (RAND_MAX));
        double u1 = ((double) rand() / (RAND_MAX));
        double v1 = ((double) rand() / (RAND_MAX));
		double logu = sqrt(-2 * log(u));
        val.push_back(logu*cos(2*3.1415*v)*sigma + mu);
        val.push_back(logu*sin(2*3.1415*v)*sigma + mu);
        val.push_back(sqrt(-2*log(u1))*cos(2*3.1415*v1)*sigma + mu);
        double a = fmult(val)/pdfmult(val,mu,sigma);
        rep += a;
        i++;
    }

    rep /= maxIter;
    return rep;
}
