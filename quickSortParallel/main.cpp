#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <malloc.h>
#include <random>

void quickSort(int* a, int left, int right)
{
	if (left > right) return;
	int pivot = a[(left + right) / 2];
	int i = left;
	int j = right;
	while (i <= j)
	{
		while (a[i] < pivot) i++;
		while (a[j] > pivot) j--;
		if (i <= j)
		{
			int temp = a[i];
			a[i] = a[j];
			a[j] = temp;
			i++;
			j--;
		}
	}
	quickSort(a, left, j);
	quickSort(a, i, right);
}

void quickSortOmp(int* a, int left, int right)
{
	if (left > right) return;
	int pivot = a[(left + right) / 2];
	int i = left;
	int j = right;
	while (i <= j)
	{
		while (a[i] < pivot) i++;
		while (a[j] > pivot) j--;
		if (i <= j)
		{
			int temp = a[i];
			a[i] = a[j];
			a[j] = temp;
			i++;
			j--;
		}
	}
	#pragma omp task shared(a)
	{
		quickSort(a, left, j);
	}
	#pragma omp task shared(a)
	{
		quickSort(a, i, right);
	}
	#pragma omp taskwait
}

int main()
{
	int n, * a, *b;
	FILE* f;

	f = fopen("input.txt", "r");
	fscanf(f, "%d", &n);
	a = (int*)malloc(n * sizeof(int));
	b = (int*)malloc(n * sizeof(int));
	for (int i = 0; i < n; i++)
	{
		fscanf(f, "%d", &a[i]);
		b[i] = a[i];
	}
	fclose(f);
	double time = omp_get_wtime();
	quickSort(b, 0, n - 1);
	time = omp_get_wtime() - time;
	printf("Time: %f", time);
	printf("\n");
	
	time = omp_get_wtime();
#pragma omp parallel shared(a)
	{
#pragma omp single nowait 
		{
			quickSortOmp(a, 0, n - 1);
		}
	}
	time = omp_get_wtime() - time;
	printf("Time: %f", time);
	printf("\n");

	f = fopen("output.txt", "w");
	for (int i = 0; i < n; i++)
	{
		fprintf(f, "%d ", a[i]);
	}
	fclose(f);
	printf("\n");
	int flag = 1;
	for (int i = 0; i < n - 1; i++)
	{
		if (a[i] > a[i + 1])
		{
			flag = 0;
		}
	}
	if (flag == 1) printf("Ok\n");
	else printf("Bad\n");

	free(a);
	return 0;
}