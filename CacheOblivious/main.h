#include <cstdint>
#include <exception>
#include <chrono>
#include <stdio.h>
#include <math.h>
#include <cstdlib>

//velikost matice, ktera uz se transponuje trivialnim zpusobem
#define SMALL 16

typedef std::int64_t my_int;
typedef std::int32_t my_val;

//struktura reprezentujici rozmery a offsety sub-matice v cele matici
struct matrix
{
	my_val* data;
	my_int orig_dim, m, n, offset_m, offset_n;

	matrix(my_val* data_p, my_int orig_dim_p, my_int m_p, 
		my_int n_p, my_int offset_m_p, my_int offset_n_p) :
		data(data_p), orig_dim(orig_dim_p), m(m_p), n(n_p), 
		offset_m(offset_m_p), offset_n(offset_n_p)
	{ 
		if (m <= 0 || n <= 0)
			throw 0;//std::invalid_argument("Dimension has to by > 0!!");
	};

	//vrati pocet prvku matice reprezentovany vsemi daty (cili ne pocet prvku sub-matice)
	my_int get_data_length() const
	{
		return orig_dim * orig_dim;
	}

	//vrati globalni i z lokalniho i
	my_int get_i_glob(my_int i) const
	{
		return offset_m + i;
	}

	//vrati globalni j z lokalniho j
	my_int get_j_glob(my_int j) const
	{
		return offset_n + j;
	}

	//vrati referenci na prvek v matici. indexy jsou zero based!!!!!
	my_val& at(my_int i, my_int j)
	{
		my_int g_i = get_i_glob(i);
		my_int g_j = get_j_glob(j);
		
		return data[g_i * orig_dim + g_j];
	}

	//vrati velikost matice
	my_int size() const
	{
		return m * n;
	}

	//vrati sub-matici 11
	matrix get_m11() const 
	{
		my_int new_m = m / 2;
		my_int new_n = n / 2;

		return matrix(data, orig_dim, new_m, new_n, offset_m, offset_n);
	};

	//vrati sub-matici 12
	matrix get_m12() const 
	{
		my_int new_m = m / 2;
		my_int new_n = n / 2;

		return matrix(data, orig_dim, new_m, n - new_n, offset_m, offset_n + new_n);
	};

	//vrati sub-matici 21
	matrix get_m21() const 
	{
		my_int new_m = m / 2;
		my_int new_n = n / 2;

		return matrix(data, orig_dim, m - new_m, new_n, offset_m + new_m, offset_n);
	};

	//vrati sub-matici 22
	matrix get_m22() const 
	{
		my_int new_m = m / 2;
		my_int new_n = n / 2;

		return matrix(data, orig_dim, m - new_m, n - new_n, offset_m + new_m, offset_n + new_n);
	};
};
