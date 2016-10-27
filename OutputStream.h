#ifndef output_stream_
#define output_stream_
#include "Entry.h"
#include <stdio.h>

class InStream
{
	FILE* file;
	num buffer_size;
	Entry* buffer1;
	Entry* buffer2;
	Entry* current_buffer;
	num position;
	bool closed;
	snum end;
	bool first;

	void fill_buffer()
	{
		num remain_space = buffer_size;
		num readed = 0;
		while (remain_space > 0 && (readed = fread(current_buffer + (buffer_size - remain_space), sizeof(Entry), remain_space, file)) != 0)
			remain_space -= readed;

		if (readed == 0)
		{
			end = buffer_size - remain_space;
		}
	}
public:
	InStream(const char* path, num available_memory, char* memory) :
		file(fopen(path, "rb")),
		buffer_size(available_memory / sizeof(Entry) / 2),
		buffer1(reinterpret_cast<Entry*>(memory)),
		buffer2(reinterpret_cast<Entry*>(memory) + buffer_size),
		current_buffer(buffer1),
		position(0),
		closed(false),
		end(0xFFFFFFFFFFFFFFFF),//TODO
		first(true)
	{
		if (file == nullptr)
			terminatexx("File not opened");
		fill_buffer();
	};

	~InStream()
	{
		if (!closed)
			close();
	}

	bool eof() const
	{
		return position >= end;
	}

	void close()
	{
		fclose(file);
		//delete[] buffer1;
		//delete[] buffer2;
		closed = true;
	}

	const Entry& peak()
	{
		if (eof())
			terminatexx("End of file.");

		if (position >= buffer_size)
		{
			current_buffer = current_buffer == buffer1 ? buffer2 : buffer1;
			position = 0;
			first = false;
			fill_buffer();
		}

		return current_buffer[position];
	}

	const Entry& read()
	{
		const auto& res = peak();
		++position;
		return res;
	}
};

class OutputStream
{
	FILE* file;
	num buffer_size;
	Entry* buffer1;
	Entry* buffer2;
	Entry* current_buffer;
	num position;
	bool closed;
	bool buffer_ready;
	bool first;

	void write_buffer(num size)
	{
		num remain_space = size;
		while (remain_space > 0)
			remain_space -= fwrite(current_buffer + (size - remain_space), sizeof(Entry), remain_space, file);
	}

public:
	OutputStream(const char* path, num available_memory, char* memory) :
		file(fopen(path, "wb")),
		buffer_size(available_memory / sizeof(Entry) / 2),
		buffer1(reinterpret_cast<Entry*>(memory)),
		buffer2(reinterpret_cast<Entry*>(memory) + buffer_size),
		current_buffer(buffer1),
		position(0),
		closed(false),
		buffer_ready(false),
		first(true)
	{
		if (file == nullptr)
			terminatexx("File not open");
	};

	~OutputStream()
	{
		if (!closed)
			close();
	}

	void close()
	{
		write_buffer(position);
		fclose(file);
		//delete[] buffer1;
		//delete[] buffer2;
		closed = true;
	}

	const Entry& read_prev()
	{
		if (first)
			terminatexx("File not open.");

		if (position == 0)
			return (current_buffer == buffer1 ? buffer2 : buffer1)[buffer_size - 1];
		else
			return current_buffer[position - 1];
	}

	void rewrite(const Entry& e)
	{
		if (first)
			terminatexx("File not open.");

		if (position == 0)
			(current_buffer == buffer1 ? buffer2 : buffer1)[buffer_size - 1] = e;
		else
			current_buffer[position - 1] = e;
	}

	void write(const Entry& e)
	{
		if (buffer_ready && position == 1)
		{
			write_buffer(buffer_size);
			buffer_ready = false;
		}

		if (position >= buffer_size)
		{
			buffer_ready = true;
			current_buffer = current_buffer == buffer1 ? buffer2 : buffer1;
			position = 0;
		}
		current_buffer[position] = e;
		++position;
		first = false;
	}
};
#endif