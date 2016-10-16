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
};
#endif
