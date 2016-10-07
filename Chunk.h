#ifndef chunk_
#define chunk_

#include "Entry.h"

class Chunk
{
	num capacity;
	Entry* chunk;
	num size = 0;

public:
	explicit Chunk(num chunk_byte_size) : capacity(chunk_byte_size / static_cast<num>(sizeof(Entry))), chunk(new Entry[static_cast<int>(capacity)]) { }
	virtual ~Chunk()
	{
		free(chunk);
	}

	void AddEntry(Entry entry)
	{
		if (size == capacity)
			throw "chunk overflow";

		chunk[size++] = entry;
	}

	num Size() const
	{
		return size;
	}

	num Capacity() const
	{
		return capacity;
	}

	bool IsFull() const
	{
		return size == capacity;
	}

	Entry* Begin() const
	{
		return chunk;
	}
	Entry* End() const
	{
		return chunk + size;
	}
};

#endif