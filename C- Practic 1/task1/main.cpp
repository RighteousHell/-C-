#include "main.h"
int main(void)
{
	int selector = 0;
	while (true)
	{
		cout << " 1- test Circle class\n 2- Earth and rope\n 3- The pool "<< endl;
		cout << "Enter you choose: ";
		cin >> selector;
		switch (selector)
		{
		case 1:
		{
			test();
		}
		break;
		case 2:
		{
			earth_and_rope();
		}
		break;
		case 3:
		{
			the_pool();
		}
		
		break;
		default:
			return 1;
			
		}
	}
	
	return 0;
}



