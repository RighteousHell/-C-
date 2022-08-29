#include "LongNumsSum.h"

int main()
{
	//file_handler::File_handler source_handler(PATH_TO_SOURCE, file_handler::modes::input);
	//file_handler::File_handler dest_handler(PATH_TO_DEST, file_handler::modes::output);
	std::ifstream fin(PATH_TO_SOURCE);
	std::ofstream fout(PATH_TO_DEST);
	
	if (!fin.is_open())
	{
		std::cout << "Source file didn't open!" << std::endl;
		exit(-1);
	}

	if (!fout.is_open())
	{
		std::cout << "Destination file didn't open!";
		exit(-2);
	}

	if (fin.peek() == EOF)
	{
		fout.put('0');
		exit(0);
	}
	std::string new_line;
	getline(fin, new_line);
	big_num::Big_num sum(new_line);

	while (!fin.eof())
	{
		getline(fin, new_line);
		big_num::Big_num new_num(new_line);
		sum = sum + new_num;
	}
	fout << std::string(sum);

	return 0;
}
