#include "main.h"
void the_pool()
{
	Circle* pool = new Circle;
	double *road = new double;
	double *fec = new double;
	pool->Tusk2(road, fec);
	cout << "Cast road for a pool (r=3m):" << *road << endl;
	cout << "Cast fec for a pool (r=3m + 1m road):" << *fec << endl;
	system("pause");
	delete pool, road, fec;
	system("cls");

}