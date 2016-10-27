#ifndef  log_help_
#define log_help_
#include <chrono>


inline void logt(char* msg, std::chrono::time_point<std::chrono::steady_clock> t1, std::chrono::time_point<std::chrono::steady_clock> t2)
{
	//auto diff = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1);
	//auto diff = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
	auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
	printf("%s %lld ms\n", msg, diff.count());
}

inline void terminatexx(char* message)
{
	printf("%s\n", message);
	throw std::exception("%s");
}

#endif
