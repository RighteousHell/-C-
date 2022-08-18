#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

typedef std::unordered_map<int, int> um_ii;



class Filter
{
public:
	Filter() = default;
	~Filter() = default;
	static std::vector<int> findItems(std::vector<int>& arr);

};