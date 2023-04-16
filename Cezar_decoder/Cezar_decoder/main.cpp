#include <iostream>
#include <string>
#define CHAR_TO_INT(x) x-'0'
#define CHAR_TO_INDEX(x) CHAR_TO_INT(x)-1


void forvard_cezar(std::string mess_for);
void reverse_cezar(std::string mess_rev);
void combined_cezar(std::string mess);

int main(int argc, char** argv)
{

	void (*cezar_directions[3])(std::string) = { forvard_cezar, reverse_cezar, combined_cezar};
	std::string message;

	if (2 == argc)
	{
		message = argv[1];
	}
	else
	{
		std::cout << "Enter your message: ";
		std::cin >> message;
		system("cls");
	}
	char mode = 0;
	std::cout << "1. Forward mode;\n2. Reverse mode\n3.Combined mode\n";
	std::cin >> mode;
	while (mode != '1' && mode != '2' && mode != '3')
	{
		std::cin >> mode;
	}
	system("cls");
	std::cout << "Original message: " << message << std::endl;
	cezar_directions[CHAR_TO_INDEX(mode)](message);


	return EXIT_SUCCESS;
}


void forvard_cezar(std::string mess_for)
{
	std::cout << "Forvard cezar:" << std::endl;
	for (int i = 1; i < 25; ++i)
	{
		for (int j = 0; j < mess_for.length(); ++j)
		{
			++mess_for[j];
			if (mess_for[j] > 'z')
			{
				mess_for[j] -= 26;
			}
		}
		std::cout << i << ". " << mess_for << std::endl;
	}
}
void reverse_cezar(std::string mess_rev)
{
	std::cout << "Reverse cezar:" << std::endl;
	for (int i = 1; i < 25; ++i)
	{
		for (int j = 0; j < mess_rev.length(); ++j)
		{
			--mess_rev[j];
			if (mess_rev[j] < 'a')
			{
				mess_rev[j] += 26;
			}
		}
		std::cout << i << ". " << mess_rev << std::endl;
	}
}

void combined_cezar(std::string mess)
{
	forvard_cezar(mess);
	std::cout << '\v';
	reverse_cezar(mess);
}
