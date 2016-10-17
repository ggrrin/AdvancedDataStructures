#ifndef chunk_creator_
#define chunk_creator_

#include <fstream>
#include "Types.h"

class InputNumberStream;

class ChunkCreator
{
public:
	virtual ~ChunkCreator() {}

	virtual bool Create(InputNumberStream& input_file, const std::string &chunk_name) = 0;
};

#endif
