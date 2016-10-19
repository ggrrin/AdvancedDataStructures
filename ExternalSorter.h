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
#include "InputNumberStream.h"
#include <errno.h> 


class ExternalSorter
{
	std::string filename;
	std::unique_ptr<ChunkCreator> chunkCreator;

public:
	ExternalSorter(const std::string& filename_p, std::unique_ptr<ChunkCreator> chunkCreator_p)
		: filename(filename_p), chunkCreator(std::move(chunkCreator_p)) {};

	num CreateSortedChunks(InputNumberStream& input_file) const
	{
		num chunk_index = 0;
		while (!input_file.eof())
		{
			printf("Creating chunk %llu\n", chunk_index);
			chunkCreator->Create(input_file, "chunk_0_" + std::to_string(chunk_index));
			chunk_index++;
		}
		return chunk_index;
	}

	void ExternalMergeSort(num chunks_count)
	{
		num max_layer = static_cast<num>(ceil(log2(static_cast<long double>(chunks_count))));

		MergeSort(max_layer, 0, false);
	}

	void rewrite_chunk(Entry& last_entry, std::string& ch_out, bool chunk_output)
	{
		std::ifstream chunk_decode("chunk_0_0", std::ios::in | std::ios::binary);
		std::ofstream output_decode(ch_out, std::ios::out | std::ios::binary | std::ios::trunc);
		if (chunk_decode.is_open() && output_decode.is_open())
		{
			WriteRest(last_entry, chunk_output, chunk_decode, output_decode);
			chunk_decode.close();
			output_decode.close();
		}
		else
		{
			throw 0;
		}
		remove("chunk_0_0");
	}

	void MergeSort(num layer, num offset, bool chunk_output)
	{
		Entry last_entry;

		std::string ch_out = !chunk_output ? "data.out" : "chunk_" + std::to_string(layer) + "_" + std::to_string(offset);

		if (layer == 0)
		{
			if (!chunk_output)
				rewrite_chunk(last_entry, ch_out, chunk_output);

			return;
		}

		num left_index = 2 * offset;
		num right_index = left_index + 1;
		std::string ch1_path = "chunk_" + std::to_string(layer - 1) + "_" + std::to_string(left_index);
		std::string ch2_path = "chunk_" + std::to_string(layer - 1) + "_" + std::to_string(right_index);

		MergeSort(layer - 1, left_index, true);
		MergeSort(layer - 1, right_index, true);


		auto o_mode = std::ios::out | std::ios::trunc;
		if (chunk_output)
			o_mode |= std::ios_base::binary;


		std::ifstream chunk1(ch1_path, std::ios::in | std::ios::binary);
		std::ifstream chunk2(ch2_path, std::ios::in | std::ios::binary);

		if(chunk1.is_open() && !chunk2.is_open())
		{
			chunk1.close();
			if (chunk_output)
				rename(ch1_path.c_str(), ch_out.c_str());
			else
				rewrite_chunk(last_entry, ch_out, chunk_output);

			return;
		}
		else if(!chunk1.is_open() && chunk2.is_open())
		{
			printf("This combination shouldnt be possible.");
			throw 0;
		}
		else if(!chunk1.is_open() && !chunk2.is_open())
		{
			return;
		}

		std::ofstream output_chunk(ch_out, o_mode);

		printf("Merging on level %llu chunks %s : %s\n", layer, ch1_path.c_str(), ch2_path.c_str());

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
				write_value(last_entry, e1, chunk_output, output_chunk, &chunk2);
			else
				write_value(last_entry, e2, chunk_output, output_chunk, &chunk1);

		}

		WriteRest(last_entry, chunk_output, chunk1, output_chunk);
		WriteRest(last_entry, chunk_output, chunk2, output_chunk);

		chunk1.close();
		chunk2.close();
		output_chunk.close();

		std::remove(ch1_path.c_str());
		std::remove(ch2_path.c_str());
	}


	num last_len = 0;
	void write_entry(Entry& e, bool chunk_output, std::ofstream& output_chunk, bool seek_back = false)
	{
		if (chunk_output)
		{
			if(seek_back)
				output_chunk.seekp(-static_cast<off_t>(sizeof(Entry)), std::ios_base::cur);

			output_chunk.write(reinterpret_cast<char*>(&e), sizeof(Entry));
		}
		else
		{
			if(seek_back)
				output_chunk.seekp(-last_len, std::ios_base::cur);

			char str[255];
			auto len = e.get_string(str);
			output_chunk.write(str, len);
			last_len = len;
			j++;
		}

		if (!output_chunk.good())
		{
			printf(strerror(errno));
			throw 0;
		}
	}


	void write_value(Entry& last_entry, Entry& e, bool chunk_output, std::ofstream& output_chunk, std::ifstream* seek_chunk)
	{
		if (last_entry.IsEmpty() || last_entry.GetVal() != e.GetVal())//TODO
		{
			write_entry(e, chunk_output, output_chunk);
		}
		else if(last_entry.GetKey() > e.GetKey())
		{
			write_entry(e, chunk_output, output_chunk, true);
		}


		if (seek_chunk != nullptr)
			seek_chunk->seekg(-static_cast<off_t>(sizeof(Entry)), std::ios_base::cur);

		last_entry = e;
	}

	num i = 0;
	num j = 0;

	void WriteRest(Entry& last_entry, bool chunk_output, std::ifstream &chunk, std::ofstream &output_chunk)
	{
		if (!chunk.is_open())
			return;

		char buf[sizeof(Entry)];
		while (!chunk.eof())
		{
			chunk.read(buf, sizeof(Entry));
			Entry e = *reinterpret_cast<Entry*>(buf);
			if (!chunk.eof() && !chunk.good())
			{
				printf(strerror(errno));
				throw 0;
			}

			if (chunk.good())
			{
				write_value(last_entry, e, chunk_output, output_chunk, nullptr);
				i++;
			}
			else
			{
				//char xxx[200];//TODO
				//e.get_string(xxx);
				//printf(xxx);
			}
		}
		//printf("%llu    %llu\n", i, j);
	}

	void Sort()
	{
		InputNumberStream input_file(filename.c_str());

		printf("Creating intial chunks.\n");
		num chunks_count = CreateSortedChunks(input_file);

		input_file.close();

		printf("Merging chunks.\n");
		ExternalMergeSort(chunks_count);
	}

};


#endif
