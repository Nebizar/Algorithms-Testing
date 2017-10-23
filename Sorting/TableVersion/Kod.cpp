#include<iostream>
#include<ctime>
#include<cstdlib>
#include<vector>
#include<fstream>
#include<algorithm>
using namespace std;

const int N = 200000;
const unsigned int zakres = N + 1;
int tab[N];
int tab2[N];
int tab3[N];
int tab4[N];
int pom[zakres];
int back[N];


//liczenie czasu od uruchomienia programu
double obliczSekundy(clock_t czas)
{
	return static_cast < double >(czas) / CLOCKS_PER_SEC;
}

// SORTOWANIA PROSTE
//sortowanie-SelectionSort
void SelectionSort(int tab[])
{
	int min = 0;
	int miejsce, pom;
	for (int i = 0; i<N; i++)
	{
		min = tab[i];
		for (int j = i + 1; j<N; j++)
		{
			if (tab[j]<min)
			{
				min = tab[j];
				miejsce = j;
			}
		}
		pom = min;
		tab[miejsce] = tab[i];
		tab[i] = min;
	}
}

//sortowanie-InsertionSort
void InsertionSort(int tab[])
{
	int pom, j;
	for (int i = 0; i < N; i++)
	{
		j = i;
		while (j > 0 && tab[j] < tab[j - 1])
		{
			pom = tab[j];
			tab[j] = tab[j - 1];
			tab[j - 1] = pom;
			j--;
		}
	}
}

//sortowanie-BubbleSort
void BubbleSort(int tab[])
{
	int pom;
	for (int i = 0; i<N - 1; i++)
	{
		for (int j = 0; j<(N - i - 1); j++)
		{
			if (tab[j]>tab[j + 1])
			{
				pom = tab[j];
				tab[j] = tab[j + 1];
				tab[j + 1] = pom;
			}
		}
	}
}
//pokazywanie vectora liczb
void pokazuj(int tab[])
{
	for (int i = 0; i < N; i++)
	{
		cout << tab[i] << "\t";
	}
	cout << endl << endl;
}







//sortowanie-HeapSort
void Swap(int Heap[], int i, int j)
{
	if (i == j)
		return;

	int temp;
	temp = Heap[i];
	Heap[i] = Heap[j];
	Heap[j] = temp;
}

void HeapBuilder(int Heap[], int heapSize, int Current)
{
	int i, j;

	j = Current;
	do
	{
		i = j;
		if (((2 * i + 1)<heapSize) && Heap[j]<Heap[2 * i + 1] && Heap[j]<Heap[2 * i + 2])
		{
			if (Heap[2 * i + 1]<Heap[2 * i + 2])
				j = 2 * i + 1;
			else
				j = 2 * i + 2;
		}
		if (((2 * i + 1)<heapSize) && Heap[j]<Heap[2 * i + 1])
			j = 2 * i + 1;
		if (((2 * i + 2)<heapSize) && Heap[j]<Heap[2 * i + 2])
			j = 2 * i + 2;

		Swap(Heap, i, j);
	} while (i != j);
}

void MakeHeap(int tab[])
{
	for (int i = N - 1; i >= 0; --i)
	{
		HeapBuilder(tab, N, i);
	}
}

void HeapSort(int Heap[])
{
	MakeHeap(Heap);
	for (int i = N - 1; i>0; --i)
	{
		Swap(Heap, i, 0);
		HeapBuilder(Heap, i, 0);
	}
}

//sortowanie-MergeSort
void merge(int array[], int low, int pivot, int high)
{
	int temp[N];
	int h, i, j, k;
	h = low;
	i = low;
	j = pivot + 1;

	while ((h <= pivot) && (j <= high))
	{
		if (array[h] <= array[j])
		{
			temp[i] = array[h];
			h++;
		}
		else
		{
			temp[i] = array[j];
			j++;
		}
		i++;
	}
	if (h > pivot)
	{
		for (k = j; k <= high; k++)
		{
			temp[i] = array[k];
			i++;
		}
	}
}

void mergeSort(int array[], int low, int high) 
{
	if (low < high) {
		int middle = (low + high) / 2;
		mergeSort(array, low, middle);
		mergeSort(array, middle + 1, high);
		merge(array, low, middle, high);
	}
}

//sortowanie-QuickSort-PIVOT
void quicksort(int lewy, int prawy, int dane[])
{
	int i = lewy, j = prawy;
	int sr = dane[(lewy + prawy) / 2];

	do
	{
		while (dane[i]<sr)i++;
		while (dane[j]>sr)j--;
		if (i <= j)
		{
			int pom = dane[i];
			dane[i] = dane[j];
			dane[j] = pom;
			i++;
			j--;
		}
	} while (i <= j);
	if (lewy<j)quicksort(lewy, j, dane);
	if (prawy>i)quicksort(i, prawy, dane);
}

//sortowanie-QuickSort-PIERWSZY EL.
int partition(int *a, int start, int end)

{

	int pivot = a[start], p1 = start + 1, i, temp;

	for (i = start + 1; i <= end; i++)
	{

		if (a[i]<pivot)
		{
			if (i != p1)
			{
				temp = a[p1];
				a[p1] = a[i];
				a[i] = temp;
			}   p1++;
		}
	}

	a[start] = a[p1 - 1];
	a[p1 - 1] = pivot;

	return p1 - 1;
}

void quicksortL(int *a, int start, int end)
{
	int p1;
	while (start < end)
	{
		p1 = partition(a, start, end);
		if ((p1 - start) <= (end - p1))
		{
			quicksortL(a, start, p1 - 1);
			start += 1;
		}
		else
		{
			quicksortL(a, p1, end);
			end -= 1;
		}
	}
}
//sortowanie-CountSort
void countsort()
{
	for (int i = 0; i < N; i++)
	{
		pom[tab[i]]++;
	}
	for (int i = 1; i < zakres; i++)
	{
		pom[i] += pom[i - 1];
	}
	for (int i = N - 1; i >= 0; i--)
	{
		back[pom[tab[i]] - 1] = tab[i];
		pom[tab[i]]--;
	}
}

void zad1part1()
{
	ofstream plik1;
	plik1.open("dane.txt", ios::out|ios::app);
	plik1 << N << "\t";
	double start, stop;
	srand(time(NULL));
	for (int i = 0; i < N; i++)
	{
	tab[i] = rand() % N;
	}
	for (int i = 0; i < N; i++)
	{
	tab2[i] = tab[i];
	}
	for (int i = 0; i < N; i++)
	{
	tab3[i] = tab[i];
	}
	start = obliczSekundy(clock());
	SelectionSort(tab);
	stop = obliczSekundy(clock()) - start;
	plik1 << stop << "\t";
	start = 0;
	stop = 0;
	start = obliczSekundy(clock());
	InsertionSort(tab2);
	stop = obliczSekundy(clock()) - start;
	plik1 << stop << "\t";
	start = 0;
	stop = 0;
	start = obliczSekundy(clock());
	BubbleSort(tab3);
	stop = obliczSekundy(clock()) - start;
	plik1 << stop << "\n";
	start = 0;
	stop = 0;
}

void zad1part2()
{
	ofstream plik1;
	plik1.open("dane2.txt", ios::out | ios::app);
	plik1 << N << "\t";
	double start, stop;
	srand(time(NULL));
	for (int i = 0; i < N; i++)
	{
	tab[i] = rand() % N;
	}for (int i = 0; i < N; i++)
	{
	tab2[i] = tab[i];
	}
	for (int i = 0; i < N; i++)
	{
	tab3[i] = tab[i];
	}
	for (int i = 0; i < N; i++)
	{
	tab4[i] = tab[i];
	}
	start = obliczSekundy(clock());
	HeapSort(tab2);
	stop = obliczSekundy(clock()) - start;
	plik1 << stop << "\t";
	start = 0;
	stop = 0;
	start = obliczSekundy(clock());
	mergeSort(tab3,0,N-1);
	stop = obliczSekundy(clock()) - start;
	plik1 << stop << "\t";
	start = 0;
	stop = 0;
	start = obliczSekundy(clock());
	quicksort(0,N-1,tab4);
	stop = obliczSekundy(clock()) - start;
	plik1 << stop << "\t";
	start = 0;
	stop = 0;
	start = obliczSekundy(clock());
	countsort();
	stop = obliczSekundy(clock()) - start;
	plik1 << stop << "\n";
	start = 0;
	stop = 0;
}

void zad2()
{
	ofstream plik1;
	plik1.open("dane3.txt", ios::out | ios::app);
	plik1 << N << "\t";
	double start, stop;
	srand(time(NULL));
	for (int i = 0; i < N; i++)
	{
	tab[i] = rand() % N / 100;
	}
	for (int i = 0; i < N; i++)
	{
	tab4[i] = tab[i];
	}
	start = obliczSekundy(clock());
	quicksort(0, N - 1, tab4);
	stop = obliczSekundy(clock()) - start;
	plik1 << stop << "\t";
	start = 0;
	stop = 0;
	start = obliczSekundy(clock());
	countsort();
	stop = obliczSekundy(clock()) - start;
	plik1 << stop << "\n";
	start = 0;
	stop = 0;
}

void zad3()
{
	ofstream plik1;
	plik1.open("danetest.txt", ios::out | ios::app);
	plik1 << N << "\t";
	double start, stop;
	srand(time(NULL));
	for (int i = 0; i < N; i++)
	{
		tab[i] = i;
	}
	for (int i = 0; i < N; i++)
	{
		tab2[i] = tab[i];
	}
	for (int i = 0; i < N; i++)
	{
		tab3[i] = tab[i];
	}
	start = obliczSekundy(clock());
	quicksort(0, N - 1, tab);
	stop = obliczSekundy(clock()) - start;
	plik1 << stop << "\t";
	start = obliczSekundy(clock());
	quicksortL(tab2, 0, N - 1);
	stop = obliczSekundy(clock()) - start;
	plik1 << stop << "\t";
	start = obliczSekundy(clock());
	InsertionSort(tab3);
	stop = obliczSekundy(clock()) - start;
	plik1 << stop << "\n";
	start = 0;
	stop = 0;
}




int main()
{
	//SORTOWANIA PROSTE
	zad1part1();
	//SORTOWANIA ZLOZONE
	zad1part2();
	//ZAD 2
	zad2();
	//ZAD 3
	/zad3();
	/*double start, stop;
	srand(time(NULL));
	for (int i = 0; i < N; i++)
	{
		tab[i] = i;
	}
	quicksort(0, N - 1, tab);*/
	system("PAUSE");
	return 0;
}