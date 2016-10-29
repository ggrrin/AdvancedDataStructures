#ifndef simple_chunk_creator_
#define simple_chunk_creator_

#include <memory>
#include "ChunkCreator.h"
#include "Chunk.h"
#include "Entry.h"
#include "InputNumberStream.h"
#include "QuickSort.h"
#include "LogHelp.h"
#include "SubChunk.h"
#include "OutputStream.h"

struct layer_rec
{
	SubChunk* next_sub_chunks;
	num next_chunks_count;
	SubChunk buffer;
	bool need_dealocation;

	layer_rec(SubChunk* next_sub_chunks_p, num next_chunks_count_p, SubChunk buffer_p, bool need_dealocation_p = true) :
		next_sub_chunks(next_sub_chunks_p),
		next_chunks_count(next_chunks_count_p),
		buffer(buffer_p),
		need_dealocation(need_dealocation_p){};
};


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


	void SaveChunk(const Chunk& chunk, const std::string& chunk_name, bool binnary) const
	{
		Entry* e = chunk.begin();
		Entry prev = *e;

		for (++e; e != chunk.end(); ++e)
		{
			if (prev.GetVal() > e->GetVal())
			{
				terminatexx("Invalid order when saving");
			}

		}


		FILE* f = fopen(chunk_name.c_str(), "wb");
		if (binnary)
		{
			OutputStream::write(chunk.begin(), chunk.Size(), f);
		}
		else
		{
			OutputStream::write_text(chunk.begin(), chunk.Size(), f);
		}


		fclose(f);
	}

	void write_value(bool& first, OutputStream& ch_it, Entry*&sch_it) const
	{
		if (first)
		{
			ch_it.write(*sch_it);
			first = false;
		}
		else
		{
			const Entry& prev = ch_it.read_prev();
			if (prev.GetVal() == sch_it->GetVal())
			{
				if (prev.GetKey() > sch_it->GetKey())
				{
					ch_it.rewrite(*sch_it);
				}
			}
			else
				ch_it.write(*sch_it);
		}
		sch_it++;
	}

	void merge2(bool& first, OutputStream& ch_it, Entry*&sch1_it, Entry*&sch2_it, const SubChunk& sch1, const SubChunk& sch2) const
	{
		while (sch1_it != sch1.end() && sch2_it != sch2.end())
		{
			if (sch1_it->GetVal() < sch2_it->GetVal())
			{
				write_value(first, ch_it, sch1_it);

			}
			else if (sch1_it->GetVal() > sch2_it->GetVal())
			{
				write_value(first, ch_it, sch2_it);
			}
			else
			{
				if (sch1_it->GetKey() < sch2_it->GetKey())
				{
					write_value(first, ch_it, sch1_it);
					sch2_it++;
				}
				else
				{
					write_value(first, ch_it, sch2_it);
					sch1_it++;
				}
			}
		}
	}


	void trivial_sort(Entry** start) const
	{
		Entry** a = (start);
		Entry** b = ((start + 1));
		Entry** c = ((start + 2));
		if (!(*a)->less(**b))
			std::swap(*a, *b);

		if (!(*b)->less(**c))
			std::swap(*b, *c);

		if (!(*a)->less(**b))
			std::swap(*a, *b);
	}


	void write_chunks(const SubChunk& sch1, const SubChunk& sch2, const SubChunk& sch3, const SubChunk& buffer) const
	{
		OutputStream file(false, "chunk-test", buffer.size() * sizeof(Entry), reinterpret_cast<char*>(buffer.begin())); //TODO text mode

		Entry* sch1_it = sch1.begin();
		Entry* sch2_it = sch2.begin();
		Entry* sch3_it = sch3.begin();

		bool first = true;

		while (sch1_it != sch1.end() && sch2_it != sch2.end() && sch3_it != sch3.end())
		{
			Entry* heads[3];
			heads[0] = sch1_it;
			heads[1] = sch2_it;
			heads[2] = sch3_it;
			trivial_sort(heads);
			write_value(first, file, heads[0]);
		}

		merge2(first, file, sch1_it, sch2_it, sch1, sch2);
		merge2(first, file, sch1_it, sch3_it, sch1, sch3);
		merge2(first, file, sch2_it, sch3_it, sch2, sch3);

		while (sch3_it != sch3.end())
			write_value(first, file, sch3_it);

		while (sch2_it != sch2.end())
			write_value(first, file, sch2_it);

		while (sch1_it != sch1.end())
			write_value(first, file, sch1_it);

		file.close();
	}

	void set_value(bool& first, Entry*& ch_it, Entry*&sch_it) const
	{
		if (first)
		{
			*ch_it++ = *sch_it;
			first = false;
		}
		else
		{
			auto prev = ch_it - 1;
			if (prev->GetVal() == sch_it->GetVal())
			{
				if (prev->GetKey() > sch_it->GetKey())
				{
					*(--ch_it)++ = *sch_it;
				}
			}
			else
				*ch_it++ = *sch_it;

		}
		sch_it++;
	}



	void MergeSort(SubChunk& read, SubChunk& buffer, SubChunk* subchunks, num chunks_count) const
	{
		while (chunks_count > 3)
		{
			num next_subchunks_count = chunks_count / 2 + (chunks_count % 2);
			SubChunk* next_subchunks = new SubChunk[next_subchunks_count];

			for (num i = 0; i < chunks_count; i += 2)
			{
				Entry* ch_it = buffer.begin();
				SubChunk& sch1 = subchunks[i];
				SubChunk& sch2 = subchunks[i + 1];

				Entry* sch1_it = sch1.begin();
				Entry* sch2_it = sch2.begin();
				Entry* sch_o_begin = ch_it;
				bool first = true;

				if (i + 1 != chunks_count)
				{
					while (sch1_it != sch1.end() && sch2_it != sch2.end())
					{
						if (sch1_it->GetVal() < sch2_it->GetVal())
						{
							set_value(first, ch_it, sch1_it);

						}
						else if (sch1_it->GetVal() > sch2_it->GetVal())
						{
							set_value(first, ch_it, sch2_it);
						}
						else
						{
							if (sch1_it->GetKey() < sch2_it->GetKey())
							{
								set_value(first, ch_it, sch1_it);
								sch2_it++;
							}
							else
							{
								set_value(first, ch_it, sch2_it);
								sch1_it++;
							}
						}
					}

					while (sch2_it != sch2.end())
						set_value(first, ch_it, sch2_it);
				}

				while (sch1_it != sch1.end())
					set_value(first, ch_it, sch1_it);

				next_subchunks[i / 2] = SubChunk(sch_o_begin, ch_it);
				buffer = SubChunk(ch_it, sch2.end());
			}

			delete[] subchunks;
			BackwardMerge(buffer, next_subchunks, next_subchunks_count);
			subchunks = next_subchunks;
			chunks_count = next_subchunks_count;

		}

		if(chunks_count != 3)
			terminatexx("Chunk count is not 3");

		write_chunks(subchunks[0], subchunks[1], subchunks[2], buffer);
		delete[] subchunks;
	}


	void BackwardMerge(SubChunk& buffer, SubChunk*& subchunks, num& chunks_count) const
	{
		if(chunks_count < 4)
			return;

		num next_subchunks_count = chunks_count / 2 + (chunks_count % 2);
		SubChunk* next_subchunks = new SubChunk[next_subchunks_count];

		for (snum i = chunks_count - 1; i >= 0; i -= 2)
		{
			Entry* ch_it = buffer.end() - 1;
			SubChunk& sch1 = subchunks[i];
			SubChunk& sch2 = subchunks[i - 1];

			Entry* sch1_it = sch1.end() - 1;
			Entry* sch2_it = sch2.end() - 1;
			bool first = true;

			if (i - 1 != 0)
			{
				while (sch1_it != sch1.begin() - 1 && sch2_it != sch2.begin() - 1)
				{
					if (sch1_it->GetVal() > sch2_it->GetVal())
					{
						set_value_max(first, ch_it, sch1_it);

					}
					else if (sch1_it->GetVal() < sch2_it->GetVal())
					{
						set_value_max(first, ch_it, sch2_it);
					}
					else
					{
						if (sch1_it->GetKey() < sch2_it->GetKey())
						{
							set_value_max(first, ch_it, sch1_it);
							sch2_it--;
						}
						else
						{
							set_value_max(first, ch_it, sch2_it);
							sch1_it--;
						}
					}
				}

				while (sch2_it != sch2.begin() - 1)
					set_value_max(first, ch_it, sch2_it);
			}

			while (sch1_it != sch1.begin() - 1)
				set_value_max(first, ch_it, sch1_it);

			next_subchunks[i / 2] = SubChunk(ch_it + 1, buffer.end());
			buffer = SubChunk(sch2.begin(), ch_it + 1);
		}

		delete[] subchunks;
		subchunks = next_subchunks;
		chunks_count = next_subchunks_count; 
	}


	void set_value_max(bool& first, Entry*& ch_it, Entry*&sch_it) const
	{
		if (first)
		{
			*ch_it-- = *sch_it;
			first = false;
		}
		else
		{
			auto prev = ch_it + 1;
			if (prev->GetVal() == sch_it->GetVal())
			{
				if (prev->GetKey() > sch_it->GetKey())
				{
					*(++ch_it)-- = *sch_it;
				}
			}
			else
				*ch_it-- = *sch_it;

		}
		sch_it--;
	}



	void Sort(SubChunk& chunk, num chunk_capacity, SubChunk& buffer) const
	{
		Entry* begin = chunk.begin();
		Entry* end = chunk.end();
		const num subchunk_byte_size = 1024llu * 1024llu * 1024llu / 2llu;//512 MB //64 // 4MB
		//const num subchunk_size = subchunk_byte_size / sizeof(Entry);

		num subchunk_size = chunk_capacity / 24llu;
		num sub_chunk_count = chunk.size() / subchunk_size + ((chunk.size() % subchunk_size) != 0 ? 1 : 0);

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
		MergeSort(chunk, buffer, subchunks, sub_chunk_count);
		auto te = std::chrono::steady_clock::now();
		logt("Merging phase ", ts, te);
		delete[] subchunks;
	}


	bool Create(InputNumberStream& input_file, const std::string &chunk_name, char* memory, num memory_available) override
	{
		num fourth_memory = memory_available / 4;
		printf("Creating chunk.\n");
		Chunk buffer(fourth_memory, memory);
		Chunk chunk(3 * fourth_memory, memory + fourth_memory);

		printf("Reading chunk.\n");
		auto ts = std::chrono::steady_clock::now();

		ReadChunk(input_file, chunk);
		if (chunk.Size() <= 0)
			return false;

		auto te_read = std::chrono::steady_clock::now();

		printf("Sorting chunk.\n");

		SubChunk chunk_sb(chunk.begin(), chunk.end());
		SubChunk buffer_sb(buffer.begin(), buffer.end());
		Sort(chunk_sb, chunk.Capacity(), buffer_sb);

		auto te_sort = std::chrono::steady_clock::now();

		/*		printf("Saving chunk.\n");
				SaveChunk(res, chunk_name, !(input_file.eof() && chunk_name == "chunk_0_0"));

				auto te_save = std::chrono::steady_clock::now();

				logt("Reading chunk in", ts, te_read);
				logt("Sorting chunk in", te_read, te_sort);
				logt("Saving chunk in", te_sort, te_save);*/
		return true;
	}
};

#endif
