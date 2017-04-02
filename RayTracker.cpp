// RayTracker.cpp : définit le point d'entrée pour l'application console.
//
#include <iostream>
#include <stdlib.h>
#include "CImg.h"
#include "Mesh.h"
#include <time.h>
#include <omp.h>
#include <math.h>
#include <vector>
#include <random>
#include "vecteur.h"
#include "scene.h"
#include "sphere.h"
#include "string"
#include "rayon.h"

using namespace std;

void createImage()
{
	string meshName;
	cout << "Nom du mesh à utiliser : ";
	cin >> meshName;
	cout << endl;
	time_t start;
	time(&start);
	int W = 1024;
	int H = 1024;
	vector<unsigned char> pixels(W*H * 3, 0);
	double alpha = 3.14 / 2.;
	int nbRayon = 1;

	Vecteur C(0, -20, 55);



	Sphere lum(Vecteur(-10, 0, 30), 0.5, "white", false, false, 0., 50000.);
	Scene scene(&lum);

	Mesh mesh("../" + meshName + ".obj", 10.0, Vecteur(0, 0, 0), 1.0, 1.0, 1.0, false, false, 1.0, 0.);
	scene.addObjet(&mesh);

	cout << "Boxing fait." << endl;

	//Sphere s(Vecteur(0, -25, 10), 10., "white", false, false, 1.3, 0.);
	//scene.addObjet(&s);
	Sphere murGauche(Vecteur(-1000, 0, 0), 960, "red", false, false, 0, 0.);
	scene.addObjet(&murGauche);
	Sphere murDroite(Vecteur(1000, 0, 0), 960, "blue", false, false, 0., 0.);
	scene.addObjet(&murDroite);
	Sphere murFond(Vecteur(0, 0, -1000), 960, "green", false, false, 0., 0.);
	scene.addObjet(&murFond);
	Sphere murDerriere(Vecteur(0, 0, 1000), 940, "white", false, false, 0., 0.);
	scene.addObjet(&murDerriere);
	Sphere sol(Vecteur(0, -1000, 0), 960, "white", false, false, 0., 0.);
	scene.addObjet(&sol);

	cout << "scene faite." << endl;

	omp_set_num_threads(8);

	#pragma omp parallel for schedule(dynamic,1)
	for (int i = 0; i<W; i++)
	{
		for (int j = 0; j<H; j++)
		{
			Vecteur u(j - H / 2., W / 2. - i, -W / (2.*tan(alpha / 2.)));
			u.normalize();
			pixels[i*W + j] = 0;
			pixels[i*W + j + H*W] = 0;
			pixels[i*W + j + 2 * H*W] = 0;
			Vecteur inten(0., 0., 0.);
			if (i == 170 && j == 500)
			{
				int grd = 42;
			}


			Rayon r(C, u);
			for (int k = 0; k<nbRayon; k++)
			{
				inten = inten + scene.lightPixel(r);
			}
			inten[0] = (255.*pow(inten[0] / nbRayon / 255., 1. / 2.2));
			inten[1] = (255.*pow(inten[1] / nbRayon / 255., 1. / 2.2));
			inten[2] = (255.*pow(inten[2] / nbRayon / 255., 1. / 2.2));

			pixels[i*W + j] = inten[0];
			pixels[i*W + j + H*W] = inten[1];
			pixels[i*W + j + 2 * H*W] = inten[2];
		}
		if (i % 20 == 0)
		{
			cout << "\r" << (i / 10) << "%";
		}
	}

	cimg_library::CImg<unsigned char> cimg(&pixels[0], W, H, 1, 3);
	cimg.save("fichier.bmp");
	time_t end;
	time(&end);
	double seconds = difftime(end, start);
	cout << endl;
	cout << "temps de calcul : " << seconds << " secondes." << endl;;
}


int main()
{

	createImage();
	char a;
	cin >> a;
	return 0;
}
