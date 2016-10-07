#ifndef chunk_creator_
#define chunk_creator_

#include <fstream>
#include "Types.h"

class ChunkCreator
{
public:
	virtual ~ChunkCreator() {}

	virtual bool Create(std::ifstream &input_file, num &line_number, const std::string chunk_name) = 0;
};

#endif
