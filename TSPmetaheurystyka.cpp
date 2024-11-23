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
struct path
{
	float feromony;
	int distance;
};
void macierzfill(int n,path ** macierz,points * list)
{
	for(int i=1;i<=n;i++)
		for(int j=1;j<i;j++)
		{
			if(i!=j)
			{
				macierz[i][j].distance=sqrt(pow(list[i].X-list[j].X,2)+pow(list[i].Y-list[j].Y,2));
				
			}
		}
}
void ant(int n,path ** macierz)
{
	
}

void update_pheromones(int A, int n, path** pheromones, int** paths, path** macierz, float Q, float rho) {
    for (int i = 1; i <= A; i++) {
        for (int j = 1; j <= A; j++) {
            pheromones[i][j].feromony *= (1 - rho);  // Apply evaporation
        }
    }

    for (int ant = 0; ant < n; ant++) {
        float path_length = 0.0;
        for (int i = 0; i < A - 1; i++) {
            int city1 = paths[i][ant];
            int city2 = paths[i + 1][ant];
            path_length += macierz[city1][city2].distance;
        }
	// Calculate distances
        path_length += macierz[paths[A - 1][ant]][paths[0][ant]].distance;

        for (int i = 0; i < A - 1; i++) {
            int city1 = paths[i][ant];
            int city2 = paths[i + 1][ant];
            float pheromone_deposit = Q / path_length;

            pheromones[city1][city2].feromony += pheromone_deposit;
            pheromones[city2][city1].feromony += pheromone_deposit;
        }
        int last_city = paths[A - 1][ant];
        int first_city = paths[0][ant];
        float pheromone_deposit = Q / path_length;

        pheromones[last_city][first_city].feromony += pheromone_deposit;
        pheromones[first_city][last_city].feromony += pheromone_deposit;
    }
}
int main()
{
	srand(time(NULL));//przygotowanie generatora liczb pseudolosowych
	int n,x;
	ifstream plik;
	const int num_ants = 10;  // Number of ants
	const float rho = 0.5;    // Evaporation rate
	const float Q = 100.0;    // Pheromone deposition constant
	const int iterations = 100;
	
	plik.open("dane.txt");
	plik>>n;
	points * list = new points[n+1];
	for(int i=1;i<=n;i++)
	{
		plik>>i>>list[i].X>>list[i].Y;
	}
	plik.close();
	//for(int i=1;i<=n;i++) cout<<i<<" "<<list[i].X<<" "<<list[i].Y<<endl;//wypisuje wczytane dane 
	path ** macierz;
	macierz = new path * [n+1];
	for (int i = 1; i <= n; i++) macierz[i] = new path[n+1];
	for (int i = 0; i <= n; i++) 
	    for (int j = 0; j <= n; j++) 
		macierz[i][j].distance = 0;  
		macierz[i][j].feromony = 1.0;
	macierzfill(n,macierz,list);
	for(int i=1;i<=n;i++) for(int j=1;j<=i;j++) macierz[j][i].distance=macierz[i][j].distance;
	for(int i=1;i<=n;i++) {for(int j=1;j<=n;j++) cout<<macierz[i][j].distance<<" "; cout<<endl;}//wypisuje macierz odleg³oœci
	return 0;
}
