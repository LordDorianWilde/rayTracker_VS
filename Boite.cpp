#include "Boite.h"


Boite::Boite(vector< vector<int> > t, vector< vector<int> > nS, vector<Vecteur>* p, vector<Vecteur>* n, vector<double> mini, vector<double> maxi, int maxTriangle)
{
	maxPoints = maxi;
	minPoints = mini;
	points = p;
	normale = n;
	triangles = vector< vector<int> >();
	normaleSommet = vector< vector<int> >();
	boites = vector<Boite>();

	for (int i = 0; i < t.size(); i++)
	{
		if (pointInBoite(points->at(t[i][0])) || pointInBoite(points->at(t[i][1])) || pointInBoite(points->at(t[i][2])))
		{
			triangles.push_back(t[i]);
			normaleSommet.push_back(nS[i]);
			
			minPoints[0] = min(min(minPoints[0], points->at(t[i][0])[0]), min(points->at(t[i][1])[0], points->at(t[i][2])[0]));
			minPoints[1] = min(min(minPoints[1], points->at(t[i][0])[1]), min(points->at(t[i][1])[1], points->at(t[i][2])[1]));
			minPoints[2] = min(min(minPoints[2], points->at(t[i][0])[2]), min(points->at(t[i][1])[2], points->at(t[i][2])[2]));

			maxPoints[0] = max(max(maxPoints[0], points->at(t[i][0])[0]), max(points->at(t[i][1])[0], points->at(t[i][2])[0]));
			maxPoints[1] = max(max(maxPoints[1], points->at(t[i][0])[1]), max(points->at(t[i][1])[1], points->at(t[i][2])[1]));
			maxPoints[2] = max(max(maxPoints[2], points->at(t[i][0])[2]), max(points->at(t[i][1])[2], points->at(t[i][2])[2]));
		}
	}

	if (triangles.size() > maxTriangle * 2)
	{
		int index = indexMaxSize();
		
		maxPoints[index] = (maxPoints[index] + minPoints[index]) / 2;
		Boite a = Boite(triangles, normaleSommet, points, normale, minPoints, maxPoints, maxTriangle);
		boites.push_back(a);

		vector< vector<int> > filterList = a.getTriangles();
		vector< vector<int> > secondListTriangles = diffVector(triangles, filterList);
		
		maxPoints[index] = maxPoints[index];
		minPoints[index] = (maxPoints[index] + minPoints[index]) / 2;
		Boite b = Boite(secondListTriangles, normaleSommet, points, normale, minPoints, maxPoints, maxTriangle);
		boites.push_back(b);

	}
}

Boite::Boite()
{
	maxPoints = vector<double>();
	minPoints = vector<double>();
	points = new vector<Vecteur>();
	normale = new vector<Vecteur>();
	triangles = vector< vector<int> >();
	normaleSommet = vector< vector<int> >();
	boites = vector<Boite>();
}

bool Boite::pointInBoite(Vecteur p)
{
	bool rep = true;
	for (int i = 0; i < 3; i++)
	{
		if (p[i] < minPoints[i] || p[i] > maxPoints[i])
		{
			rep = false;
		}
	}
	return rep;
}

int Boite::indexMaxSize()
{
	int index = 0;
	for (int i = 1; i < minPoints.size(); i++)
	{
		if (maxPoints[i] - minPoints[i] > maxPoints[index] - minPoints[index])
		{
			index = i;
		}
	}
	return index;
}

Boite::~Boite()
{
}

vector<Vecteur> Boite::intersect(Rayon u)
{
	vector<double> inf = { 0.0, 0.0, 0.0 };
	vector<double> sup = { 0.0, 0.0, 0.0 };

	for (int i = 0; i<3; i++)
	{
		if (u.u[i] == 0 && u.camera[i] >= minPoints[i] && u.camera[i] <= maxPoints[i])
		{
			inf[i] = -10000;
			sup[i] = 10000;
		}
		else if (u.u[i] == 0)
		{
			inf[i] = 0;
			sup[i] = -1;
		}
		else
		{
			double a = (minPoints[i] - u.camera[i]) / (u.u[i]);
			double b = (maxPoints[i] - u.camera[i]) / (u.u[i]);
			inf[i] = min(a, b);
			sup[i] = max(a, b);
		}
	}
	double mini = max(inf[0], max(inf[1], inf[2]));
	double maxi = min(sup[0], min(sup[1], sup[2]));

	vector<Vecteur> rep = { Vecteur(), Vecteur() };
	if (maxi < mini)
	{
		return rep;
	}
	else if (triangles.size() == 0)
	{
		return rep;
	}
	else if (boites.size() > 0)
	{
		vector<Vecteur> rep1 = boites[0].intersect(u);
		vector<Vecteur> rep2 = boites[1].intersect(u);

		if (rep1[1] == Vecteur() && rep2[1] == Vecteur())
		{
			return rep;
		}
		else if (!(rep1[1] == Vecteur()) && rep2[1] == Vecteur())
		{
			return rep1;
		}
		else if (!(rep2[1] == Vecteur()) && rep1[1] == Vecteur())
		{
			return rep2;
		}
		else
		{
			double dist1 = (rep1[0] - u.camera).squareNorm();
			double dist2 = (rep2[0] - u.camera).squareNorm();

			if (dist1 < dist2)
			{
				return rep1;
			}
			else
			{
				return rep2;
			}
		}
	}
	else if (boites.size() == 0)
	{
		for (int i = 0; i < triangles.size(); i++)
		{
			Vecteur P = intersectPlan(u, i);
			if (P[0] >= minPoints[0] && P[1] >= minPoints[1] && P[2] >= minPoints[2]
				&& P[0] <= maxPoints[0] && P[1] <= maxPoints[1] && P[2] <= maxPoints[2]
				&& (rep[1] == Vecteur() || (P - u.camera).squareNorm() < (rep[0] - u.camera).squareNorm()))
			{
				Vecteur A = points->at(triangles[i][0]);
				Vecteur B = points->at(triangles[i][1]);
				Vecteur C = points->at(triangles[i][2]);
				Vecteur Coord = barycentriqueCoord(P, i);

				if (Coord[1] >= 0 && Coord[2] >= 0 && Coord[1] + Coord[2] < 1)
				{

					Vecteur n = normale->at(normaleSommet[i][0]) * (Coord[0] - Coord[1] - Coord[2]) + normale->at(normaleSommet[i][1]) * Coord[2] + normale->at(normaleSommet[i][2]) * Coord[1];
					n.normalize();
					rep = { P, n };
				}
			}
		}
	
		return rep;
	}

	return rep;
}

Vecteur Boite::intersectPlan(Rayon u, int i)
{
	Vecteur A = points->at(triangles[i][0]);
	Vecteur B = points->at(triangles[i][1]);
	Vecteur C = points->at(triangles[i][2]);

	Vecteur n = (A - B).cross(A - C);
	n.normalize();
	if (u.u*n != 0)
	{
		double t = (u.camera - A)*n / (u.u*n)*(-1);
		Vecteur p = (u.camera + u.u*t);
		double test = n*(A - p);
		return p;
	}
	else
	{
		return Vecteur();
	}
}

Vecteur Boite::barycentriqueCoord(Vecteur P, int i)
{
	Vecteur A = points->at(triangles[i][0]);
	Vecteur B = points->at(triangles[i][1]);
	Vecteur C = points->at(triangles[i][2]);

	Vecteur v2 = P - A;
	Vecteur v1 = B - A;
	Vecteur v0 = C - A;

	double u = ((v1*v1)*(v2*v0) - (v1*v0)*(v2*v1)) / ((v0*v0)*(v1*v1) - (v0*v1)*(v1*v0));
	double v = ((v0*v0)*(v2*v1) - (v0*v1)*(v2*v0)) / ((v0*v0)*(v1*v1) - (v0*v1)*(v1*v0));
	Vecteur rep = Vecteur(1, u, v);
	return rep;
}

vector< vector<int> > Boite::getTriangles()
{
	return triangles;
}

vector< vector<int> > Boite::diffVector(vector< vector<int> > a, vector< vector<int> > b)
{
	vector< vector<int> > rep = vector< vector<int> >();
	
	int i = 0;
	while( i <  a.size() && b.size() > 0)
	{
		if (a[i] != b[0])
		{
			rep.push_back(a[i]);
		}
		else
		{
			b.erase(b.begin());
		}
		i++;
	}

	return rep;
}