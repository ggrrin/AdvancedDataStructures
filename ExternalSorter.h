#ifndef External_Sorter_
#define External_Sorter_

#include <iostream>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <memory>
#include <string>
#include <algorithm>
#include "ChunkCreator.h"
#include "Types.h"
#include "Entry.h"


class ExternalSorter
{
	std::string filename;
	std::unique_ptr<ChunkCreator> chunkCreator;

public:
	ExternalSorter(const std::string& filename_p, std::unique_ptr<ChunkCreator> chunkCreator_p)
		: filename(filename_p), chunkCreator(std::move(chunkCreator_p)) {};

	num CreateSortedChunks(std::ifstream& input_file) const
	{
		num line_number = 1;
		num chunk_index = 0;
		while (!input_file.eof())
		{
			chunkCreator->Create(input_file, line_number, "chunk_0_" + std::to_string(chunk_index));
			chunk_index++;
		}
		return chunk_index;
	}

	void ExternalMergeSort(num chunks_count)
	{
		num max_layer = ceil(log2(static_cast<long double>(chunks_count)));

		MergeSort(max_layer, 0, false);
	}

	void MergeSort(num layer, num offset, bool chunk_output)
	{
		if (layer == 0)
			return;

		num left_index = 2 * offset;
		num right_index = left_index + 1;

		MergeSort(layer - 1, left_index, true);
		MergeSort(layer - 1, right_index, true);

		std::string ch1_path = "chunk_" + std::to_string(layer - 1) + "_" + std::to_string(left_index);
		std::string ch2_path = "chunk_" + std::to_string(layer - 1) + "_" + std::to_string(right_index);
		std::string ch_out = !chunk_output ? "data.out" : "chunk_" + std::to_string(layer) + "_" + std::to_string(offset);

		std::ifstream chunk1(ch1_path);
		std::ifstream chunk2(ch2_path);
		std::ofstream output_chunk(ch_out);

		num last_value = 0;
		bool first = true;
		while (!chunk1.eof() && !chunk2.eof())
		{
			char buf[sizeof(Entry)];
			chunk1.read(buf, sizeof(Entry));
			Entry e1 = *reinterpret_cast<Entry*>(buf);

			chunk2.read(buf, sizeof(Entry));
			Entry e2 = *reinterpret_cast<Entry*>(buf);

			if (chunk1.fail() || chunk2.fail())
			{
				if (!chunk1.fail())
					chunk1.seekg(-static_cast<off_t>(sizeof(Entry)), std::ios_base::cur);

				if (!chunk2.fail())
					chunk2.seekg(-static_cast<off_t>(sizeof(Entry)), std::ios_base::cur);

				break;
			}

			if (e1.GetVal() <= e2.GetVal())
				write_value(first, last_value, e1, chunk_output, output_chunk, &chunk2);
			else
				write_value(first, last_value, e2, chunk_output, output_chunk, &chunk1);

			first = false;
		}

		WriteRest(first, last_value, chunk_output, chunk1, output_chunk);
		WriteRest(first, last_value, chunk_output, chunk2, output_chunk);

		chunk1.close();
		chunk2.close();
		output_chunk.close();

		std::remove(ch1_path.c_str());
		std::remove(ch2_path.c_str());
	}

	void write_value(bool first, num& last_value, Entry& e, bool chunk_output, std::ofstream& output_chunk, std::ifstream* seek_chunk)
	{
		if (first || last_value != e.GetVal())
		{
			if (chunk_output)
			{
				output_chunk.write(reinterpret_cast<char*>(&e), sizeof(Entry));
			}
			else
			{
				std::string s = e.get_string() + "\n";
				output_chunk.write(s.c_str(), s.length());
			}
		}

		if (seek_chunk != nullptr)
			seek_chunk->seekg(-static_cast<off_t>(sizeof(Entry)), std::ios_base::cur);

		last_value = e.GetVal();
	}

	void WriteRest(bool first, num& last_value, bool chunk_output, std::ifstream &chunk, std::ofstream &output_chunk)
	{
		while (chunk.is_open() && !chunk.eof())
		{
			char buf[sizeof(Entry)];
			chunk.read(buf, sizeof(Entry));
			Entry e = *reinterpret_cast<Entry*>(buf);
			write_value(first, last_value, e, chunk_output, output_chunk, nullptr);
		}
	}

	void Sort()
	{
		std::ifstream input_file(filename);
		if (input_file.is_open())
		{
			num chunks_count = CreateSortedChunks(input_file);
			input_file.close();

			ExternalMergeSort(chunks_count);
		}
		else
		{
			throw 0;
		}
	}

};


#endif
