#ifndef entry_
#define entry_

struct Entry
{
	Entry() : key(0), val(0) {};

	Entry(num key_p, num val_p) : key(key_p), val(val_p) {}
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

private:
	num key;
	num val;
};

#endif