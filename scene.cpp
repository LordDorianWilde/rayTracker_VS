#include "scene.h"

Scene::Scene(Sphere lum)
{
	addSphere(lum);
    intensity = 70000;
	engine = std::default_random_engine();
	distribe = std::uniform_real_distribution<double>(0, 1);
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

bool Scene::objetBetweenHiddingLight(Sphere s, Rayon r, double dist)
{
    double a = 1;
    double d = sqrt((spheres[0].centre-r.camera).squareNorm());
    double b = (r.u*(r.camera-s.centre))*2;
    double c = (r.camera-(s.centre)).squareNorm() - (s.rayon)*(s.rayon);
    double delta = b*b - 4*a*c;
    if(delta >= 0)
    {
        double t1 = (-b - sqrt(delta))/(2.*a);
        if(t1 > 0 && t1*t1 < dist)
            return true;
		else
		{
			double t2 = (-b + sqrt(delta)) / (2.*a);
			if (t2 > 0 && t2*t2 < dist)
				return true;
			return false;
		}
    }
	return false;

}

Vecteur Scene::intensityLight(Sphere s, Rayon r, double t1, int occ)
{
    if(occ == 5)
    {
        Vecteur inten(0., 0., 0.);
        return inten;
    }
	else if (s.emission > 0.1)
	{
		double a = s.emission;
		Vecteur inten(a, a, a);
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
            Vecteur v = (r.u).refract(n*(-1), s.indiceRefract, 1);
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
        Vecteur n = (p - s.centre);
		n.normalize();
		Vecteur l = spheres[0].centre;
		Vecteur lp = p - l;
        lp.normalize();
		Vecteur lum_dir;
		lum_dir = lum_dir.random(lp, distribe(engine), distribe(engine));
		Vecteur xp = l + lum_dir*(spheres[0].rayon) - p;
		double distLum = xp.squareNorm();
		xp.normalize();
		double cosTheta = n*(lp*(-1));
		double cosThetaPrime = (xp*(-1))*lum_dir;
		double pdf = lp*lum_dir;

		
		double a = max(0., min(255., spheres[0].emission*cosTheta*cosThetaPrime / (distLum*pdf)));

        for(unsigned int i = 1; i<spheres.size(); i++)
        {
            if(spheres[i] != s && objetBetweenHiddingLight(spheres[i], Rayon(p + n*0.002, xp), distLum))
            {
                a = 0.;
            }
        }

		Vecteur inten(a, a, a);
        Vecteur random_dir;
        random_dir = random_dir.random(n, distribe(engine), distribe(engine));
        Rayon r1(p + n*0.001, random_dir);
		Vecteur reflexion = lightPixel(r1, occ +1);
		reflexion[0] = max(0., min(255., reflexion[0]));
		reflexion[1] = max(0., min(255., reflexion[1]));
		reflexion[2] = max(0., min(255., reflexion[2]));
        for(int j = 0; j<3; j++)
        {
            inten[j] = inten[j]* s.color[j] + reflexion[j]*s.color[j]/3.1415;
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