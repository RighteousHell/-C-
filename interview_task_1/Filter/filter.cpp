#include "filter.h"



/*int main(void)
{

	std::vector<int> preset = { 1, 0, 2, 9, -6, 1, -6, 0, 19, 5, 2 };

	for (auto& tmp : Filter::findItems(preset))
	{
		std::cout << tmp << " ";
	}



	return EXIT_SUCCESS;
}*/

std::vector<int> Filter::findItems(std::vector<int>& arr) // O(n)
{
	std::vector<int> answer;

	um_ii hash_table;

	for (auto& tmp : arr)
	{
		hash_table[tmp]++;
	}

	for (auto& tmp : hash_table)
	{

		if (1 < tmp.second)
			answer.emplace_back(tmp.first);
	}

	return answer;
};