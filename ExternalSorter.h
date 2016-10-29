#ifndef External_Sorter_
#define External_Sorter_

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include "ChunkCreator.h"
#include "Types.h"
#include "Entry.h"
#include "InputNumberStream.h"
#include "OutputStream.h"
#include <chrono>
#include "LogHelp.h"

#define half

class ExternalSorter
{
	std::string filename;
	std::unique_ptr<ChunkCreator> chunkCreator;
	const num gigabyte = 1024llu * 1024llu * 1024llu;
	const num memory_available = 8llu * gigabyte - 8 * 1024;
	//const num memory_available = 16llu * 1024llu * 1024llu; ///15 strangly crash in relesase
	char* memory;

public:
	ExternalSorter(const std::string& filename_p, std::unique_ptr<ChunkCreator> chunkCreator_p)
		: filename(filename_p), chunkCreator(std::move(chunkCreator_p)) {};

	num CreateSortedChunks(InputNumberStream& input_file, char* memory) const
	{
		num chunk_index = 0;
		while (!input_file.eof())
		{
			printf("Creating chunk %lu\n", chunk_index);
			chunkCreator->Create(input_file, "chunk_0_" + std::to_string(chunk_index), memory, memory_available);
			chunk_index++;
		}
		return chunk_index;
	}

	void set_value(bool& first, OutputStream& ch_it, InStream& input) const
	{
		const Entry& sch_it = input.read();
		if (first)
		{
			ch_it.write(sch_it);
			first = false;
		}
		else
		{
			const Entry& prev = ch_it.read_prev();
			if (prev.GetVal() == sch_it.GetVal())
			{
				if (prev.GetKey() > sch_it.GetKey())
				{
					ch_it.rewrite(sch_it);
				}
			}
			else if (prev.GetVal() > sch_it.GetVal())
			{
				printf("reading form chunk %s at %lu \n output stream at %lu \n", input.name.c_str(), input.index, ch_it.index);
				char xxx[255];
				prev.get_string(xxx);
				printf("prev %s", xxx);
				sch_it.get_string(xxx);
				printf("readed %s", xxx);
				
				terminatexx("Invalid Order!!!!");

			}
			else
				ch_it.write(sch_it);
		}
	}

	void MergeSort(num chunks_count, num layer) const
	{
		while (chunks_count > 1)
		{
			num next_subchunks_count = chunks_count / 2 + (chunks_count % 2);

			for (num i = 0; i < chunks_count; i += 2)
			{
				auto ts = std::chrono::steady_clock::now();

				std::string ch1_path = "chunk_" + std::to_string(layer) + "_" + std::to_string(i);
				std::string ch2_path = "chunk_" + std::to_string(layer) + "_" + std::to_string(i + 1);
				std::string ch_out = "chunk_" + std::to_string(layer + 1) + "_" + std::to_string(i / 2);

				const num avail_stream_memory = memory_available / 3;
				InStream sch1(ch1_path.c_str(), avail_stream_memory, memory);
				OutputStream ch_it(chunks_count != 2, ch_out.c_str(), avail_stream_memory, memory + avail_stream_memory);

				bool first = true;

				if (i + 1 != chunks_count)
				{
					InStream sch2(ch2_path.c_str(), avail_stream_memory, memory + 2 * avail_stream_memory);

					while (!sch1.eof() && !sch2.eof())
					{
						const auto& sch1_it = sch1.peak();
						const auto& sch2_it = sch2.peak();
						if (sch1_it.GetVal() < sch2_it.GetVal())
						{
							set_value(first, ch_it, sch1);
						}
						else if (sch1_it.GetVal() > sch2_it.GetVal())
						{
							set_value(first, ch_it, sch2);
						}
						else
						{
							if (sch1_it.GetKey() < sch2_it.GetKey())
							{
								set_value(first, ch_it, sch1);
								sch2.read();
							}
							else
							{
								set_value(first, ch_it, sch2);
								sch1.read();
							}
						}
					}

					while (!sch2.eof())
						set_value(first, ch_it, sch2);

					sch2.close();

					while (!sch1.eof())
						set_value(first, ch_it, sch1);
				}
				else
				{
					ch_it.close();
					sch1.close();
					remove(ch_out.c_str());
					std::rename(ch1_path.c_str(), ch_out.c_str());
				}

				sch1.close();
				ch_it.close();

				//std::remove(ch1_path.c_str());
				//std::remove(ch2_path.c_str());

				auto ts_merge = std::chrono::steady_clock::now();
				printf("Layer %lu (%lu;%lu)", layer, i, i + 1);
				logt("merged in ", ts, ts_merge);
			}

			++layer;
			chunks_count = next_subchunks_count;
		}
		std::string res = "chunk_" + std::to_string(layer) + "_0";
		remove("data.out");
		rename(res.c_str(), "data.out");
	}

	void Sort()
	{
		auto tss = std::chrono::steady_clock::now();
		printf("Allocating memory.\n");
		memory = new char[memory_available];


		printf("Creating intial chunks.\n");
		InputNumberStream input_file(filename.c_str());
		num chunks_count = CreateSortedChunks(input_file, memory);

		input_file.close();

		auto ts = std::chrono::steady_clock::now();
		printf("Merging chunks.\n");
		MergeSort(chunks_count, 0);
		auto te = std::chrono::steady_clock::now();
		logt("External merging in ", ts, te);
		logt("TOTAL in", tss, te);
		delete[] memory;
	}

};

#endif
