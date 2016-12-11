#include "main.h" 

//#define SIM

void report_swap(my_int i1, my_int j1, my_int i2, my_int j2)
{
	printf("X %lld %lld %lld %lld\n", i1, j1, i2, j2);
}

void trivial_transpose(matrix mat)
{
	if (mat.m != mat.n)
		throw 0;
	
	for (my_int i = 0; i < mat.m; i++)
	{
		for (my_int j = 0; j < i; j++)
		{
#ifdef SIM
			report_swap(mat.get_i_glob(i), mat.get_j_glob(j),
				mat.get_i_glob(j), mat.get_j_glob(i));
#else
			my_val temp = mat.at(i, j);
			mat.at(i, j) = mat.at(j, i);
			mat.at(j, i) = temp;
#endif 
		}
	}
}

void transpose_and_swap(matrix mat1, matrix mat2)
{
	if (mat1.m != mat2.n || mat1.n != mat2.m)
		throw 0;

	if (mat1.size() <= SMALL)
	{
		for (my_int i = 0; i < mat1.m; i++)
		{
			for (my_int j = 0; j < mat1.n; j++)
			{
#ifdef SIM
				report_swap(mat1.get_i_glob(i), mat1.get_j_glob(j),
					mat2.get_i_glob(j), mat2.get_j_glob(i));
#else
				my_val temp = mat1.at(i, j);
				mat1.at(i, j) = mat2.at(j, i);
				mat2.at(j, i) = temp;
#endif

			}
		}
	}
	else
	{
		matrix a11 = mat1.get_m11();
		matrix a12 = mat1.get_m12(); 
		matrix a21 = mat1.get_m21(); 
		matrix a22 = mat1.get_m22(); 

		matrix b11 = mat2.get_m11();
		matrix b12 = mat2.get_m12(); 
		matrix b21 = mat2.get_m21(); 
		matrix b22 = mat2.get_m22(); 

		transpose_and_swap(a11, b11);
		transpose_and_swap(a12, b21);
		transpose_and_swap(a21, b12);
		transpose_and_swap(a22, b22);
	}
}

void transpose_on_diagonal(matrix mat)
{
	if (mat.size() <= SMALL)
	{
		trivial_transpose(mat);
	}
	else
	{
		matrix a11 = mat.get_m11();
		matrix a12 = mat.get_m12(); 
		matrix a21 = mat.get_m21(); 
		matrix a22 = mat.get_m22(); 

		transpose_on_diagonal(a11);
		transpose_on_diagonal(a22);
		transpose_and_swap(a12, a21);
	}
}

void test1()
{
	const my_int k = 5;
	const my_int k2= k*k;
	my_val data1[k2] = {
		1, 2, 3, 4, 5, 
		6, 7, 8, 9, 10, 
		11, 12, 13, 14, 15, 
		16, 17, 18, 19, 20, 
		21, 22, 23, 24, 25
	};

	my_val data2[k2] = {
		1, 2, 3, 4, 5, 
		6, 7, 8, 9, 10, 
		11, 12, 13, 14, 15, 
		16, 17, 18, 19, 20, 
		21, 22, 23, 24, 25
	};

	trivial_transpose(matrix(data1, k, k, k, 0, 0));

	transpose_on_diagonal(matrix(data2, k, k, k, 0, 0));
}

void test2(my_int k)
{
	my_int k2= k*k;
	my_val* data1 = new my_val[k2];
	my_val* data2 = new my_val[k2];

	for (size_t i = 0; i < k2; i++)
		data1[i] = data2[i] = i + 1;

	auto t1 = std::chrono::steady_clock::now();
	trivial_transpose(matrix(data1, k, k, k, 0, 0));
	auto t2 = std::chrono::steady_clock::now();

	auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
	printf("%s %ld ms\n", "trivial", diff.count());


	t1 = std::chrono::steady_clock::now();
	transpose_on_diagonal(matrix(data2, k, k, k, 0, 0));
	t2 = std::chrono::steady_clock::now();

	diff = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
	printf("%s %ld ms\n", "cache-oblivious", diff.count());

	for (size_t i = 0; i < k2; i++)
		if (data1[i] != data2[i])
			throw 0;

	delete[] data1;
	delete[] data2;
}


void simulate(bool triv, my_int max_size_MB, my_int rep_count)
{
	my_int max_elements = max_size_MB * 1024 * 1024 / sizeof(my_val);
	my_val* data = new my_val[max_elements];
	my_int sq = sqrt(max_elements);

	for (my_int k = 54; ; k++)
	{
		my_int N = exp2(k / 9.0);

		if (N*N >= max_elements)
			break;

#ifdef SIM
		printf("N %d\n", N);
		transpose_on_diagonal(matrix(nullptr, N, N, N, 0, 0));
		printf("E\n");
#else 

		auto t1 = std::chrono::steady_clock::now();
		for (my_int rep = 0; rep < rep_count; rep++	)
		{
			if(!triv)
				transpose_on_diagonal(matrix(data, N, N, N, 0, 0));
			else
				trivial_transpose(matrix(data, N, N, N, 0, 0));
		}

		auto t2 = std::chrono::steady_clock::now();

		auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
		printf("%lld %f\n", N, diff.count() / (double)rep_count);

#endif 
	}
	delete[] data;
}

int main(int argc, char* argv[])
{
	//test1();
	//test2(293);
	//test2(10001);

	//test3
	//transpose_on_diagonal(matrix(nullptr,5 , 5, 5, 0, 0));
	//return 0;

	if (argc == 4 && (argv[0] == "t" || argv[0] == "n"))
	{
		int mb_size = atoi(argv[2]);
		int measures_count = atoi(argv[3]);
		simulate(argv[0] == "t", mb_size, measures_count); 
	}
	else
	{
		printf("usage: main.out t|n <max_memory_usage_MB> <measures_count_per_N>\nt ... trivial\nn ... normal");
	}
	return 0;
}
