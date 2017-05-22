#include "main.h"



Circle::Circle()
{
	cout << "Enter radius: ";
	cin >> radius;
}


Circle::~Circle()
{
}

void Circle::show()
{
	cout << "Radius is " << radius << endl;
	cout << "Ference is " << ference << endl;
	cout << "Area is " << area << endl;

}
void Circle::calculation()
{
	ference = 2 * PI*radius;
	area = PI*pow(radius, 2);
}
void Circle::Tusk1(double *&ference2)
{
	radius = 6378100;
	ference = 2 * PI*radius;
	*ference2 = 2*PI*(++radius);
	*ference2 = *ference2-ference;
}
void Circle::Tusk2(double*& k, double*& j)
{
	radius = 3;
	ference =  2 * PI*radius*cast_roud;
	*k = ference;
	ceil(*k);
	ference = 2 * PI*(++radius)*cast_fec;
	*j = ference;
	ceil(*j);
}