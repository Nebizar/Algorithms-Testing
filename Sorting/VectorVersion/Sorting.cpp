#include<iostream>
#include<ctime>
#include<cstdlib>
#include<vector>
#include<fstream>
#include<algorithm>
using namespace std;

clock_t clock(void);

vector <int> copy( vector <int> original)
{
	vector <int> dane;
	for (int i = 0; i < original.size(); i++)
	{
		dane.push_back(original[i]);
	}
	return dane;
}

//liczenie czasu od uruchomienia programu
double obliczSekundy(clock_t czas)
{
	return static_cast < double >(czas) / CLOCKS_PER_SEC;
}

//pokazywanie vectora liczb
void pokazuj(vector <int> tab)
{
	for (int i = 0; i < tab.size(); i++)
	{
		cout << tab[i] << "\t";
	}
	cout << endl << endl;
}

//SORTOWANIA PROSTE
//sortowanie-SelectionSort
void SelectionSort(vector <int> &tab)
{
	int min = 0;
	int miejsce, pom;
	for (int i = 0; i<tab.size(); i++)
	{
		min = tab[i];
		for (int j = i + 1; j<tab.size(); j++)
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
void InsertionSort(vector <int> tab)
{
	int pom, j;
	for (int i = 0; i < tab.size(); i++)
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
void BubbleSort(vector <int> &tab)
{
	int pom;
	for (int i = 0;i<tab.size() - 1; i++)
	{
		for(int j=0;j<(tab.size()-i-1);j++)
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

//funkcja testujaca sortowania proste dla tych samych vectorów i zapisuj¹ca czas ich wykonania
void testSort()
{
	ofstream save;
	save.open("dane.txt", ios::out);
	save << "Quant \t SS \t IS \t BS \n";
	double start, stop;
	vector <int> dane;
	vector <int> backup;
	srand(time(NULL));
	for (int i = 2000; i <= 20000; i = i + 2000)
	{
		dane.reserve(i);
		backup.reserve(i);
		for (int j = 0; j <= i; j++)
		{
			dane.push_back(rand() % i+1);
		}
		backup = copy(dane);
		start = obliczSekundy(clock());
		SelectionSort(dane);
		stop = obliczSekundy(clock()) - start;
		save << i << "\t" << stop << "\t";
		dane.clear();
		dane = copy(backup);
		start = obliczSekundy(clock());
		InsertionSort(dane);
		stop = obliczSekundy(clock()) - start;
		save << stop << "\t";
		dane.clear();
		dane = copy(backup);
		start = obliczSekundy(clock());
		BubbleSort(dane);
		stop = obliczSekundy(clock()) - start;
		save << stop << "\n";
		start = 0;
		stop = 0;
		dane.clear();
		backup.clear();
	}
	save.close();
}

//SORTOWANIA ZAAWANSOWANE

//sortowanie-HeapSort
void Swap(vector<int>& Heap, int i, int j)
{
	if (i == j)
		return;

	int temp;
	temp = Heap[i];
	Heap[i] = Heap[j];
	Heap[j] = temp;
}

void HeapBuilder(vector<int>& Heap, int heapSize, int Current)
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

void MakeHeap(vector<int>& Heap)
{
	for (int i = Heap.size() - 1; i >= 0; --i)
	{
		HeapBuilder(Heap, Heap.size(), i);
	}
}

void HeapSort(vector<int>& Heap)
{
	MakeHeap(Heap);
	for (int i = Heap.size() - 1; i>0; --i)
	{
		Swap(Heap, i, 0);
		HeapBuilder(Heap, i, 0);
	}
}

//sortowanie-MergeSort
void merge(vector<int>& aVector, int low, int middle, int high) 
{
	vector <int> temp;
	for (int i = 0; i < aVector.size(); i++)
	{
		temp.push_back(0);
	}
	for (int i = low; i <= high; i++) {
		temp[i] = aVector[i];
	}
	int i = low;
	int j = middle + 1;
	int k = low;

	while (i <= middle && j <= high) {
		if (temp[i] <= temp[j]) {
			aVector[k] = temp[i];
			++i;
		}
		else {
			aVector[k] = temp[j];
			++j;
		}
		++k;
	}
	while (i <= middle) {
		aVector[k] = temp[i];
		++k;
		++i;
	}
	temp.clear();
}

void mergeSort(vector<int>& aVector, int low, int high) {
	if (low < high) {
		int middle = (low + high) / 2;
		mergeSort(aVector, low, middle);
		mergeSort(aVector, middle + 1, high);
		merge(aVector, low, middle, high);
	}
}

//sortowanie-QuickSort-PIVOT
void quicksort(int lewy, int prawy, vector <int>& dane)
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
void quicksortL(int lewy, int prawy, vector <int>& dane)
{
	int i = lewy, j = prawy;
	int sr = dane[0];

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

//sortowanie-CountSort
vector <int> countsort(vector <int>& tab, int zakres)
{
	vector <int> pom;
	vector <int> back;
	for (int i = 0; i < zakres; i++)
	{
		pom.push_back(0);
	}
	for (int i = 0; i < tab.size(); i++)
	{
		back.push_back(0);
	}
	for (int i = 0; i < tab.size(); i++)
	{
		pom[tab[i]]++;
	}
	for (int i = 1; i < pom.size(); i++)
	{
		pom[i] += pom[i - 1];
	}
	for (int i = tab.size() - 1; i >= 0; i--)
	{
		back[pom[tab[i]] - 1] = tab[i];
		pom[tab[i]]--;
	}
	return back;
}

//funkcja testujaca sortowania zaawansowane dla tych samych vectorów i zapisujaca czas ich wykonania
void testSort2()
{
	double start = 0, stop = 0;
	vector <int> dane;
	vector <int> backup;
	ofstream plik1;
	plik1.open("dane2.txt", ios::out);
	plik1 << "Quant \t HS \t MS \t QS \t CS \n";
	srand(time(NULL));
	for (int i = 2000; i <= 20000; i = i + 2000)
	{
		for (int j = 0; j <= i; j++)
		{
			dane.push_back(rand() % (i+1));
		}
		plik1 << i << "\t";
		backup = copy(dane);
		start = obliczSekundy(clock());
		HeapSort(dane);
		stop = obliczSekundy(clock()) - start;
		plik1 << stop << "\t";
		dane.clear();
		dane = copy(backup);
		start=obliczSekundy(clock());
		mergeSort(dane, 0, dane.size() - 1 );
		stop = obliczSekundy(clock()) - start;
		plik1 << stop << "\t";
		dane.clear();
		dane = copy(backup);
		start = obliczSekundy(clock());
		quicksort(0, dane.size() - 1, dane);
		stop = obliczSekundy(clock()) - start;
		plik1 << stop << "\t";
		dane.clear();
		dane = copy(backup);
		start = obliczSekundy(clock());
		dane=countsort(dane, i+1);
		stop = obliczSekundy(clock()) - start;
		plik1 << stop << "\n";
		dane.clear();
		backup.clear();
	}
	plik1.close();
}
//AISD -Zad3
void zad3()
{
	ofstream plik;
	vector <int> dane;
	vector <int> dane2;
	vector <int> backup;
	vector <int> backup2;
	double start = 0, stop = 0;
	plik.open("dane3.txt", ios::out);
	plik << "Rozklad losowy \n";
	plik << "Quant \t QS-sr \t QS-pi \t IS \n";
	for (int i = 4000; i <= 40000; i = i + 4000)
	{
		dane.reserve(i);
		backup.reserve(i);
		for (int j = 0; j < i; j++)
		{
			dane.push_back(rand() % i);
		}
		backup = copy(dane);
		start = obliczSekundy(clock());
		quicksort(0, dane.size() - 1, dane);
		stop = obliczSekundy(clock()) - start;
		plik << i << "\t" << stop << "\t";
		dane.clear();
		dane = copy(backup);
		start = obliczSekundy(clock());
		quicksortL(0, dane.size() - 1, dane);
		stop = obliczSekundy(clock()) - start;
		plik << stop << "\t";
		dane.clear();
		dane = copy(backup);
		start = obliczSekundy(clock());
		InsertionSort(dane);
		stop = obliczSekundy(clock()) - start;
		plik << stop << "\n";
		backup.clear();
		dane.clear();
	}
	plik << "\n Rozklad rosnacy \n";
	plik << "Quant \t QS-sr \t QS-pi \t IS \n";
	for (int i = 4000; i <= 40000; i = i + 4000)
	{
		dane2.reserve(i);
		backup2.reserve(i);
		for (int j = 0; j < i; j++)
		{
			dane2.push_back(j);
		}
		backup2 = copy(dane2);
		start = obliczSekundy(clock());
		quicksort(0, dane2.size() - 1, dane2);
		stop = obliczSekundy(clock()) - start;
		plik << i << "\t" << stop << "\t";
		dane2.clear();
		dane2 = copy(backup2);
		start = obliczSekundy(clock());
		quicksortL(0, dane2.size() - 1, dane2);
		stop = obliczSekundy(clock()) - start;
		plik << stop << "\t";
		dane2.clear();
		dane2 = copy(backup2);
		start = obliczSekundy(clock());
		InsertionSort(dane2);
		stop = obliczSekundy(clock()) - start;
		plik << stop << "\n";
		dane.clear();
		backup.clear();
	}
	plik.close();
}
//AISD -Zad4
void zad4()
{
	ofstream plik;
	vector <int> dane;
	vector <int> dane2;
	vector <int> backup;
	double start = 0, stop = 0;
	plik.open("dane4.txt", ios::out);
	plik << "n*100 - zakres \n";
	plik << "Quant \t QS \t CS \n";
	for (int i = 10000;i <= 100000;i = i + 10000)
	{
		dane.reserve(i);
		backup.reserve(i);
		for (int j = 0;j < i;j++)
		{
			dane.push_back(rand() % (i * 100));
		}
		backup = copy(dane);
		start = obliczSekundy(clock());
		quicksort(0, dane.size()-1, dane);
		stop = obliczSekundy(clock()) - start;
		plik <<i<<"\t"<< stop << "\t";
		dane.clear();
		dane = copy(backup);
		start = obliczSekundy(clock());
		dane=countsort(dane, i*100+1);
		stop = obliczSekundy(clock()) - start;
		plik << stop << "\n";
		dane.clear();
		backup.clear();
	}
	backup.clear();
	plik << "\n n/100 - zakres \n";
	plik << "Quant \t QS \t CS \n";
	for (int i = 10000;i <= 100000;i = i + 10000)
	{
		dane2.reserve(i);
		for (int j = 0;j < i;j++)
		{
			dane2.push_back(rand() % (i / 100));
		}
		backup = copy(dane2);
		start = obliczSekundy(clock());
		quicksort(0, dane2.size()-1, dane2);
		stop = obliczSekundy(clock()) - start;
		plik << i << "\t" << stop << "\t";
		dane2.clear();
		dane2 = copy(backup);
		start = obliczSekundy(clock());
		dane = countsort(dane, i/100 + 1);
		stop = obliczSekundy(clock()) - start;
		plik << stop << "\n";
		backup.clear();
		dane.clear();
	}
	plik.close();
}
	

//g³ówna funkcja wykonuj¹ca programu
int main()
{
	//testSort();
	//testSort2();
	//zad3();
	//zad4();
	srand(time(NULL));
	vector <int> data;
	for (int i = 0; i < 100; i++)
		data.push_back(rand() % 100);
	pokazuj(data);
	mergeSort(data, 0, data.size() - 1);
	pokazuj(data);
	system("PAUSE");
	return 0;
}
