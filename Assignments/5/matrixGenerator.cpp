#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <random>
#include <string>
#include <omp.h>

using namespace std;

const string QUERY_PRINT = "print";
const string QUERY_GET_MATRICES_FROM_TERMINAL = "init";
const string QUERY_GET_MATRICES_FROM_FILE = "load";
const string QUERY_SAVE_MATRICES_TO_FILE = "save";
const string QUERY_GENERATE_RANDOM_MATRICES = "gen_random";
const string QUERY_HELP = "help";
const string QUERY_CLEAR = "clear";
const string QUERY_EXIT = "exit";

const int MIN_N = 1;
const int MAX_N = 20000;
const int MIN_VAL = -50000;
const int MAX_VAL = 50000;

void printHelp();
void clearScreen();

void getMatricesFromTerminal(int**& a, int**& b, int& n, int& k, int& m);
void saveMatrices(int** a, int** b, int n, int k, int m, char path[300]);
void loadMatrices(int**& a, int**& b, int& n, int& k, int& m, char path[300]);
void generateRandomMatrices(int**& a, int**& b, int& n, int& k, int& m);

void printMatrices(int** a, int** b, int n, int k, int m);
void deleteAllIfNeeded(int**& a, int**& b, int& n, int& k, int& m);

int main()
{
    char c_query[50] = "";
	string query(c_query);
	printHelp();

	int n = -1, k = -1, m = -1;
	int** a = nullptr, ** b = nullptr;

	while (query.compare(QUERY_EXIT))
	{
		printf("\nEnter any command(to see all possible commands type help): ");

		scanf("%s", c_query);
		query.assign(c_query);

		if (!query.compare(QUERY_GET_MATRICES_FROM_TERMINAL))
		{
			deleteAllIfNeeded(a, b, n, k, m);
			getMatricesFromTerminal(a, b, n, k, m);
		}
		else if (!query.compare(QUERY_GET_MATRICES_FROM_FILE))
		{
			char path[300] = "";
			scanf("%s", path);

			deleteAllIfNeeded(a, b, n, k, m);
			loadMatrices(a, b, n, k, m, path);
		}
		else if (!query.compare(QUERY_SAVE_MATRICES_TO_FILE))
		{
			if (n == -1)
			{
				printf("\nError: Matrices are not initialized!\n\n");
				continue;
			}

			char path[300] = "";
			scanf("%s", path);

			saveMatrices(a, b, n, k, m, path);
		}
		else if (!query.compare(QUERY_GENERATE_RANDOM_MATRICES))
		{
			int tempN;
			scanf("%d", &tempN);
			if (tempN < MIN_N || tempN > MAX_N)
			{
				printf("\nError: <N> should be at least %d and at most %d!\n\n", MIN_N, MAX_N);
				continue;
			}

			int tempK;
			scanf("%d", &tempK);
			if (tempK < MIN_N || tempK > MAX_N)
			{
				printf("\nError: <K> should be at least %d and at most %d!\n\n", MIN_N, MAX_N);
				continue;
			}

			int tempM;
			scanf("%d", &tempM);
			if (tempM < MIN_N || tempM > MAX_N)
			{
				printf("\nError: <M> should be at least %d and at most %d!\n\n", MIN_N, MAX_N);
				continue;
			}
			deleteAllIfNeeded(a, b, n, k, m);
			n = tempN;
			k = tempK;
			m = tempM;
			generateRandomMatrices(a, b, n, k, m);
		}
		else if (!query.compare(QUERY_PRINT))
		{
			if (n == -1)
			{
				printf("\nError: Matrices are not initialized!\n\n");
				continue;
			}

			printMatrices(a, b, n, k, m);
		}
		else if (!query.compare(QUERY_CLEAR))
		{
			clearScreen();
		}
		else if (!query.compare(QUERY_HELP))
		{
			printHelp();
		}
		else if (query.compare(QUERY_EXIT))
		{
			printf("\nError: Command not defined!\n\n");
		}
	}

	deleteAllIfNeeded(a, b, n, k, m);
}

void printHelp()
{
	printf("\n_______________HELP_______________\n");

	printf("\n\t* To input the matrices from terminal enter: \"%s\" without the quotations.\n"
		, QUERY_GET_MATRICES_FROM_TERMINAL.c_str());

	printf("\n\t* To read the matrices from a file with path <PATH> enter: \"%s <PATH>\" without the quotations.\n"
		, QUERY_GET_MATRICES_FROM_FILE.c_str());

	printf("\n\t* To save the matrices to a file with path <PATH> enter: \"%s <PATH>\" without the quotations.\n"
		, QUERY_SAVE_MATRICES_TO_FILE.c_str());

	printf("\n\t* To generate random matrices with size <N> x <K> and <K> x <M> enter: \"%s <N> <K> <M>\" without the quotations.\n"
		, QUERY_GENERATE_RANDOM_MATRICES.c_str());

	printf("\n\t* To print the matrices enter: \"%s\" without the quotations.\n", QUERY_PRINT.c_str());

	printf("\n\t* To print these information again enter: \"%s\" without the quotations.\n", QUERY_HELP.c_str());

	printf("\n\t* To clear the screen enter: \"%s\" without the quotations.\n", QUERY_CLEAR.c_str());

	printf("\n\t* To exit enter: \"%s\" without the quotations.\n", QUERY_EXIT.c_str());

	printf("\n");

	printf("\n+ While using \"%s\", or \"%s\" queries, <PATH> should not contain any whitespace.\n"
		, QUERY_GET_MATRICES_FROM_FILE.c_str(), QUERY_SAVE_MATRICES_TO_FILE.c_str());
	printf("\n+ When the matrices are too large don't use the \"%s\" query.\n"
		, QUERY_PRINT.c_str());
}

void clearScreen() {
	system("cls");
}

void getMatricesFromTerminal(int**& a, int**& b, int& n, int& k, int& m)
{
	while (n < MIN_N || n > MAX_N) {
		printf("Enter the height of the first matrix (it should be an integer not lower than %d or more than %d): "
			, MIN_N, MAX_N);
		scanf("%d", &n);
	}

	while (k < MIN_N || k > MAX_N) {
		printf("Enter the width of the first matrix(it should be an integer not lower than % d or more than % d) : "
			, MIN_N, MAX_N);
		scanf("%d", &k);
	}

	while (m < MIN_N || m > MAX_N) {
		printf("Enter the width of the second matrix(it should be an integer not lower than % d or more than % d) : "
			, MIN_N, MAX_N);
		scanf("%d", &m);
	}

	a = new int* [n];
	b = new int* [k];

	printf("\n__________First Matrix__________\n\n");
	for (int i = 0; i < n; ++i)
	{
		a[i] = new int[k];

		printf("+ %dth row of the first matrix:\n", i + 1);
		for (int j = 0; j < k; ++j)
		{
			printf("\t- enter the %dth element: ", j + 1);
			scanf("%d", a[i] + j);
		}
	}

	printf("\n__________Second Matrix__________\n\n");
	for (int i = 0; i < k; ++i)
	{
		b[i] = new int[m];

		printf("+ %dth row of the second matrix:\n", i + 1);
		for (int j = 0; j < m; ++j)
		{
			printf("\t- enter the %dth element: ", j + 1);
			scanf("%d", b[i] + j);
		}
	}

	printf("\n#### Matrices initialized succesfully! ####\n\n");
}

void saveMatrices(int** a, int** b, int n, int k, int m, char path[300])
{
	FILE* fp = fopen(path, "w+");
	fprintf(fp, "%d %d %d\n", n, k, m);

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < k; ++j)
			fprintf(fp, j == k - 1 ? "%d" : "%d ", a[i][j]);
		fprintf(fp, i == n - 1 ? "\n\n" : "\n");
	}

	for (int i = 0; i < k; ++i)
	{
		for (int j = 0; j < m; ++j)
			fprintf(fp, j == m - 1 ? "%d" : "%d ", b[i][j]);
		fprintf(fp, i == k - 1 ? "" : "\n");
	}

	fclose(fp);

	printf("\n#### Matrices saved succesfully! ####\n\n");
}

void loadMatrices(int**& a, int**& b, int& n, int& k, int& m, char path[300])
{

	FILE* fp = fopen(path, "r");
	fscanf(fp, "%d%d%d", &n, &k, &m);

	a = new int* [n];
	b = new int* [k];

	for (int i = 0; i < n; ++i)
	{
		a[i] = new int[k];
		for (int j = 0; j < k; ++j)
		{
			fscanf(fp, "%d", a[i] + j);
		}
	}

	for (int i = 0; i < k; ++i)
	{
		b[i] = new int[m];
		for (int j = 0; j < m; ++j)
			fscanf(fp, "%d", b[i] + j);
	}

	fclose(fp);

	printf("\n#### Matrices loaded succesfully! ####\n\n");
}

void generateRandomMatrices(int**& a, int**& b, int& n, int& k, int& m)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> distrib(MIN_VAL, MAX_VAL);

	a = new int* [n];
	b = new int* [k];
	for (int i = 0; i < n; ++i)
	{
		a[i] = new int[k];
		for (int j = 0; j < k; ++j)
		{
			a[i][j] = distrib(gen);
		}
	}

	for (int i = 0; i < k; ++i)
	{
		b[i] = new int[m];
		for (int j = 0; j < m; ++j)
		{
			b[i][j] = distrib(gen);
		}
	}

	printf("\n#### Random matrices generated succesfully! ####\n\n");
}

void printMatrices(int** a, int** b, int n, int k, int m)
{
	printf("Size = %d x %d || %d x %d\n", n, k, k, m);

	printf("A:\n");
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < k; ++j)
			printf("\t%d ", a[i][j]);
		printf("\n");
	}

	printf("\nB:\n");
	for (int i = 0; i < k; ++i)
	{
		for (int j = 0; j < m; ++j)
			printf("\t%d ", b[i][j]);
		printf("\n");
	}

	printf("\n#### Done ####\n\n");
}

void deleteAllIfNeeded(int**& a, int**& b, int& n, int& k, int& m)
{
	if (n != -1)
	{
		for (int i = 0; i < n; ++i)
		{
			delete[] a[i];
			a[i] = nullptr;
		}
		for (int i = 0; i < k; ++i)
		{
			delete[] b[i];
			b[i] = nullptr;
		}
		delete[] a;
		delete[] b;
		a = nullptr;
		b = nullptr;

		n = -1;
		k = -1;
		m = -1;
	}
}