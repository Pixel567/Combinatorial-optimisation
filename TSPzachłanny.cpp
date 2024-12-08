#include <iostream>
#include<fstream>
#include<stddef.h>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <cmath>
#include <vector>
#include <climits>

using namespace std;

const int N=100000;
struct points
{
	int X;
	int Y;
};

void greedy(int n,float ** macierz) //algorytm zach�anny dla TSP
{
	float length=0.0;
	cout<<endl;
	int min;
	float min0=0;
	int i=1;
	for(int j=1;j<=n;j++){
		macierz[0][j]=macierz[1][j];
	}
	while(min0!=N)
	{
		cout<<i<<" ";
		min0=N;
		for(int j=1;j<=n;j++)
		{
			if(macierz[i][j]<min0 && macierz[i][j]!=0)
			{
				min0=macierz[i][j];
				min=j;
			}
		}
		macierz[i][min]=0;
		for(int k=1;k<=n;k++){
			macierz[k][i]=0;
		}
		if(min0==N) {
			length+=macierz[0][i]; i=1;
			cout<<i<<endl;
			} //ostatni przypadek powr�t do pocz�tku
		else{
		length+=min0;
		i=min;
		}
		//for(int i=0;i<n;i++) {for(int j=0;j<n;j++) cout<<macierz[i][j]<<" "; cout<<endl;} cout<<endl; // wy�wietlanie zmian w tablicy
	}
	cout<<endl<<"Length: "<<length<<endl;
}
void matrixfill(int n,float** macierz,points* list) //calulates distance from coordinates
{
	for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (i != j) {
                macierz[i][j] = sqrt(pow(list[i].X - list[j].X, 2) + pow(list[i].Y - list[j].Y, 2));
            } else {
                macierz[i][j] = 0;  // Dystans do samego siebie
            }
        }
    }
}
int main()
{
	int n;
	srand(time(NULL));//przygotowanie generatora liczb pseudolosowych
	ifstream plik;
	plik.open("dane.txt");

	plik>>n;
	points * list = new points[n+1];

	int cityIndex;
	for(int i=1;i<=n;i++)
	{
		plik>>cityIndex>>list[i].X>>list[i].Y;
	}
	plik.close();

	float** macierz;
	macierz = new float* [n+1];
	for (int i = 0; i <= n; i++) {
	macierz[i] = new float[n+1];
	}
	for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            macierz[i][j] = 0;
        }
    }
	//for(int i=0;i<n;i++) for(int j=0;j<i;j++) macierz[i][j]=rand()%9+1;
	
	matrixfill(n,macierz,list);
	for(int i=0;i<=n;i++) for(int j=0;j<=i;j++) macierz[j][i]=macierz[i][j];
	for(int i=0;i<=n;i++) {for(int j=0;j<=n;j++) cout<<macierz[i][j]<<" "; cout<<endl;}
	greedy(n,macierz);
	for (int i = 0; i <= n; i++) {
        delete[] macierz[i];
    }
    delete[] macierz;
	delete[] list;
	
	return 0;
}
