#ifndef simple_chunk_creator_
#define simple_chunk_creator_

#include "ChunkCreator.h"
#include "Chunk.h"
#include "Entry.h"
#include "InputNumberStream.h"
#include "QuickSort.h"
#include "LogHelp.h"
#include "SubChunk.h"

class SimpleChunkCreator : public ChunkCreator
{
public:

	void ReadChunk(InputNumberStream& input_file, Chunk& chunk) const
	{
		while (!chunk.IsFull())
		{
			Entry e = input_file.read_next();
			if (e == Entry::empty)
				break;

			chunk.AddEntry(e);
		}
	}

	void SaveChunk(const Chunk& chunk, const std::string& chunk_name) const
	{
		std::fstream output_file(chunk_name, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
		if (!output_file.is_open())
			throw 0;

		output_file.write(reinterpret_cast<char*>(chunk.begin()), chunk.ByteSize());
		if (!output_file.good())
			throw 0;

		output_file.close();
	}

	void set_value(Chunk& ch, Entry*& ch_it, Entry*&sch_it) const
	{
		if (ch_it == ch.begin())
			*ch_it++ = *sch_it;
		else
		{
			auto prev = ch_it - 1;
			if (prev->GetVal() == sch_it->GetVal())
			{
				if (prev->GetKey() > sch_it->GetKey())
				{
					*ch_it++ = *sch_it;
				}
			}
			else
				*ch_it++ = *sch_it;

		}
		sch_it++;
	}

	Chunk& MergeSort(Chunk& read, Chunk& write, SubChunk* subchunks, num chunks_count) const
	{
		if (chunks_count == 1)
		{
			read.Shrink(subchunks[chunks_count - 1].end() - read.begin());
			return read;
		}

		num next_subchunks_count = chunks_count / 2 + (chunks_count % 2);
		SubChunk* next_subchunks = new SubChunk[next_subchunks_count];

		Entry* ch_it = write.begin();
		for (num i = 0; i < chunks_count; i += 2)
		{
			SubChunk& sch1 = subchunks[i];
			SubChunk& sch2 = subchunks[i + 1];

			Entry* sch1_it = sch1.begin();
			Entry* sch2_it = sch2.begin();
			Entry* sch_o_begin = ch_it;

			if (i + 1 != chunks_count)
			{
				while (sch1_it != sch1.end() && sch2_it != sch2.end())
				{
					if (sch1_it->GetVal() < sch2_it->GetVal())
					{
						set_value(write, ch_it, sch1_it);

					}
					else if (sch1_it->GetVal() > sch2_it->GetVal())
					{
						set_value(write, ch_it, sch2_it);
					}
					else
					{
						Entry*& used_it = sch1_it;
						if (sch1_it->GetKey() < sch2_it->GetKey())
						{
							used_it = sch1_it;
							sch2_it++;
						}
						else
						{
							used_it = sch2_it;
							sch1_it++;
						}
						set_value(write, ch_it, used_it);
					}
				}


				while (sch2_it != sch2.end())
					set_value(write, ch_it, sch2_it);
			}

			while (sch1_it != sch1.end())
				set_value(write, ch_it, sch1_it);

			next_subchunks[i / 2] = SubChunk(sch_o_begin, ch_it);
		}


		auto& res = MergeSort(write, read, next_subchunks, next_subchunks_count);
		delete[] next_subchunks;
		return res;
	}


	Chunk& Sort(Chunk& chunk, Chunk& buffer) const
	{
		Entry* begin = chunk.begin();
		Entry* end = chunk.end();
		const num subchunk_byte_size = 1024llu * 1024llu * 1024llu / 2llu;//512 MB
		const num subchunk_size = subchunk_byte_size / sizeof(Entry);
		num size = end - begin;
		if (size <= 0)
			return chunk;

		num sub_chunk_count = size / subchunk_size + ((size % subchunk_size) != 0 ? 1 : 0);

		SubChunk* subchunks = new SubChunk[sub_chunk_count];

		num sum_size = 0;
		Entry* beg_i = begin;
		for (num i = 0; i < sub_chunk_count; ++i, beg_i += subchunk_size)
		{
			subchunks[i] = SubChunk(beg_i, std::min(beg_i + subchunk_size, end));
			subchunks[i].sort();
			sum_size += subchunks[i].size();
		}


		auto ts = std::chrono::steady_clock::now();
		auto& res = MergeSort(chunk, buffer, subchunks, sub_chunk_count);
		auto te = std::chrono::steady_clock::now();
		logt("Merging phase ", ts, te);
		delete[] subchunks;
		return res;
	}


	bool Create(InputNumberStream& input_file, const std::string &chunk_name, num chunk_byte_size) override
	{
		printf("Creating chunk.\n");
		Chunk chunk(chunk_byte_size);
		Chunk buffer(chunk_byte_size);

		printf("Reading chunk.\n");
		auto ts = std::chrono::steady_clock::now();

		ReadChunk(input_file, chunk);
		if (chunk.Size() <= 0)
			return false;

		auto te_read = std::chrono::steady_clock::now();

		printf("Sorting chunk.\n");

		auto& res = Sort(chunk, buffer);

		auto te_sort = std::chrono::steady_clock::now();

		printf("Saving chunk.\n");
		SaveChunk(res, chunk_name);

		auto te_save = std::chrono::steady_clock::now();

		logt("Reading chunk in", ts, te_read);
		logt("Sorting chunk in", te_read, te_sort);
		logt("Saving chunk in", te_sort, te_save);
		return true;
	}
};

#endif
