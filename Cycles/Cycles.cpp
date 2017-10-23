#include <iostream>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include<iomanip>
#include <fstream>
#include<chrono>
#include <ratio>

#define d 6/10  	// gestosc

int tab[] = { 5,6,7,8,9,10,11,12,13,14,15,25,35,45,55,65,75,85,95,105,115 };

using namespace std;

double obliczSekundy(clock_t czas)
{
	return static_cast < double >(czas) / CLOCKS_PER_SEC;
}

//DFS
void dfs(int n, int v, bool* visited, int **NM) { //analizuj v

	visited[v] = 1;
	//	cout << v << "-> ";
	for (int x = 0; x<n; x++) 		// dla wszystkich wierzchołkow x
		if (NM[v][x] && !visited[x]) 	// jesli istnieje krawedz v-x i x nie byl odwiedzony
			dfs(n, x, visited, NM); // analizuj x

}

//Wyszukiwanie cyklu Eulera
void Euler(int n, int** NM_kopia, int v)
{
	for (int i = 0; i<n; i++)
	{
		while (NM_kopia[v][i])
		{
			NM_kopia[v][i] = 0;
			NM_kopia[i][v] = 0;
			Euler(n, NM_kopia, i);
		}
	}
	//cout << v << ", ";
}

//Generowanie spójnego grafu Eulerowskiego
void generuj(int n, int m, int** NM)
{
	int i = m, a, b;
	srand(time(NULL));
	while (i) {
		a = rand() % n; //losowy wierzcholek (od 0 do n-1)
		b = rand() % n;

		if (NM[a][b] != 1 && a != b) {
			NM[a][b] = 1;
			NM[b][a] = 1;
			i--;
		}
	}
	int q = m;
	for (int i = 0; i<n; i++)
	{
		int deg = 0;
		for (int j = 0; j<n; j++)
		{
			if (NM[i][j])
				deg++;
		}
		if (deg % 2 != 0)
		{
			int j = rand() % (n - i - 1) + i + 1;
			if (NM[i][j])
			{
				NM[i][j] = 0;
				NM[j][i] = 0;
				q--;
			}
			else
			{
				NM[i][j] = 1;
				NM[j][i] = 1;
				q++;
			}


		}

	}
	//Sprawdzanie czy graf jest spójny - zabezpieczenie - powtórne uruchomienie jeśli się nie udało
	bool *visited = new bool[n];
	bool all_visited = 1;
	dfs(n, 0, visited, NM);

	for (int x = 0; x<n - 1; x++) if (!visited[x]) { all_visited = 0; break; }

	if (all_visited) printf("graf spojny\n");
	else {
		printf("graf niespojny\nponowne generowanie: ");
		generuj(n, m, NM);
	}

}

//Kopiowanie macierzy
void kopiuj(int n, int **NM, int **NM_copy)
{

	for (int x = 0; x<n; x++)
		for (int y = 0; y<n; y++)
			NM_copy[x][y] = NM[x][y];

}

//Wyszukiwanie wszystkich cykli Hamiltona
int k = 0, found = 0;
int HA = 0;
void Hamilton(int n, int v, int** NM, int* path, bool* visited)
{
	path[k++] = v;
	if (k != n)
	{
		visited[v] = 1;
		for (int i = 0; i<n; i++)
		{
			if (NM[v][i] == 1 && !visited[i])
				Hamilton(n, i, NM, path, visited);
		}
		visited[v] = 0;
	}
	else if (NM[v][0])
	{
		HA++;
	}
	k--;
}

//Wyszukiwanie jednego cyklu Hamiltona
void Hamilton1(int n, int v, int** NM, int *path, bool *visited)
{
	if (found == 1) return;
	path[k++] = v;
	if (k != n)
	{
		visited[v] = 1;
		for (int i = 0; i<n; i++)
		{
			if (NM[v][i] == 1 && !visited[i])
				Hamilton1(n, i, NM, path, visited);
		}
		visited[v] = 0;
	}
	else if (NM[v][0])
	{
		if (!found)
		{
			found = 1;
			return;
		}

	}
	k--;
}



void work(int n)
{

	ofstream plik1, plik2, plik3, plik4;
	plik1.open("Zad2-1.txt", ios::out | ios::app);
	plik2.open("Zad3-1.txt", ios::out | ios::app);
	plik3.open("Zad4-1.txt", ios::out | ios::app);
	plik4.open("Zad5-1.txt", ios::out | ios::app);
	plik1 << n << "\t";
	plik2 << n << "\t";
	plik3 << n << "\t";
	plik4 << n << "\t";
	double start, stop;
	HA = 0;
	found = 0;
	k = 0;
	srand(time(NULL));
	//Tworzenie grafu
	int m = (n*(n - 1)*d) / 2;
	int a, b;
	int **NM = new int*[n];
	for (int x = 0; x < n; x++)
		NM[x] = new int[n];	// tablica jest inicjalizowana
	for (int x = 0; x < n; x++)
		for (int y = 0; y < n; y++)
			NM[x][y] = 0;//zerowanie
	//Tworzenie macierzy grafu
	generuj(n, m, NM);
	//Tworzenie kopii - do Eulera
	int **NM_kopia = new int*[n];
	for (int x = 0; x < n; x++)
		NM_kopia[x] = new int[n];
	kopiuj(n, NM, NM_kopia);

	//Tablice pomocnicze + zerowanie ich wartości
	int *path = new int[n];
	for (int i = 0; i < n; i++) path[i] = 0;
	bool *visited = new bool[n];
	for (int i = 0; i < n; i++) visited[i] = 0;

	//Wyszukiwanie cyklu Eulera
	//start = obliczSekundy(clock());
	auto t1 = std::chrono::high_resolution_clock::now();
	Euler(n, NM_kopia, 0);
	auto t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
	//stop = obliczSekundy(clock()) - start;
	cout << fp_ms.count() << endl;
	plik1 << fp_ms.count() << "\n";

	//Wyszukiwanie 1 cyklu Hamiltona
	auto t3 = std::chrono::high_resolution_clock::now();
	//start = obliczSekundy(clock());
	Hamilton1(n, 0, NM, path, visited);
	auto t4 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> fp_ms2 = t4 - t3;
	//stop = obliczSekundy(clock()) - start;
	cout << fp_ms2.count() << endl;
	plik2 << fp_ms2.count() << "\n";
	
	//zerowanie tablic pomocniczych
	for (int i = 0; i < n; i++) path[i] = 0;
	for (int i = 0; i < n; i++) visited[i] = 0;
	
	//Wyszukiwanie wszystkich cykli Hamiltona do maks 15 wierzchołków 
	if (n <= 15)
	{
		auto t1 = std::chrono::high_resolution_clock::now();
		//start = obliczSekundy(clock());
		Hamilton(n, 0, NM, path, visited);
		auto t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
		//stop = obliczSekundy(clock()) - start;
		cout << fp_ms.count() << endl;
		plik3 << fp_ms.count() << "\n";
		plik4 << HA / 2 << "\n";
	}

	plik1.close();
	plik2.close();
	plik3.close();
	plik4.close();

}

int main()
{
	for (int i = 0; i < 21; i++)
	{
		work(tab[i]);
	}
	system("PAUSE");
	return 0;
}