#ifndef simple_chunk_creator_
#define simple_chunk_creator_

#include "ChunkCreator.h"
#include <string>
#include <sstream>
#include "Chunk.h"
#include "Entry.h"

class SimpleChunkCreator : public ChunkCreator
{
	num chunk_byte_size = 8LL * 1024LL * 1024LL * 1024LL;
	void ReadChunk(std::ifstream& input_file, num &line_number, Chunk& chunk) const
	{
		std::string line = "";
		while (!chunk.IsFull() && !input_file.eof())
		{
			getline(input_file, line);
			num current_number;
			std::stringstream ss(line);
			ss >> current_number;

			chunk.AddEntry(Entry(line_number, current_number));
			line_number++;
		}
	}

	void SaveChunk()
	{

	}

public:
	bool Create(std::ifstream& input_file, num &line_number, const std::string chunk_name) override
	{
		Chunk chunk(chunk_byte_size);
		ReadChunk(input_file, line_number, chunk);

		//std::sort(chunk.Begin(), chunk.End());

		SaveChunk();
		return true;
	}
};

#endif
