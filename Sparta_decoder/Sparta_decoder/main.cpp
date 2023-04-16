#include <iostream>
#include <string>

int main(int argc, char** argv)
{
	std::string message;
	if (2 == argc)
	{
		message = argv[1];
	}
	else
	{
		std::cout << "Enter your message: ";
		std::cin >> message;
		std::cout << std::endl;
	}

	std::cout << "Original message: " << message << std::endl;

	for (int i = 1; i <= message.length(); ++i)
	{
		std::cout << i << ". \t";
		int start_pos = 0;
		int cur_pos = start_pos;
		std::cout << message[cur_pos];
		for (int j = 0; j < message.length()-1; ++j)
		{
			cur_pos += i;
			if (cur_pos >= message.length())
			{
				cur_pos = ++start_pos;
			}
			std::cout << message[cur_pos];
		}
		std::cout << std::endl;
	}
	return EXIT_SUCCESS;
}



