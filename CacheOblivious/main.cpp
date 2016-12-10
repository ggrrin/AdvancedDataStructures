#include "main.h" 

//#define SIM

void report_swap(std::int32_t i1, std::int32_t j1, std::int32_t i2, std::int32_t j2)
{
	printf("X %d %d %d %d\n", i1, j1, i2, j2);
}

void trivial_transpose(matrix mat)
{
	if (mat.m != mat.n)
		throw 0;
	
	for (std::int32_t i = 0; i < mat.m; i++)
	{
		for (std::int32_t j = 0; j < i; j++)
		{
#ifdef SIM
			report_swap(mat.get_i_glob(i), mat.get_j_glob(j),
				mat.get_i_glob(j), mat.get_j_glob(i));
#else
			std::int32_t temp = mat.at(i, j);
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
		for (std::int32_t i = 0; i < mat1.m; i++)
		{
			for (std::int32_t j = 0; j < mat1.n; j++)
			{
#ifdef SIM
				report_swap(mat1.get_i_glob(i), mat1.get_j_glob(j),
					mat2.get_i_glob(j), mat2.get_j_glob(i));
#else
				std::int32_t temp = mat1.at(i, j);
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
	const std::int32_t k = 5;
	const std::int32_t k2= k*k;
	std::int32_t data1[k2] = {
		1, 2, 3, 4, 5, 
		6, 7, 8, 9, 10, 
		11, 12, 13, 14, 15, 
		16, 17, 18, 19, 20, 
		21, 22, 23, 24, 25
	};

	std::int32_t data2[k2] = {
		1, 2, 3, 4, 5, 
		6, 7, 8, 9, 10, 
		11, 12, 13, 14, 15, 
		16, 17, 18, 19, 20, 
		21, 22, 23, 24, 25
	};

	trivial_transpose(matrix(data1, k, k, k, 0, 0));

	transpose_on_diagonal(matrix(data2, k, k, k, 0, 0));
}

void test2(std::int32_t k)
{
	std::int32_t k2= k*k;
	std::int32_t* data1 = new std::int32_t[k2];
	std::int32_t* data2 = new std::int32_t[k2];

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


void simulate(std::int32_t max_size_MB)
{
	std::int32_t element_size = sizeof(std::int32_t);
	std::uint32_t max_elements = max_size_MB * 1024 * 1024 /  element_size;

	//N = floor(2 ^ {k / 9}) pro k >= 54, 55, 56
	std::int32_t max_k = log2(max_elements) * 9;

	for (std::int32_t k = 54; k < max_k; k++)
	{
		std::int32_t N = exp2(k / 9);

#ifdef SIM
		printf("N %d\n", N);
		transpose_on_diagonal(matrix(nullptr, N, N, N, 0, 0));
		printf("E\n");
#else 
		std::int32_t* data = new std::int32_t[N*N];
		transpose_on_diagonal(matrix(data, N, N, N, 0, 0));
		delete[] data;
#endif 
	}
}

int main(int argc, char* argv[])
{
	//test1();
	//test2(293);
	//test2(10001);

	if (argc == 2)
	{
		int mb_size = atoi(argv[1]);
		simulate(mb_size); 
	}
	else
	{
		printf("invalid arguments");
	}
	return 0;
}