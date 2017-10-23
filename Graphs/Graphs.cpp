#include <iostream>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include<iomanip>
#include <fstream>

#define d 2/10  	// gestosc

int tab[] = { 600,1200,1800,2400,3000,3600,4200,4800,5400,6000 };

using namespace std;

double obliczSekundy(clock_t czas)
{
	return static_cast < double >(czas) / CLOCKS_PER_SEC;
}

////// ELEMENTY LISTY //////

struct lista {
	int w;
	lista* nast;
};

int nr = 0;

void dfs(int* begin, int* end, bool* visited, int n, int v, lista **SL) {

	visited[v] = 1;
	begin[v] = ++nr; //poczatek analizy wierzcholka

	lista *nastepnik = SL[v];
	//	cout << "odwiedzono " << v << endl;

	while (nastepnik != NULL) {
		if (visited[nastepnik->w] == 0) dfs(begin, end, visited, n, nastepnik->w, SL);
		nastepnik = nastepnik->nast;
	}

	end[v] = ++nr; //koniec analizy wierzcholka

}

void work(int n)
{
	nr = 0;
	ofstream plik1, plik2, plik3;
	plik1.open("Zad2-1.txt", ios::out | ios::app);
	plik2.open("Zad3-1.txt", ios::out | ios::app);
	plik3.open("Zad4-1.txt", ios::out | ios::app);
	plik1 << n << "\t";
	plik2 << n << "\t";
	plik3 << n << "\t";
	double start, stop;
	srand(time(NULL));
	//Tworzenie grafu
	int m = n*n*d;
	int a, b;
	int **NM = new int*[n];

	for ( int x = 0; x < n; x++)
		NM[x] = new int[n];	// tablica jest inicjalizowana zerami		
	for (int x = 0; x < n; x++)
		for (int y = 0; y < n; y++)
			NM[x][y] = 0;

	int i = m;
	while (i) {
		a = rand() % n; //losowy wierzcholek (od 0 do n-1)
		b = rand() % n;

		if (NM[a][b] != 1 && a != b) {
			NM[a][b] = 1;
			i--;
		}
	}

	/*for (int x = 0; x < n; x++)
	{
		for (int y = 0; y < n; y++)
			cout << NM[x][y] << " ";
		cout << endl;
	}
	cout << endl << endl << endl;*/
	//graf - lista ³uków - tworzenie na podstawie macierzy
	int *AL[2];
	AL[0] = new int[m];
	AL[1] = new int[m];

	for (int i = 0, x = 0; x<n; x++) {
		for (int y = 0; y<n; y++) {
			if (NM[x][y]) 
			{
				AL[0][i] = x;
				AL[1][i] = y;
				i++;
			}
		}
	}

	/*for (int i = 0; i < m; i++)
		cout << AL[0][i] << " " << AL[1][i] << endl;

	cout << endl << endl << endl;*/

	//graf - lista nastêpników - na podstawie Macierzy

	lista **SL;
	SL=new lista*[n + 1];
	lista *ostatni, *nowy, *p;
	for (int i = 0; i < n + 1; i++)
		SL[i] = NULL;
	for (int x = 0; x<n; x++) {
		for (int y = 0; y<n; y++) {
			if (NM[x][y]) {
				p = new lista;    // Tworzymy nowy element
				p->w = y;          // Numerujemy go jako v2
				p->nast = SL[x];    // Dodajemy go na pocz¹tek listy A[v1]
				SL[x] = p;
			}
		}
	}
	/*for (i = 0; i < n; i++)
	{
		cout << "SL[" << i << "] =";
		p = SL[i];
		while (p)
		{
			cout << setw(3) << p->w;
			p = p->nast;
		}
		cout << endl;
	}

	cout << endl << endl;*/

	// sortowanie topologiczne	

	// obliczanie etykiet czasowych

	bool *visited = new bool[n]; //odwiedzony
	int *begin = new int[n]; // rozpoczêto analizê
	int *end = new int[n]; // zakonczono analizê
	for (int i = 0; i < n; i++)
	{
		begin[i] = 0;
		end[i] = 0;
		visited[i] = 0;
	}
	start=obliczSekundy(clock());

	for (int x = 0; x<n; x++)
		if (visited[x] == 0)
			//			cout << "WZNOWIENIE od " << x << endl;
			dfs(begin, end, visited, n, x, SL);

	//	for (x=0; x<n; x++) { cout << "begin[" << x << "]=" << begin[x] << " end[" << x << "]=" << end[x] << endl;}

	printf("zliczanie etykiet: ");
	stop = obliczSekundy(clock()) - start;
	cout << stop <<endl;
	plik1 << stop << "\n";
	/*for (int i = 0; i < n; i++)
		cout << i << " " << begin[i] << " " << end[i] << endl;*/
	// zliczanie ³uków powrotnych - lista ³uków

	int luki_powrotne = 0;

	start = obliczSekundy(clock());
	for (int x = 0; x<m; x++) //dla kazdego ³uku
		if (begin[AL[1][x]] < begin[AL[0][x]] && end[AL[0][x]] < end[AL[1][x]])
			luki_powrotne++;
	//			printf("³uk powrotny: %d, %d\n", AL[0][x], AL[1][x]);		
	stop = obliczSekundy(clock()) - start;
	plik3 << stop << "\t";
	// wypisywanie wynikow

	cout << "lukow powrotnych: " << luki_powrotne << " / " << m << endl;
	plik2 << luki_powrotne << "\n";

	// zliczanie ³uków powrotnych - macierz s¹siedztwa

	luki_powrotne = 0;

	start = obliczSekundy(clock());
	for (int x = 0; x < n; x++) //dla kazdego ³uku
	{
		for (int y = 0; y < n; y++)
		{
			if (NM[x][y] == 1)
			{
				if (begin[y] < begin[x] && end[x] < end[y])
					luki_powrotne++;
			}
		}
	}
	stop = obliczSekundy(clock()) - start;
	plik3 << stop << "\t";
	// wypisywanie wynikow

	cout << "lukow powrotnych: " << luki_powrotne << " / " << m << endl;
	plik2 << luki_powrotne << "\n";

	// zliczanie ³uków powrotnych - lista nastêpników
	
	luki_powrotne = 0;
	lista *nastepnik;
	int poprzednik;
	start = obliczSekundy(clock());
	for (int x = 0; x < n; x++) //dla kazdego ³uku
	{
		if (SL[x] != NULL)
		{
			poprzednik = SL[x]->w;
			nastepnik = SL[x]->nast;
			while (nastepnik != NULL)
			{
				if (begin[poprzednik] < begin[x] && end[x] < end[poprzednik])
					luki_powrotne++;
				poprzednik = nastepnik->w;
				nastepnik = nastepnik->nast;
			}
			if (begin[poprzednik] < begin[x] && end[x] < end[poprzednik])
				luki_powrotne++;
		}
	}
	//				printf("³uk powrotny: %d, %d\n", AL[0][x], AL[1][x]);		
	stop = obliczSekundy(clock()) - start;
	plik3 << stop << "\n";
	// wypisywanie wynikow

	cout << "lukow powrotnych: " << luki_powrotne << " / " << m << endl;
	plik2 << luki_powrotne << "\n";
	
	plik1.close();
	plik2.close();
	plik3.close();
}

int main()
{
	for (int i = 0; i < 10; i++)
	{
		work(tab[i]);
	}
	system("PAUSE");
	return 0;
}