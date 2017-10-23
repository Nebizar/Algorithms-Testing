#include<iostream>
#include<ctime>
#include<cstdlib>
#include<vector>
#include<fstream>
#include<list>
#include<algorithm>
using namespace std;

const int N = 100000;
int tab[] = { 10000,20000,30000,40000,50000,60000,70000,80000,90000,100000 };
int A[N];
int B[N];
int C[N];
int back[N];

// liczenie czasu od uruchomienia programu
double obliczSekundy(clock_t czas)
{
	return static_cast < double >(czas) / CLOCKS_PER_SEC;
}

//sortowanie-QuickSort-PIVOT
void quicksort(int lewy, int prawy, int *dane)
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

void createTab(int N, int tab[])
{
	srand(time(NULL));
	for (int i = 0; i < N; i++)
		tab[i] = (i + 1);
	for (int i = 0; i < N; i++)
	{
		int j = rand() % N;
		swap(tab[i], tab[j]);
	}
}

void copy(int tab[],int tab2[], int N)
{
	for (int i = 0; i < N; i++)
		tab2[i] = tab[i];
}

int LineSearch(int tab[], int num, int N)
{
	for (int i = 0; i < N; i++)
		if (tab[i] == num)
			return i;
	return -1;
}

int BinSearch(int tab[], int value, int left, int right)
{
	while (left <= right) {
		int middle = (left + right) / 2;
		if (tab[middle] == value)
			return middle;
		else if (tab[middle] > value)
			right = middle - 1;
		else
			left = middle + 1;
	}
	return -1;

}


//Lista - klasa 
class LinkedList {

	struct Node {
		int x;
		Node *next;
	};
	
public:
	LinkedList() {
		head = NULL;
	}

	void AddValue(int val)
	{
		Node *newNode;
		newNode = new Node;
		newNode->next = NULL;
		newNode->x = val;
		if (head != NULL) newNode->next = head;
		head = newNode;
	}

	int popValue() {
		Node *n = head;
		int ret = n->x;

		head = head->next;
		delete n;
		return ret;
	}
	int Search(int num) {
		int counter=0;
		Node *list = head;
		while (list) {
			if(list->x==num) return counter ;
			counter++;
			list = list->next;
		}
		return -1;
	}
	void Destroy() {
		Node *n = this->head, *current = NULL;
		while (n) {
			current = n;
			n = n->next;
			delete(current);
		}
	}
	

private:
	Node *head;
};

//BST - Algorytmmy
struct node
{
	int value;
	node *left, *right;
};

node *create_BST(int tab[], int n)
{
	node *root = NULL, *add, *current, *parent;

	for (int i = 0; i<n; i++)
	{
		add = new node();
		add->value = tab[i];
		add->left = add->right = NULL;
		if (root == NULL)
		{
			root = add;
		}
		else {
			current = parent = root;
			while (current != NULL)
			{
				parent = current;
				if ((add->value)<(current->value))
					current = current->left;
				else
					current = current->right;
			}
			if ((add->value)<(parent->value))
				parent->left = add;
			else
				parent->right = add;
		}
	}
	return root;
}

void find_BST(node *root, int searched)
{
	while ((root->value != searched))
	{
		if (searched<(root->value))
			root = root->left;
		else
			root = root->right;
	}

}

void bin_search(int tab1[], int tab2[], int q, int e, int i)
{
	int m = (q + e) / 2;
	tab2[i] = tab1[m];
	if (m>q)
		bin_search(tab1, tab2, q, m - 1, i + 1);
	if (m<e)
		bin_search(tab1, tab2, m + 1, e, i + m - q + 1);
}

int height(node *root)
{
	if (root == NULL)
		return 0;
	else
		return (1 + max(height(root->left), height(root->right)));
}



void test()
{
	double start, stop;
	int el = 0;
	int n=0;
	int licz;
	int h = 0;
	ofstream plik1;
	ofstream plik2;
	ofstream plik3;
	plik1.open("Creating2.txt", ios::out|ios::app);
	plik2.open("Searching2.txt", ios::out|ios::app);
	plik3.open("Height2.txt", ios::out | ios::app);
	plik1 << "Quant \t Tab \t List \t BST \t BST \n";
	plik2 << "Quant \t Tab \t Tab-bin \t List \t BST \t BST \n";
	plik3 << "Quant \t BST \t BST-row \n";
	do
	{
		createTab(N, A);
		n = tab[el];
		el++;
		plik1 << n << "\t";
		plik2 << n << "\t";
		plik3 << n << "\t";
		//Kopia A do B i posortowanie B
		start = obliczSekundy(clock());
		copy(A, B, n);
		quicksort(0, n - 1, B);
		stop = obliczSekundy(clock()) - start;
		plik1 << stop << "\t";
		//Wyszukiwanie liniowe tablicy
		start = obliczSekundy(clock());
		for (int j = 0; j < n; j++)
			licz = LineSearch(B, A[j], n);
		stop = obliczSekundy(clock()) - start;
		plik2 << stop << "\t";
		//Wyszukiwanie binarne w tablicy
		start = obliczSekundy(clock());
		for (int j = 0; j < n; j++)
			licz = BinSearch(B, A[j], 0, n);
		stop = obliczSekundy(clock()) - start;
		plik2 << stop << "\t";
		//kopiuj dane na liste
		start = obliczSekundy(clock());
		LinkedList L;
		for (int j = 0; j < n; j++)
			L.AddValue(A[j]);
		stop = obliczSekundy(clock()) - start;
		plik1 << stop << "\t";
		//wyszukiwanie liniowe na liscie
		start = obliczSekundy(clock());
		for (int j = 0; j < n; j++)
			licz = L.Search(A[j]);
		stop = obliczSekundy(clock()) - start;
		plik2 << stop << "\t";
		for (int i = 0; i < n; i++)
			back[i] = A[i];
		//tworzenie BST
		start = obliczSekundy(clock());
		node *root1 = create_BST(back, n);
		stop = obliczSekundy(clock())-start;
		plik1 << stop << "\t";
		//wysokosc drzewa
		h = height(root1);
		plik3 << h << "\t";
		//wyszukiwanie w BST
		start = obliczSekundy(clock());
		for (int i = 0; i<n; i++)
			find_BST(root1, A[i]);
		stop = obliczSekundy(clock()) - start;
		plik2 << stop << "\t";
		//kopia w kolejnosci dzielenia polowkowego
		bin_search(B, C, 0, n - 1, 0);
		//tworzenie BST zrownowazonego
		start = obliczSekundy(clock());
		node *root2 = create_BST(C, n);
		stop = obliczSekundy(clock()) - start;
		plik1 << stop << "\n";
		//wysokosc BST zrownowazonego
		h = height(root2);
		plik3 << h << "\n";
		//wyszukiwanie w BST zrownowazonym
		start = obliczSekundy(clock());
		for (int i = 0; i<n; i++)
			find_BST(root2, A[i]);
		stop = obliczSekundy(clock()) - start;
		plik2 << stop << "\n";
		system("PAUSE");
	} while (n != N);
	plik1.close();
	plik2.close();
	plik3.close();
}

int main()
{
	test();
	system("PAUSE");
	return 0;
}


