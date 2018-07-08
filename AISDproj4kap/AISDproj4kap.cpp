// AISDproj4kap.cpp : Defines the entry point for the console application.
//
#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#define INFINITY 2000000000
#define MAKSYMALNA_WIELKOSC_KOPCA 100000
int numerUsunietej = -1;
int najkrotsza = INFINITY, drugaNajkrotsza = INFINITY;
using namespace std;

struct sciezka
{
	int dokad;
	int dlugosc;
	int numer;
};

struct node
{
	sciezka * sciezki;
	int iloscPolaczen;
	int najkrotszaOdleglosc;
	int odleglosc_tmp;
	int numerPoprzedniejSciezki;
	int poprzedniaSciezka;

	node(int ilePolaczen)
	{
		iloscPolaczen = 0;
		sciezki = new sciezka[ilePolaczen];
		najkrotszaOdleglosc = INFINITY;
		odleglosc_tmp = INFINITY;
		numerPoprzedniejSciezki = -1;
		poprzedniaSciezka = -1;
	}
};

void swap(int*tab, int a, int b)
{
	int tmp = tab[a];
	tab[a] = tab[b];
	tab[b] = tmp;
}

void heapify(node ** nodes, int*kopiec, int number, int rozmiar)
{
	int left = number * 2, right = number * 2 + 1, min = number;
	if (left <= rozmiar && nodes[kopiec[left]]->odleglosc_tmp<nodes[kopiec[min]]->odleglosc_tmp)
		min = left;
	if (right <= rozmiar && nodes[kopiec[right]]->odleglosc_tmp<nodes[kopiec[min]]->odleglosc_tmp)
		min = right;
	if (min != number)
	{
		swap(kopiec, min, number);
		heapify(nodes, kopiec, min, rozmiar);
	}
}

void addElement(node**nodes, int * kopiec, int value, int &rozmiar)
{
	rozmiar++;
	kopiec[rozmiar] = value;
	int i = rozmiar;
	while (i>1 && nodes[kopiec[i]]->odleglosc_tmp < nodes[kopiec[i / 2]]->odleglosc_tmp)
		swap(kopiec, i, i / 2);
}

int getElement(node ** nodes, int * kopiec, int& rozmiar)
{
	int wartosc = kopiec[1];
	kopiec[1] = kopiec[rozmiar];
	rozmiar--;
	heapify(nodes, kopiec, 1, rozmiar);
	return wartosc;
}

void DijkstraFirst(node ** nodes, int * kopiec, int skrzyzowan)
{
	nodes[0]->odleglosc_tmp = 0;
	int tmp_node, tmp_numer, tmp_dlugosc, tmp_dokad;
	int rozmiarKopca = 0;
	addElement(nodes, kopiec, 0, rozmiarKopca);
	do
	{
		tmp_node = getElement(nodes, kopiec, rozmiarKopca);
		nodes[tmp_node]->najkrotszaOdleglosc = nodes[tmp_node]->odleglosc_tmp;
		
		for (int i = 0; i<nodes[tmp_node]->iloscPolaczen; i++)
		{
			tmp_dokad = nodes[tmp_node]->sciezki[i].dokad;
			tmp_numer = nodes[tmp_node]->sciezki[i].numer;
			tmp_dlugosc = nodes[tmp_node]->sciezki[i].dlugosc + nodes[tmp_node]->najkrotszaOdleglosc;
			if (nodes[tmp_dokad]->odleglosc_tmp <= tmp_dlugosc)
				continue;
			nodes[tmp_dokad]->odleglosc_tmp = tmp_dlugosc;
			nodes[tmp_dokad]->poprzedniaSciezka = tmp_node;
			nodes[tmp_dokad]->numerPoprzedniejSciezki = tmp_numer;
			addElement(nodes, kopiec, tmp_dokad, rozmiarKopca);
		}
	} while (rozmiarKopca>0);

	najkrotsza = nodes[skrzyzowan]->najkrotszaOdleglosc;
}

int DijkstraSecond(node ** nodes, int * kopiec, int skrzyzowan, int fromWhere)
{
	int tmp_node, tmp_dlugosc, tmp_dokad;
	int rozmiarKopca = 0;
	addElement(nodes, kopiec, fromWhere, rozmiarKopca);
	nodes[fromWhere]->odleglosc_tmp = nodes[fromWhere]->najkrotszaOdleglosc;
	do
	{
		tmp_node = getElement(nodes, kopiec, rozmiarKopca);
		if (tmp_node == skrzyzowan)
			break;
		for (int i = 0; i<nodes[tmp_node]->iloscPolaczen; i++)
		{
			tmp_dokad = nodes[tmp_node]->sciezki[i].dokad;
			tmp_dlugosc = nodes[tmp_node]->sciezki[i].dlugosc + nodes[tmp_node]->odleglosc_tmp;
			if (nodes[tmp_dokad]->odleglosc_tmp <= tmp_dlugosc)
				continue;
			if (nodes[tmp_node]->sciezki[i].numer == nodes[tmp_node]->numerPoprzedniejSciezki)
				continue;
			if (drugaNajkrotsza <= tmp_dlugosc || tmp_dlugosc >= nodes[skrzyzowan]->odleglosc_tmp || nodes[tmp_node]->sciezki[i].numer == numerUsunietej)
				continue;
			nodes[tmp_dokad]->odleglosc_tmp = tmp_dlugosc;
			addElement(nodes, kopiec, tmp_dokad, rozmiarKopca);
		}
	} while (rozmiarKopca>0);
	return nodes[skrzyzowan]->odleglosc_tmp;
}

void findSecond(node ** nodes, int * kopiec, int skrzyzowan)
{
	int poczatek = skrzyzowan;
	int koniec = nodes[poczatek]->poprzedniaSciezka;
	int dlugosc_tmp = INFINITY;
	while (koniec != -1)
	{
		for (int i = 0; i<skrzyzowan + 1; i++)	
		{
			nodes[i]->odleglosc_tmp = INFINITY;
		}
		numerUsunietej	 = nodes[poczatek]->numerPoprzedniejSciezki;
		dlugosc_tmp = DijkstraSecond(nodes, kopiec, skrzyzowan, koniec);
		poczatek = koniec;
		koniec = nodes[poczatek]->poprzedniaSciezka;
		if (dlugosc_tmp<drugaNajkrotsza)
			drugaNajkrotsza = dlugosc_tmp;
	}
}

int main()
{
	int * kopiec = new int[MAKSYMALNA_WIELKOSC_KOPCA];
	for (int i = 0; i < MAKSYMALNA_WIELKOSC_KOPCA; i++)
		kopiec[i] = -1;
	int iloscSkrzyzowan = 0, iloscSciezek = 0;
	scanf("%d %d", &iloscSkrzyzowan, &iloscSciezek);
	node ** nodes = new node*[iloscSkrzyzowan];
	int * wejscie = new int[iloscSciezek * 3];
	int* ilePolaczen = new int[iloscSkrzyzowan];	
	for (int i = 0; i<iloscSkrzyzowan; i++)
		ilePolaczen[i] = 0;
	for (int i = 0; i<iloscSciezek; i++)
	{
		scanf("%d %d %d", &wejscie[i * 3], &wejscie[i * 3 + 1], &wejscie[i * 3 + 2]);
		ilePolaczen[wejscie[i * 3]]++;
		ilePolaczen[wejscie[i * 3 + 1]]++;
	}
	for (int i = 0; i<iloscSkrzyzowan; i++)
	{
		nodes[i] = new node(ilePolaczen[i]);
		nodes[i]->iloscPolaczen = ilePolaczen[i];
	}
	int first, second, value;
	int * ileWczytanych = new int[iloscSkrzyzowan];
	for (int i = 0; i<iloscSkrzyzowan; i++)
		ileWczytanych[i] = 0;
	for (int i = 0; i<iloscSciezek; i++)
	{
		first = wejscie[i * 3];
		second = wejscie[i * 3 + 1];
		value = wejscie[i * 3 + 2];
		nodes[first]->sciezki[ileWczytanych[first]].numer = i;
		nodes[second]->sciezki[ileWczytanych[second]].numer = i;
		nodes[first]->sciezki[ileWczytanych[first]].dokad = second;
		nodes[second]->sciezki[ileWczytanych[second]].dokad = first;
		nodes[first]->sciezki[ileWczytanych[first]].dlugosc = value;
		nodes[second]->sciezki[ileWczytanych[second]].dlugosc = value;
		ileWczytanych[first]++;
		ileWczytanych[second]++;
	}

	DijkstraFirst(nodes, kopiec, iloscSkrzyzowan - 1);
	findSecond(nodes, kopiec, iloscSkrzyzowan - 1);

	if (drugaNajkrotsza != INFINITY)
		printf("%d %d", najkrotsza, drugaNajkrotsza);
	else
		printf("#");



	delete[] wejscie;
	delete[] ilePolaczen;
	delete[] kopiec;
	for (int i = 0; i < iloscSkrzyzowan; i++)
	{
		delete[] nodes[i];
	}
	delete[] nodes;

	return 0;
}
