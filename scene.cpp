#include "scene.h"

Scene::Scene(Objet* lum)
{
	addObjet(lum);
    intensity = 70000;
	engine = std::default_random_engine();
	distribe = std::uniform_real_distribution<double>(0, 1);
}

void Scene::addObjet(Objet* s)
{
    spheres.push_back(s);
}

Vecteur Scene::intensityLight(int index, Rayon r, vector<Vecteur> vect, int occ)
{
    if (spheres[index]->getEmission() > 0.1)
	{
        double a = spheres[index]->getEmission();
		Vecteur inten(a, a, a);
		return inten;
	}
    else if(spheres[index]->getMirroir() == true)
    {
        Vecteur p = vect[0];
        Vecteur n = vect[1];
        n.normalize();
        p = p + n*0.001;
        Vecteur u1 = r.u - (n*(r.u*n))*2;

        Rayon r1(p, u1);
        return lightPixel(r1, occ +1);
    }
    else if(spheres[index]->getTransparent() == true)
    {
        Vecteur p = vect[0];
        Vecteur n = vect[1];
        n.normalize();

        if(n*r.u < 0)
        {
            p = p - n*0.0001;
            Vecteur v = (r.u).refract(n, 1, spheres[index]->getIndiceRefract());
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
            Vecteur v = (r.u).refract(n*(-1), spheres[index]->getIndiceRefract(), 1);
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
        Vecteur p = vect[0];
        Vecteur n = vect[1];
		n.normalize();
        Vecteur lp = spheres[0]->normal(p)*(-1);
        lp.normalize();
		Vecteur lum_dir;
		lum_dir = lum_dir.random(lp, distribe(engine), distribe(engine));
        Rayon lumiere = Rayon(spheres[0]->getCentre(), lum_dir);
        Vecteur l = spheres[0]->intersect(lumiere)[0];
		Vecteur xp = l - p;
		double distLum = xp.squareNorm();
		xp.normalize();
		double cosTheta = n*(lp*(-1));
		double cosThetaPrime = (xp*(-1))*lum_dir;
		double pdf = lp*lum_dir;

		
        double a = max(0., min(255., spheres[0]->getEmission()*cosTheta*cosThetaPrime / (distLum*pdf)));

        for(unsigned int i = 1; i<spheres.size(); i++)
        {
			vector<Vecteur> inter = spheres[i]->intersect(Rayon(p + n*0.002, xp));
			double dist = (inter[0] - p).squareNorm();
			if (!(inter[1] == Vecteur()) && i != index && dist < distLum)
            {
                a = 0.;
            }
        }

		Vecteur inten(a, a, a);
        Vecteur random_dir;
        random_dir = random_dir.random(n, distribe(engine), distribe(engine));
        Rayon r1(p + n*0.001, random_dir);
		Vecteur reflexion = lightPixel(r1, occ +1);
		
		for(int j = 0; j<3; j++)
        {
            inten[j] = inten[j]* spheres[index]->getColor(j) + reflexion[j]*spheres[index]->getColor(j)/3.1415;
			inten[j] = max(0., min(255., inten[j]));
        }

        return inten;
    }

}

Vecteur Scene::lightPixel(Rayon u, int occ)
{
    if(occ == 4)
    {
        Vecteur inten(0., 0., 0.);
        return inten;
    }
    else
    {
        double d = 100000.;
        Vecteur inten(0.,0.,0.);
        vector<Vecteur> vect = { Vecteur(), Vecteur() };
        int indexMin = -1;
        for(unsigned int i = 0; i<spheres.size(); i++)
        {
            vector<Vecteur> a = spheres[i]->intersect(u);
            if(!(a[1] == Vecteur() ) && (u.camera - a[0]).squareNorm() < d )
            {
                indexMin = i;
                vect = a;
                d = (u.camera - a[0]).squareNorm();
            }
        }
        if(indexMin > -1)
        {
            inten = intensityLight(indexMin, u, vect, occ);
        }
        return inten;
    }
}

Vecteur Scene::lightPixel(Rayon u)
{
    return lightPixel(u, 0);
}
