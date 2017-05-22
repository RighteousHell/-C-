#include "main.h"
void test()
{
	Circle *test = new Circle;
	test->calculation();
	test->show();
	system("pause");
	system("cls");
	delete test;
}