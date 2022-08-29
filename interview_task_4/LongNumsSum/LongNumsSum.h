#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#define PARSE_STEP 9
#define PATH_TO_SOURCE "Source.txt"
#define PATH_TO_DEST "Destination.txt"



namespace big_num
{
	class Big_num
	{
	public:
		Big_num() = default;
		Big_num(std::string& str)
		{
			if (0 == str.length())
			{
				m_lnum.emplace_back(0);
				return;
			}
			else
			{
				if ('-' == str[0])
				{
					m_negative = true;
					str = str.substr(1);
				}
			}

			m_lnum.reserve(str.length());

			for (signed long long i = str.length(); i > 0; i -= PARSE_STEP)
			{
				if (i < PARSE_STEP)
				{
					m_lnum.emplace_back(atoi(str.substr(0, i).c_str()));
				}
				else
				{
					m_lnum.emplace_back(atoi(str.substr(i - PARSE_STEP, PARSE_STEP).c_str()));
				}
			}
		}

		Big_num (const Big_num& rhs) 
		{
			m_lnum = rhs.m_lnum;
			m_negative = rhs.m_negative;
		}

		Big_num& operator=(const Big_num& rhs) 
		{
			if (this == &rhs)
				return *this;
			m_negative = rhs.m_negative;
			m_lnum = rhs.m_lnum;
			return *this;
		}

		friend bool operator ==(const Big_num& left, const Big_num& right) 
		{
			if (left.m_negative != right.m_negative)
			{
				return false;
			}

			if ( left.m_lnum.empty() ) 
			{
				if (right.m_lnum.empty() || (right.m_lnum.size() == 1 && right.m_lnum[0] == 0))
				{
					return true;
				}
				else
				{
					return false;
				}
			}

			if (right.m_lnum.empty()) 
			{
				if (left.m_lnum.size() == 1 && left.m_lnum[0] == 0)
				{
					return true;
				}
				else
				{
					return false;
				}
			}

			if (left.m_lnum.size() != right.m_lnum.size())
			{
				return false;
			}

			for (size_t i = 0; i < left.m_lnum.size(); ++i)
			{
				if (left.m_lnum[i] != right.m_lnum[i])
				{
					return false;
				}
			}

			return true;
		}

		friend bool operator <(const Big_num& left, const Big_num& right) 
		{
			if (left == right)
			{
				return false;
			}

			if (left.m_negative)
			{
				if (right.m_negative)
				{
					return ((-right) < (-left));
				}
				else
				{
					return true;
				}
			}
			else if (right.m_negative)
			{
				return false;
			}
			else 
			{
				if (left.m_lnum.size() != right.m_lnum.size())
				{
					return left.m_lnum.size() < right.m_lnum.size();
				}
				else 
				{
					for (long long i = left.m_lnum.size() - 1; i >= 0; --i)
					{
						if (left.m_lnum[i] != right.m_lnum[i])
						{
							return left.m_lnum[i] < right.m_lnum[i];
						}
					}

				}
			}
			return false;
		}

		const Big_num operator -() const 
		{
			Big_num copy(*this);
			copy.m_negative = !copy.m_negative;
			return copy;
		}
		friend const Big_num operator -(Big_num left, const Big_num& right) 
		{
			if (right.m_negative)
			{
				return left + (-right);
			}
			else if (left.m_negative)
			{
				return -(-left + right);
			}
			else if (left < right)
			{
				return -(right - left);
			}
			bool carry = false;
			for (size_t i = 0; i < right.m_lnum.size() || carry; ++i) {
				left.m_lnum[i] -= carry + (i < right.m_lnum.size() ? right.m_lnum[i] : 0);
				carry = left.m_lnum[i] < 0;
				if (carry)
				{
					left.m_lnum[i] += Big_num::base;
				}
			}
			left.remove_first_zeros();
			return left;
		}

		friend const Big_num operator +(Big_num left, const Big_num& right) //O(max(m,n))
		{

			if (left.m_negative) 
			{
				if (right.m_negative)
				{
					return -(-left + (-right));
				}
				else
				{
					return right - (-left);
				}
			}
			else if (right.m_negative)
			{
				return left - (-right);
			}
			int carry = false; 
			for (size_t i = 0; i < std::max(left.m_lnum.size(), right.m_lnum.size()) || carry; ++i) 
			{
				if (left.m_lnum.size() == i )
				{
					left.m_lnum.emplace_back(0);
				}
				left.m_lnum[i] += int(carry) + (i < right.m_lnum.size() ? right.m_lnum[i] : 0);
				carry = left.m_lnum[i] >= Big_num::base;
				if (carry)
				{
					left.m_lnum[i] -= Big_num::base;
				}
			}
			return left;
		}

		bool is_negative()
		{
			return m_negative;
		}

		void remove_first_zeros() {
			while (m_lnum.size() > 1 && m_lnum.back() == 0) 
			{
				m_lnum.pop_back();
			}

			if (m_lnum.size() == 1 && m_lnum[0] == 0)
			{
				m_negative = false;
			}
		}


		friend std::ostream& operator <<(std::ostream& os, const Big_num& num) 
		{
			if (num.m_lnum.empty()) os << 0;
			else {
				if (num.m_negative) os << '-';
				os << num.m_lnum.back();
				char old_fill = os.fill('0');
				for (long long i = static_cast<long long>(num.m_lnum.size()) - 2; i >= 0; --i) os << std::setw(9) << num.m_lnum[i];
				os.fill(old_fill);
			}
			return os;
		}

		operator std::string() const 
		{
			std::stringstream ss;
			ss << *this;
			return ss.str();
		}

		~Big_num() = default;
	protected:
	private:
		static const int base = 1000000000;
		bool m_negative = false;
		std::vector<int> m_lnum;

	};
}