#include "Storage.h"

Storage::Storage(size_t capacity) : m_capacity(capacity)
{
}


void Storage::put(std::string key, std::string value)
{
	if (m_hash_table.end() == m_hash_table.find(key))
	{

		if (m_order.size() == m_capacity)
		{
			std::string lru_key = m_order.back().first;
			m_order.pop_back();
			m_hash_table.erase(lru_key);
		}
	}
	else
	{
		m_order.erase(m_hash_table[key]);
	}
	m_order.push_front(make_pair(key, value));
	m_hash_table[key] = m_order.begin();

}

std::string Storage::get(std::string key)
{
	if (m_hash_table.end() == m_hash_table.find(key))
	{
		return exit_codes::EXIT_EMPTY_STR;
	}

	m_order.push_front(make_pair(key, m_hash_table[key]->second));
	m_order.erase(m_hash_table[key]);
	m_hash_table[key] = m_order.begin();
	return m_hash_table[key]->second;


}

/*int main(void)
{



	return EXIT_SUCCESS;
}*/