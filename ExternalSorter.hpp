#ifndef External_Sorter_
#define External_Sorter_

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <algorithm>
#include "ChunkCreator.h"
#include "Types.h"


class ExternalSorter
{
	char *filename;
	std::unique_ptr<ChunkCreator> chunkCreator;

public:
	ExternalSorter(char *filename, std::unique_ptr<ChunkCreator> chunkCreator)
	{
		this->filename = filename;
		this->chunkCreator = std::move(chunkCreator);
	}

	void CreateSortedChunks(std::ifstream& input_file) const
	{
		num line_number = 1;
		num chunk_index = 0;
		while (!input_file.eof())
		{
			chunkCreator->Create(input_file, line_number, "chunk_" + std::to_string(chunk_index));
			chunk_index++;
		}
	}

	void ExternalMergeSort() const
	{

	}

	void Sort()
	{
		std::ifstream input_file(filename);
		if (input_file.is_open())
		{
			CreateSortedChunks(input_file);
			input_file.close();

			ExternalMergeSort();
		}
		else
		{
			throw 0;
		}
	}

};


#endif
