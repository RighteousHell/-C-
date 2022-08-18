

#include "Vector_.h"
#include <stdint.h>
#include <vector>

using namespace std;






template <class T>
class myown_alloc
{
public:
	using value_type = T;
	value_type allocate()
	{
	}
	void dealocate()
	{
	}
	myown_alloc() = default;
	~myown_alloc() = default;
};

template <typename T>
class _Vector
{

private:

	uint8_t* buff = nullptr;
	int m_capacity = 0;
	int m_size = 0;
protected:

public:

	_Vector()
	{
		constexpr int    min_size = 10;
		m_capacity = min_size;
		buff = new uint8_t[sizeof(T) * min_size];
		memset(buff, 0, m_capacity);
	}
	~_Vector(){}

	void push_back(T val)
	{
		if (m_size == m_capacity)
		{
			m_capacity *= 2;
			uint8_t* new_buff = new uint8_t[sizeof(T) * m_capacity];
			memset(new_buff, 0, m_capacity);
			std::copy(buff, buff + m_size, new_buff);
			delete[] buff;
			buff = new_buff;
		}
		new(buff + m_size * sizeof(T)) T(val);
		++m_size;	
	}

	T& operator[](int index)
	{
		return reinterpret_cast<T&>( *(buff + index * sizeof(T)));
	}

	const int capacity() const
	{
		return m_capacity;
	}

	const int size() const
	{
		return m_size;
	}

	T front() const
	{
		return static_cast<T>(*(buff));
	}

	T back() const
	{
		if (0 == m_size)
		{
			return static_cast<T>(*(buff ));
		}
		return static_cast<T>(*(buff + (m_size-1) * sizeof(T)));
	}

	T* data()
	{
		return reinterpret_cast<T*>(buff);
	}

	void clear()
	{
		memset(buff, 0, m_capacity);
	}

	bool empty()
	{
		return (0 == size) ? true : false;
	}


};

int main()
{
	_Vector<int> vec;
	for (int i = 0; i < 100; ++i)
	{
		vec.push_back(i);
		//vec[i] *= vec[i];
		std::cout << vec[i] << " ";
	}
	cout << endl << "capacity: " << vec.capacity() << "size: " <<  vec.size() << endl;
	cout << vec.front() << endl;
	cout << vec.back() << endl;
	cout << vec.data() << " element: " << *(vec.data()+2) << endl;
	
	return 0;
}
