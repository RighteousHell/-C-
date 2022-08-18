#pragma once

#include <iostream>
#include <unordered_map>
#include <list>
#include <string>
namespace exit_codes
{
	const int OK_EXIT = 0;
	const int NOK_EXIT = -1;
	const std::string EXIT_EMPTY_STR = "";
}


typedef std::unordered_map<std::string, std::list<std::pair<std::string, std::string>>::iterator> stor_ht;
typedef std::pair<std::string, std::string> stor_pair;
typedef std::list<stor_pair> stor_list;

class Storage
{
private:
	stor_ht m_hash_table;
	stor_list m_order;
	size_t m_capacity = 10;


public:
	Storage() = default;
	Storage(size_t capacity);
	~Storage() = default;


	std::string get(std::string key);
	void put(std::string key, std::string value);

};