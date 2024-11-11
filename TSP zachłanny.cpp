#include <iostream>
#include<stddef.h>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;

const int N=100;

void greedy(int n,int ** macierz) //algorytm zach³anny dla TSP
{
	cout<<endl;
	int min[2];
	min[0]=0;
	int i=0;
	while(min[0]!=N)
	{
		cout<<i<<" ";
		min[0]=N;
		for(int j=0;j<n;j++)
		{
			if(macierz[i][j]<min[0] && macierz[i][j]!=0)
			{
				min[0]=macierz[i][j];
				min[1]=j;
			}
		}
		macierz[i][min[1]]=0;
		for(int k=0;k<n;k++) macierz[k][i]=0;
		i=min[1];
		if(min[0]==N) {i=0; cout<<i<<endl;} //ostatni przypadek powrót do pocz¹tku
		//for(int i=0;i<n;i++) {for(int j=0;j<n;j++) cout<<macierz[i][j]<<" "; cout<<endl;} cout<<endl; // wyœwietlanie zmian w tablicy
	}
}
int main()
{
	int n;
	cout<<"Podaj liczbe wierzcholkow: ";
	cin>>n;
	int ** macierz;
	macierz = new int * [n];
	for (int i = 0; i < n; i++) macierz[i] = new int[n];
	for(int i=0;i<n;i++) for(int j=0;j<n;j++) macierz[i][j]=0;
	for(int i=0;i<n;i++) for(int j=0;j<i;j++) macierz[i][j]=rand()%9+1;
	for(int i=0;i<n;i++) for(int j=0;j<i;j++) macierz[j][i]=macierz[i][j];
	for(int i=0;i<n;i++) {for(int j=0;j<n;j++) cout<<macierz[i][j]<<" "; cout<<endl;}
	srand(time(NULL));
	greedy(n,macierz);
	return 0;
}
