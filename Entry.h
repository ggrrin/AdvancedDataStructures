#ifndef entry_
#define entry_

#include "Types.h"
#include <cmath>

struct Entry
{
public:
	Entry() : key(0), val(0) {};

	Entry(num key_p, num val_p) : key(key_p), val(val_p)
	{
	}

	static Entry empty;

	const num& GetKey() const
	{
		return key;
	}

	const num& GetVal() const
	{
		return val;
	}

	bool IsEmpty() const
	{
		return key == 0;
	}

	int less(const Entry& e)
	{
		if (val < e.GetVal() || (val == e.GetVal() && key < e.GetKey()))
			return true;
		else
			return false;
	}

	num get_string(char* buffer) const
	{
		char* ptr = buffer;
		num first = get_number_of_digits(GetKey());
		num second = get_number_of_digits(GetVal());
		num_to_str(val, ptr, second + 1);
		ptr += second;
		*ptr++ = ' ';
		num_to_str(key, ptr, first + 1);
		ptr = buffer + first + second + 1;
		*ptr++ = '\n';
		*ptr = '\0';
		return first + second + 2;
	}



	bool operator!=(const Entry& e) const
	{
		return !(*this == e);
	}

	bool operator==(const Entry& e) const
	{
		return this->GetKey() == e.GetKey() && this->GetVal() == e.GetVal();
	}

private:
	num key;
	num val;

	num get_number_of_digits(num number) const
	{
		if (number == 0)
			return 1;

		return 1 + static_cast<num>(log10(number));
	}

	bool num_to_str(num number, char* res, const num& buf_len) const
	{
		char* temp = res;
		if (number == 0 && buf_len > 1)
		{
			*res = '0';
			*(res + 1) = '\0';
			return true;
		}
		else if (number == 0)
		{
			return false;
		}

		num length = get_number_of_digits(number);
		if (length > buf_len)
			return false;

		res += length;
		*(res--) = '\0';
		do
		{
			*(res--) = '0' + (number % 10);
			number /= 10;
		} while (number > 0);

		return true;
	}
};
#endif
