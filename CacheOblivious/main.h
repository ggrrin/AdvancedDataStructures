#include <cstdint>
#include <exception>
#include <chrono>

#define SMALL 16

struct matrix
{
	std::int32_t* data;
	std::int32_t orig_dim, m, n, offset_m, offset_n;

	matrix(std::int32_t* data_p, std::int32_t orig_dim_p, std::int32_t m_p, 
		std::int32_t n_p, std::int32_t offset_m_p, std::int32_t offset_n_p) :
		data(data_p), orig_dim(orig_dim_p), m(m_p), n(n_p), 
		offset_m(offset_m_p), offset_n(offset_n_p)
	{ 
		if (m <= 0 || n <= 0)
			throw 0;//std::invalid_argument("Dimension has to by > 0!!");
	};

	std::int32_t get_data_length() const
	{
		return orig_dim * orig_dim;
	}

	//zero based!!!!!
	std::int32_t& at(std::int32_t i, std::int32_t j)
	{
		std::int32_t g_i = offset_m + i;
		std::int32_t g_j = offset_n + j;
		
		return *(data + (g_i * orig_dim + g_j));
	}

	std::int32_t size() const
	{
		return m * n;
	}

	matrix get_m11() const 
	{
		std::int32_t new_m = m / 2;
		std::int32_t new_n = n / 2;

		return matrix(data, orig_dim, new_m, new_n, offset_m, offset_n);
	};

	matrix get_m12() const 
	{
		std::int32_t new_m = m / 2;
		std::int32_t new_n = n / 2;

		return matrix(data, orig_dim, new_m, n - new_n, offset_m, offset_n + new_n);
	};

	matrix get_m21() const 
	{
		std::int32_t new_m = m / 2;
		std::int32_t new_n = n / 2;

		return matrix(data, orig_dim, m - new_m, new_n, offset_m + new_m, offset_n);
	};
	matrix get_m22() const 
	{
		std::int32_t new_m = m / 2;
		std::int32_t new_n = n / 2;

		return matrix(data, orig_dim, m - new_m, n - new_n, offset_m + new_m, offset_n + new_n);
	};
};