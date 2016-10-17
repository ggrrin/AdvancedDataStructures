#ifndef entry_
#define entry_

#include "Types.h"
#include <string>

struct Entry
{
public:
	Entry() : key(0), val(0) {};

	Entry(num key_p, num val_p) : key(key_p), val(val_p) {}

	static Entry empty;

	num GetKey() const
	{
		return key;
	}

	num GetVal() const
	{
		return val;
	}

	bool IsEmpty() const
	{
		return key == 0;
	}

	std::string get_string() const
	{
		return std::to_string(val) + " " + std::to_string(key); //TODO order of key
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

	num get_number_of_digits(num number)
	{
		return 1 + static_cast<num>(log10(number));
	}

	bool num_to_str(num number, char* res, const num& buf_len)
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
