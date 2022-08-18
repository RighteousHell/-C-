// Img.cpp: определяет точку входа для приложения.
//

#include "Img.h"
#include <stdexcept>

using namespace std;
typedef unsigned char byte;

class OutOfRangeExcep
{};

class Img
{
protected:
public:
	Img() = default;
	Img(int h, int w, byte* d): height(h), width(w), data(d)
	{
	}


	byte getByte(int r, int c)
	{
		if (0 > r || 0 > c || r >= height || c >= width)
		{
			throw std::out_of_range("getByte except");
		}
		

		return *(data + (width * r + c));
	}

	Img subImg(int h, int w, int r, int c)
	{
		if (w < 0 || h < 0 || r < 0 || c < 0)
		{
			throw std::out_of_range("subImg except");
		}
		size_t size_new_img = h* width + w;
		if (width * height <= r * width + c + size_new_img)
		{
			throw std::out_of_range("Impossible to create such img");
		}
		int offset = r * width + c;
		byte* new_img_data = new byte[size_new_img];

		memcpy(new_img_data, data + offset, size_new_img);
		return {h, w, new_img_data};
	}

	const byte* getImg() const
	{
		return data;
	}

	size_t getSize() const
	{
		return width * height;
	}
private:
	int width;
	int height;
	byte* data;
};
int main()
{
	byte* data = new byte[5*10];
	for (int i = 0; i < 50; ++i)
	{
		data[i] = i;
	}

	Img test_img(5, 10, data);
	for (int i = 0; i < test_img.getSize(); ++i)
	{
				cout << (int)test_img.getImg()[i] << " ";
	}
	cout << endl;
	

	try
	{

		byte test_byte =  test_img.getByte(4, 0);
		cout << (int)test_byte << endl;
	}
	
	catch (std::out_of_range& er)
	{
		cout << er.what();
	}

	try
	{

		Img test_subimg = test_img.subImg(1, 5, 1 ,5);
		for (int i = 0; i < 15; ++i)
		{
			byte tmp = test_subimg.getImg()[i];
			cout << (int)tmp << " ";
		}
	}
	catch (std::out_of_range& er)
	{
		cout << er.what();
	}
	return 0;
}