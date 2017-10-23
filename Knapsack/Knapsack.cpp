#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <ratio>
using namespace std;

int tab[] = { 5,8,11,14,17,20,23,26,29,32,35 };
int maxi = 0;

double obliczSekundy(clock_t czas)
{
	return static_cast < double >(czas) / CLOCKS_PER_SEC;
}

struct item
{
	int value;
	int size;
	float vs;
};

void quicksortSize(int lewy, int prawy, item *dane)
{
	int i = lewy, j = prawy;
	int sr = dane[(lewy + prawy) / 2].size;

	do
	{
		while (dane[i].size<sr)i++;
		while (dane[j].size>sr)j--;
		if (i <= j)
		{
			item pom = dane[i];
			dane[i] = dane[j];
			dane[j] = pom;
			i++;
			j--;
		}
	} while (i <= j);
	if (lewy<j)quicksortSize(lewy, j, dane);
	if (prawy>i)quicksortSize(i, prawy, dane);
}

void quicksortValue(int lewy, int prawy, item *dane)
{
	int i = lewy, j = prawy;
	int sr = dane[(lewy + prawy) / 2].value;

	do
	{
		while (dane[i].value<sr)i++;
		while (dane[j].value>sr)j--;
		if (i <= j)
		{
			item pom = dane[i];
			dane[i] = dane[j];
			dane[j] = pom;
			i++;
			j--;
		}
	} while (i <= j);
	if (lewy<j)quicksortValue(lewy, j, dane);
	if (prawy>i)quicksortValue(i, prawy, dane);
}

void quicksortVs(int lewy, int prawy, item *dane)
{
	int i = lewy, j = prawy;
	int sr = dane[(lewy + prawy) / 2].vs;

	do
	{
		while (dane[i].vs<sr)i++;
		while (dane[j].vs>sr)j--;
		if (i <= j)
		{
			item pom = dane[i];
			dane[i] = dane[j];
			dane[j] = pom;
			i++;
			j--;
		}
	} while (i <= j);
	if (lewy<j)quicksortVs(lewy, j, dane);
	if (prawy>i)quicksortVs(i, prawy, dane);
}

int PD(int n, int b, item *items, int **solution)
{
	for (int x = 1; x <= b; x++) {
		for (int y = 1; y <= n; y++) {
			if (items[y - 1].size > x) 
			{
				solution[x][y] = solution[x][y - 1];
				continue; 
			}// przedmiot siê nie mieœci? przepisz wartoœæ z góry
			else
			{
				solution[x][y] = max(solution[x][y - 1], solution[x - items[y - 1].size][y - 1] + items[y - 1].value);
			}
		}
	}
	return solution[b][n];
}

void BF1(int n,int b, item *items)
{
	int ssum, wsum, wmax = 0, smax = 0;
	for (int x = 1; x<(1 << n); x++) { // dla wszystkich mo¿liwych uk³adów n zer i jedynek != od 0
		ssum = wsum = 0;
		for (int t = 0; t<n; t++) { // jeœli t-ety bit od koñca x == 1 to dodajemy t-et¹ paczkê do rozwi¹zania
			if ((x >> t) & 1) { ssum += items[t].size; wsum += items[t].value; }
		}
		if ((wsum > wmax) && (ssum <= b)) { wmax = wsum; smax = x; }
	}

}


int BF2(int n, int ssum, int wsum, int b, item *items) {

		if (n<-1) return wsum;

		if (ssum + items[n].size <= b) { 
			BF2(n - 1, ssum + items[n].value, wsum + items[n].value, b, items);
			if (wsum>maxi) { maxi = wsum; }
		}	 	
		BF2(n - 1, ssum, wsum, b, items); 										// nie wybieraj, przejdŸ do nastêpnego elementu;

}

int GH1(int n, int b, item *items)
{
	int v=0, s=0;
	for(int i=0;i<n;i++)
	{
		if (s + items[i].size <= b)
		{
			s += items[i].size;
			v += items[i].value;
		}
	}
	return v;
}

int GH2(int n, int b, item *items)
{
	quicksortSize(0, n - 1, items);
	int s = b, v = 0;
	int var = 0;
	for(int i=0;i<n;i++)
	{
		if (items[i].size > s)break;
		s -= items[i].size;
		v += items[i].value;
	}
	return v;
}

int GH3(int n, int b, item *items)
{
	quicksortValue(0, n - 1, items);
	int s = 0, v = 0;
	for(int i=n-1;i>=0;i--)
	{
		if (s + items[i].size < b)
		{
			s += items[i].size;
			v += items[i].value;
		}
	}
	return v;
}

int GH4(int n, int b, item *items)
{
	for (int i = 0; i < n; i++) items[i].vs = items[i].value / items[i].size;
	quicksortVs(0, n - 1, items);
	int s = 0, v = 0;
	for (int i = n - 1; i >= 0; i--)
	{
		if (s + items[i].size < b)
		{
			s += items[i].size;
			v += items[i].value;
		}
	}
	return v;
}

void zad2(int n)
{
	ofstream plik;
	plik.open("zad2-2.txt", ios::out | ios::app);
	item *items = new item[n];
	int k = 0;
	srand(time(NULL));
	for (int i = 0; i<n; i++)
	{
		items[i].size = rand() % 100 * n + n / 10;
		items[i].value = rand() % 1000 * n + n;
			k += items[i].size;
	}
	int b = 0.5*k;
	int ret;
	int **solution = new int*[b + 1];
	for (int j = 0; j <= b; j++)
		solution[j] = new int[n + 1];
	for (int j = 0; j <= b; j++)
		for (int k = 0; k <= n; k++)
			solution[j][k] = 0;
	auto t1 = std::chrono::high_resolution_clock::now();
	ret = PD(n, b, items, solution);
		auto t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
	plik << fp_ms.count() << "\t";
	t1 = std::chrono::high_resolution_clock::now();
	BF1(n, b, items);
	t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> fp_ms2 = t2 - t1;
	plik << fp_ms2.count() << "\t";
	t1 = std::chrono::high_resolution_clock::now();
	ret = BF2(n-1, 0, 0, b, items);
	t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> fp_ms3 = t2 - t1;
	plik << fp_ms3.count() << "\t";
	t1 = std::chrono::high_resolution_clock::now();
	ret = GH4(n, b, items);
	t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> fp_ms4 = t2 - t1;
	plik << fp_ms4.count() << "\n";
	plik.close();
}

void zad3(int n, double var)
{
	ofstream plik;
	plik.open("zad3-3.txt", ios::out | ios::app);
	item *items = new item[n];
	int k = 0;
	int ret;
	srand(time(NULL));
	for (int i = 0; i<n; i++)
	{
		items[i].size = rand() % 100 * n + n / 10;
		items[i].value = rand() % 1000 * n + n;
		k += items[i].size;
	}
	int b = var*k;
	int **solution = new int*[b + 1];
	for (int j = 0; j <= b; j++)
		solution[j] = new int[n + 1];
	for (int j = 0; j <= b; j++)
		for (int k = 0; k <= n; k++)
			solution[j][k] = 0;
	auto t1 = std::chrono::high_resolution_clock::now();
	ret = PD(n, b, items, solution);
	auto t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
	plik << fp_ms.count() << "\t";
	t1 = std::chrono::high_resolution_clock::now();
	BF1(n, b, items);
	t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> fp_ms2 = t2 - t1;
	plik << fp_ms2.count() << "\t";
	t1 = std::chrono::high_resolution_clock::now();
	ret = BF2(n-1, 0, 0, b, items);
	cout << ret << endl;
	t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> fp_ms3 = t2 - t1;
	plik << fp_ms3.count() << "\t";
	t1 = std::chrono::high_resolution_clock::now();
	ret = GH4(n, b, items);
	t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> fp_ms4 = t2 - t1;
	plik << fp_ms4.count() << "\n";
	plik.close();
}

void zad4()
{
	int n, b;
	ofstream plik;
	double s1=0, s2=0, s3=0, s4=0;
	double ret;
	double pom;
	int sum=0;
	double x = 0.25;
	plik.open("zad4-2.txt", ios::out|ios::app);
	while (x <= 0.75)
	{
		for (int i = 0; i < 10; i++)
		{
			n = tab[i];
			srand(time(NULL));
			item *items = new item[n];
			for (int j = 0; j < n; j++)
			{
				items[j].size = rand() % 10 * n + n / 10;
				items[j].value = rand() % 100 * n + n;
				sum += items[j].size;
			}
			b = sum*x;
			sum = 0;
			//pylylyllylylyllylylylyllylylyllly
			int **solution = new int*[b+1];
			for (int j = 0; j <= b; j++)
				solution[j] = new int[n+1];
			for (int j = 0; j <= b; j++)
				for (int k = 0; k <= n; k++)
					solution[j][k] = 0;


			ret = PD(n, b, items, solution);
			cout << ret << "\t";
			/*for (int j = 0; j <= b; j++)
			{
				for (int k = 0; k <= n; k++)
					cout << solution[j][k] << " ";
				cout << endl;
			}
			system("PAUSE");*/
			pom = GH1(n, b, items);
			cout << pom << "\t";
			s1 += (ret - pom) / ret * 100;
			pom = GH2(n, b, items);
			cout << pom << "\t";
			s2 += (ret - pom) / ret * 100;
			pom = GH3(n, b, items);
			cout << pom << "\t";
			s3 += (ret - pom) / ret * 100;
			pom = GH4(n, b, items);
			cout << pom << "\t";
			s4 += (ret - pom) / ret * 100;
			cout<<(ret - GH1(n, b, items)) / ret * 100 <<"\t";
			cout<<(ret - GH2(n, b, items)) / ret * 100 << "\t";
			cout<<(ret - GH3(n, b, items)) / ret * 100 << "\t";
			cout<<(ret - GH4(n, b, items)) / ret * 100 << "\n";
		}
		plik << s1 / 10 << "\t" << s2 / 10 << "\t" << s3 / 10 << "\t" << s4 / 10 << "\n";
		s1 = 0;
		s2 = 0;
		s3 = 0;
		s4 = 0;
		x += 0.25;
	}
	plik.close();

}

int main()
{
	/*for (int i = 0; i < 10; i++)
		zad2(tab[i]);*/
	for (int i = 0; i < 10; i++)
		zad3(tab[i],0.25);
	for (int i = 0; i < 10; i++)
		zad3(tab[i],0.75);
	//zad4();
	system("PAUSE");
	return 0;
}