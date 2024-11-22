#include <iostream>
#include<fstream>
#include<stddef.h>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <cmath>

using namespace std;

struct points
{
	int X;
	int Y;
};
void macierzfill(int n,int ** macierz,points * list)
{
	for(int i=1;i<=n;i++)
		for(int j=1;j<i;j++)
		{
			if(i!=j)
			{
				macierz[i][j]=sqrt(pow(list[i].X-list[j].X,2)+pow(list[i].Y-list[j].Y,2));
			}
		}
}
void ant(int n,int ** macierz)
{
	
}
int main()
{
	srand(time(NULL));//przygotowanie generatora liczb pseudolosowych
	int n,x;
	ifstream plik;
	plik.open("dane.txt");
	plik>>n;
	points * list = new points[n+1];
	for(int i=1;i<=n;i++)
	{
		plik>>i>>list[i].X>>list[i].Y;
	}
	plik.close();
	//for(int i=1;i<=n;i++) cout<<i<<" "<<list[i].X<<" "<<list[i].Y<<endl;//wypisuje wczytane dane 
	int ** macierz;
	macierz = new int * [n+1];
	for (int i = 1; i <= n; i++) macierz[i] = new int[n+1];
	for(int i=0;i<=n;i++) for(int j=0;j<=n;j++) macierz[i][j]=0;
	macierzfill(n,macierz,list);
	for(int i=1;i<=n;i++) for(int j=1;j<=i;j++) macierz[j][i]=macierz[i][j];
	for(int i=1;i<=n;i++) {for(int j=1;j<=n;j++) cout<<macierz[i][j]<<" "; cout<<endl;}//wypisuje macierz
	return 0;
}
