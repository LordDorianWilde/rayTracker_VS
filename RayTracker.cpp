// RayTracker.cpp : définit le point d'entrée pour l'application console.
//
#include <iostream>
#include "CImg.h"
#include <time.h>
#include <omp.h>
#include <math.h>
#include <vector>
#include "vecteur.h"
#include "scene.h"
#include "sphere.h"
#include "string"
#include "rayon.h"

using namespace std;

void createImage()
{
	time_t start;
	time(&start);
	int W = 1024;
	int H = 1024;
	vector<unsigned char> pixels(W*H * 3, 0);
	double alpha = 3.14 / 2.;
	int nbRayon = 50;

	Vecteur C(0, -20, 55);
	Vecteur L(-10, 20, 30);
	Scene scene(L);

	Sphere s(Vecteur(0, -25, 10), 10., "blue", false, true, 1.5);
	scene.addSphere(s);
	Sphere murGauche(Vecteur(-1000, 0, 0), 960, "red", false, false, 0);
	scene.addSphere(murGauche);
	Sphere murDroite(Vecteur(1000, 0, 0), 960, "blue", false, false, 0.);
	scene.addSphere(murDroite);
	Sphere murFond(Vecteur(0, 0, -1000), 960, "green", false, false, 0.);
	scene.addSphere(murFond);
	Sphere murDerriere(Vecteur(0, 0, 1000), 940, "white", false, false, 0.);
	scene.addSphere(murDerriere);
	Sphere sol(Vecteur(0, -1000, 0), 960, "white", false, false, 0.);
	scene.addSphere(sol);

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

			Rayon r(C, u);
			for (int k = 0; k<nbRayon; k++)
			{
				inten = inten + scene.lightPixel(r);
			}

			inten = inten * (1. / nbRayon);

			pixels[i*W + j] = (255.*pow(inten[0] / 255., 1. / 2.2));
			pixels[i*W + j + H*W] = (255.*pow(inten[1] / 255., 1. / 2.2));
			pixels[i*W + j + 2 * H*W] = (255.*pow(inten[2] / 255., 1. / 2.2));

			if (j == 0 && i % 20 == 0)
			{
				cout << "\r" << (i / 10) << "%";
			}

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