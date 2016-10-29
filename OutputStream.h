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
	num index;
	std::string name;

	InStream(const char* path, num available_memory, char* memory) :
		file(fopen(path, "rb")),
		buffer_size(available_memory / sizeof(Entry) / 2),
		buffer1(reinterpret_cast<Entry*>(memory)),
		buffer2(reinterpret_cast<Entry*>(memory) + buffer_size),
		current_buffer(buffer1),
		position(0),
		closed(false),
		end(0xFFFFFFFFFFFFFFFF),
		first(true),
		index(0),
		name(path)
	{
		if (file == nullptr)
			terminatexx("File not opened");
		fill_buffer();
	};

	~InStream()
	{
			close();
	}

	bool eof() const
	{
		return position >= end;
	}

	void close()
	{
		if (closed)
			return;


		fclose(file);
		//delete[] buffer1;
		//delete[] buffer2;
		closed = true;
	}

	const Entry& peak()
	{
		if (eof())
			terminatexx("End of file.");



		return current_buffer[position];
	}

	const Entry& read()
	{
		const auto& res = peak();
		++position;
		++index;

		//check for next read
		if (position >= buffer_size)
		{
			current_buffer = current_buffer == buffer1 ? buffer2 : buffer1;
			position = 0;
			first = false;
			fill_buffer();
		}

		return res;
	}
};

class OutputStream
{
	bool binnary;
	FILE* file;
	num buffer_size;
	Entry* buffer1;
	Entry* buffer2;
	Entry* current_buffer;
	num position;
	bool closed;
	bool buffer_ready;
	bool first;

	void write_buffer(Entry* buf, num size)
	{
		if (binnary)
			write(buf, size, file);
		else
			write_text(buf, size, file);
	}

public:
	num index;
	std::string name;

	template<typename T>
	static void write(T* buf, num size, FILE* file)
	{
		num remain_space = size;
		while (remain_space > 0)
			remain_space -= fwrite(buf + (size - remain_space), sizeof(T), remain_space, file);
	}

	static void write_text(Entry* current_buffer, num size, FILE* file)
	{
		char buf[2048];
		Entry* i = current_buffer;

		num len = 0;

		while (i != current_buffer + size)
		{
			for (; i != current_buffer + size; ++i)
			{
				if (len < 1750)
					len += i->get_string(buf + len);
				else
					break;
			}

			write(buf, len, file);
			len = 0;
		}
	}



	OutputStream(bool binnaryp, const char* path, num available_memory, char* memory) :
		binnary(binnaryp),
		file(fopen(path, binnary ? "wb" : "w")),
		buffer_size(available_memory / sizeof(Entry) / 2),
		buffer1(reinterpret_cast<Entry*>(memory)),
		buffer2(reinterpret_cast<Entry*>(memory) + buffer_size),
		current_buffer(buffer1),
		position(0),
		closed(false),
		buffer_ready(false),
		first(true),
		index(0),
		name(path)
	{
		if (file == nullptr)
			terminatexx("File not open");
	};

	~OutputStream()
	{
		close();
	}

	void close()
	{
		if (closed)
			return;

		if (!buffer_ready)
		{
			write_buffer(current_buffer, position);
		}
		else
		{
			//previous buffer is full and next is empty => thus write whole previous one
			write_buffer(current_buffer == buffer1 ? buffer2 : buffer1, buffer_size);
			write_buffer(current_buffer != buffer1 ? buffer2 : buffer1, position);
		}

		fclose(file);
		//delete[] buffer1;
		//delete[] buffer2;
		closed = true;
	}

	const Entry& read_prev()
	{
		if (first)
			terminatexx("File not open.");

		return current_buffer[position - 1];
	}

	void rewrite(const Entry& e)
	{
		if (first)
			terminatexx("File not open.");

		current_buffer[position - 1] = e;
	}

	void write(const Entry& e)
	{
		if (buffer_ready && position == 1)
		{
			write_buffer(current_buffer == buffer1 ? buffer2 : buffer1, buffer_size);
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
		++index;
		first = false;
	}
};
#endif
