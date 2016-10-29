#ifndef InputNumberStream_
#define InputNumberStream_

#include "Types.h"
#include <stdio.h>
#include "Entry.h"
#include "LogHelp.h"

class InputNumberStream
{
	const num buf_capacity = 2*1024;
	bool closed = false;
	bool eof_f = false;
	line_t line;
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

		buf_size = len;
		line_start = buf;
		current = buf + len - 1;

		num read_num = fread((buf + buf_size), sizeof(char), buf_capacity - buf_size, file);
		if (read_num != 0)
		{
			buf_size += read_num;
			return true;
		}
		else
		{
			eof_f = true;
			return false;
		}
	}

	num parse_num(char* str) const
	{
		bool overflow = false;

		num res = 0;

		for (char* input = str; *input != '\0'; ++input)
		{
			if (*input < '0' || *input > '9')
			{
				terminatexx("invalid character");
			}

			num prev = res;

			res *= 10;
			res += *input - '0';

			if (!overflow && prev > res)
			{
				overflow = true;
			}
		}

		return res;
	}

public:
	InputNumberStream(const char* file_name) :
		line(1),
		buf(new char[buf_capacity]),
		line_start(buf),
		file(fopen(file_name, "r")),
		buf_size(0)
	{};

	~InputNumberStream()
	{
		close();
	}

	void close() 
	{
		if (!closed)
		{
			fclose(file);
			delete[] buf;
			closed = true;
		}
	};

	num get_line_number() const
	{
		return line;
	}

	bool eof() const
	{
		return eof_f;
	}

	Entry read_next()
	{
		if (buf_size == 0)
		{
			if (!fill_buffer())
				return Entry::empty;
			else
				line_start = buf;
		}

		current = line_start;
		if (*current == '\0')
			line_start++;

		while ( current == buf + buf_size || *current != '\n')
		{
			if (current >= buf + buf_size )
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

		if (current - line_start <= 0)
			return Entry::empty;

		Entry res(line, parse_num(line_start));
		line_start = current;
		line++;
		return res;
	}
};

#endif
