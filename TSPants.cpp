#include <iostream>
#include<fstream>
#include<stddef.h>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <cmath>
#include <vector>
using namespace std;

const int A=1; //number of ants in generation
const int fer=1; //power of pheromons in choosing next city
const int dis=1; //power of distance in choosing next city
const int num_ants = 10;  // Number of ants
const float rho = 0.5;    // Evaporation rate
const float Q = 100.0;    // Pheromone deposition constant
const int iterations = 100;

struct points
{
	int X;
	int Y;
};
struct path
{
	float pheromons;
	int distance;
};
void matrixfill(int n,path ** matrix,points * list) //calulates distance from coordinates
{
	for(int i=1;i<=n;i++)
		for(int j=1;j<i;j++)
		{
			if(i!=j)
			{
				matrix[i][j].distance=sqrt(pow(list[i].X-list[j].X,2)+pow(list[i].Y-list[j].Y,2));
			}
		}
}
void ant(int n,int m,path ** matrix,int ** paths)//number of cities, number of ants, matrix of distances and pheromons, list of paths to be generated
{
	int ** copy;
	copy = new int * [n+1];
	for (int i = 0; i <= n; i++) copy[i] = new int[n+1];
	for(int i=0;i<=n;i++) for(int j=0;j<=n;j++) copy[i][j]=matrix[i][j].distance; //craeting copy of distance matrix
	int node=rand()%n+1; //choosing random first city
	//cout<<node<<endl;
	paths[m][n]=node;
	for(int k=1;k<=n;k++) copy[k][node]=0; //removing paths to first city
	int lenght=0;
	int choice=0;
	float probability[n+1];
	vector<int> c;
	while(lenght<n-1)
	{
		for(int i=0;i<=n;i++) probability[i]=0;
		paths[m][lenght]=node; //adding node to solution
		for(int j=1;j<=n;j++)
		{
			if(copy[node][j]!=0)
			{
				probability[j]=pow(matrix[node][j].pheromons,fer)*pow(matrix[node][j].distance,-dis);//calculating probability of next nodes
				//cout<<probability[j]<<" ";
			}
		}
		for(int j=1;j<=n;j++)
		{
			for(int k=0;k<probability[j];k++) c.push_back(j);
		}
		//for(int j=0;j<c.size();j++) cout<<c[j]<<" ";
		choice=c[rand()%c.size()];//choosing next node
		c.clear();
		copy[node][choice]=0;
		for(int k=1;k<=n;k++) copy[k][choice]=0; //removing paths to node
		node=choice;
		lenght++;
		//cout<<choice<<endl;
		//for(int i=1;i<=n;i++) {for(int j=1;j<=n;j++) cout<<copy[i][j]<<" "; cout<<endl;} cout<<endl;
	}
	paths[m][lenght]=node; //adding last node to solution not counting the first as last
}
void update_pheromones(int A, int n, int** paths, path** matrix, float Q, float rho) {
    for (int i = 1; i <= A; i++) {
        for (int j = 1; j <= A; j++) {
            matrix[i][j].pheromons *= (1 - rho);  // Apply evaporation
        }
    }
    for (int ant = 0; ant < n; ant++) {
        float path_length = 0.0;
        for (int i = 0; i < A - 1; i++) {
            int city1 = paths[i][ant];
            int city2 = paths[i + 1][ant];
            path_length += matrix[city1][city2].distance;
        }
	// Calculate distances
        path_length += matrix[paths[A - 1][ant]][paths[0][ant]].distance;

        for (int i = 0; i < A - 1; i++) {
            int city1 = paths[i][ant];
            int city2 = paths[i + 1][ant];
            float pheromone_deposit = Q / path_length;
            matrix[city1][city2].pheromons += pheromone_deposit;
            matrix[city2][city1].pheromons += pheromone_deposit;
        }
        int last_city = paths[A - 1][ant];
        int first_city = paths[0][ant];
        float pheromone_deposit = Q / path_length;
        matrix[last_city][first_city].pheromons += pheromone_deposit;
        matrix[first_city][last_city].pheromons += pheromone_deposit;
    }
}
void antclony(int n,path ** matrix,int ** paths)//g��wna funkcja
{
	int counter=0;
	while(counter<iterations)
	{
		for(int i=0;i<A;i++)//ant generation
		{
			ant(n,i,matrix,paths);
		}
		for(int i=0;i<A;i++) {for(int j=0;j<=n;j++) cout<<paths[i][j]<<" "; cout<<endl;} //paths created by ants
		update_pheromones(A,n,paths,matrix,Q,rho);
		counter++;
	}
}
int main()
{
	srand(time(NULL));//przygotowanie generatora liczb pseudolosowych
	int n;
	ifstream plik;
	plik.open("dane.txt");
	//plik.open("test.txt");
	plik>>n;
	points * list = new points[n+1];
	for(int i=1;i<=n;i++)
	{
		plik>>i>>list[i].X>>list[i].Y;
	}
	plik.close();
	//for(int i=1;i<=n;i++) cout<<i<<" "<<list[i].X<<" "<<list[i].Y<<endl;//wypisuje wczytane dane 
	path ** matrix;
	matrix = new path * [n+1];
	for (int i = 0; i <= n; i++) matrix[i] = new path[n+1];
	for (int i = 0; i <= n; i++) for (int j = 0; j <= n; j++) {matrix[i][j].distance=0;  matrix[i][j].pheromons=0.1;}
	matrixfill(n,matrix,list);
	for(int i=1;i<=n;i++) for(int j=1;j<=i;j++) matrix[j][i].distance=matrix[i][j].distance; //making matrix symetrical
	int ** paths;
	paths = new int*[A];
	for (int i=0;i<A;i++) paths[i] = new int[n+1];
	antclony(n,matrix,paths);
	//for(int i=1;i<=n;i++) {for(int j=1;j<=n;j++) cout<<macierz[i][j].distance<<" "; cout<<endl;}//wypisuje macierz odleg�o�ci
	for (int i = 0; i < A; i++) {
        delete[] paths[i];
    }
    delete[] paths;

    for (int i = 0; i <= n; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
	return 0;
}
