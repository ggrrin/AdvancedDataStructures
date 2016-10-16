#ifndef InputNumberStream_
#define InputNumberStream_

#include "Types.h"
#include <string>
#include <stdio.h>
#include "Entry.h"
#include <string.h>

class InputNumberStream
{
	const num buf_capacity = 128;
	num line;
	FILE* file;
	char* buf;
	num buf_size;

	char* line_start;
	char* current;

	//shift current line to begining of the buffer and fill the buffer
	//if read returns 0; '\0' is appended at the end of the shifted string
	bool fill_buffer()
	{
		num len = (buf + buf_size) - line_start;
		for (size_t i = 0; i < len; i++)
			*(buf + i) = *(line_start + i);

		buf_size -= len;
		line_start -= len;
		current -= len;

		num read_num = fread((buf + buf_size), sizeof(char), buf_capacity - buf_size, file);
		if (read_num != 0)
		{
			buf_size += read_num;
			return true;
		}
		else
		{
			buf_size += 1;
			*(buf + buf_size - 1) = '\0';
			return false;
		}
	}

	num parse_num(char* str)
	{
		return 0;
	}

public:
	InputNumberStream(char* file_name) :
		line(1),
		file(fopen(file_name, "o")),
		buf(new char[buf_capacity]),
		buf_size(0) {};

	~InputNumberStream()
	{
		close();
	}

	void close()
	{
		fclose(file);
		delete buf;
	};

	num get_line_number() const
	{
		return line;
	}

	Entry read_next()
	{
		current = line_start;
		if (*current == '\0')
			line_start++;

		while (*current != '\n')
		{
			if (current - buf>= buf_size)
			{
				if (!fill_buffer())
				{
					current++;
					break;
				}
			}

			current++;
		}
		*current = '\0';
		line++;

		if (current - line_start <= 0)
			return Entry::empty;

		Entry res(line, parse_num(line_start));
		line_start = current;
		return res;
	}
};

#endif