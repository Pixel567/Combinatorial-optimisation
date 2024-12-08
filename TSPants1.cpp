#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;

const int A = 20;        // number of ants in generation
const int fer = 12;       // power of pheromons in choosing next city
const int dis = 6;       // power of distance in choosing next city
const float rho = 0.5;   // Evaporation rate
const float Q = 100.0;    // Pheromone deposition constant
const int iterations = 2000;

struct points {
    int X;
    int Y;
};

struct path {
    float pheromons;
    float distance;
};

void matrixfill(int n, path **matrix, points *list) {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j < i; j++) {
            if (i != j) {
                matrix[i][j].distance = sqrt(pow(list[i].X - list[j].X, 2) + pow(list[i].Y - list[j].Y, 2));
                matrix[j][i].distance = matrix[i][j].distance; // making matrix symmetrical
            }
        }
    }
}

void ant(int n, int m, path **matrix, int **paths) {
    int **copy = new int *[n + 1];
    for (int i = 0; i <= n; i++) {
        copy[i] = new int[n + 1];
    }

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            copy[i][j] = matrix[i][j].distance;
        }
    }

    int node = rand() % n + 1;  // Choosing a random first city
    paths[m][0] = node;

    for (int k = 1; k <= n; k++) {
        copy[k][node] = 0;  // Removing paths to the first city
    }

    vector<int> c;
    int length = 1;

    while (length < n) {
        vector<float> probability(n + 1, 0);
        for (int i = 1; i <= n; i++) {
            if (copy[node][i] != 0) {
                probability[i] = pow(matrix[node][i].pheromons, fer) * pow(matrix[node][i].distance, -dis);
            }
        }

        // Normalize the probability distribution
        float total_prob = 0;
        for (int i = 1; i <= n; i++) {
            total_prob += probability[i];
        }
        for (int i = 1; i <= n; i++) {
            probability[i] /= total_prob;
        }

        // Choose next node based on probability
        float rand_val = (float)rand() / RAND_MAX;
        float cumulative_prob = 0;
        for (int i = 1; i <= n; i++) {
            cumulative_prob += probability[i];
            if (rand_val <= cumulative_prob) {
                node = i;
                break;
            }
        }

        paths[m][length] = node;
        length++;

        // Remove paths to the chosen city
        for (int i = 1; i <= n; i++) {
            copy[i][node] = 0;
        }
    }

    delete[] copy;
}

void update_pheromones(int A, int n, int **paths, path **matrix, float Q, float rho) {
    // Evaporation
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            matrix[i][j].pheromons *= (1 - rho);
        }
    }

    // Pheromone deposition based on ant paths
    for (int ant = 0; ant < A; ant++) {
        float path_length = 0.0;
        for (int i = 0; i < n - 1; i++) {
            int city1 = paths[ant][i];
            int city2 = paths[ant][i + 1];
            path_length += matrix[city1][city2].distance;
        }

        // Add the return distance to the first city
        path_length += matrix[paths[ant][n - 1]][paths[ant][0]].distance;

        for (int i = 0; i < n - 1; i++) {
            int city1 = paths[ant][i];
            int city2 = paths[ant][i + 1];
            float pheromone_deposit = Q / path_length;
            matrix[city1][city2].pheromons += pheromone_deposit;
            matrix[city2][city1].pheromons += pheromone_deposit;
        }

        // Add the last leg of the tour
        int last_city = paths[ant][n - 1];
        int first_city = paths[ant][0];
        float pheromone_deposit = Q / path_length;
        matrix[last_city][first_city].pheromons += pheromone_deposit;
        matrix[first_city][last_city].pheromons += pheromone_deposit;
    }
}

void antcolony(int n, path **matrix, int **paths) {
    int best_length = INT_MAX;
    int best_index = -1;
    int overall_best_length = INT_MAX;
    int overall_best_index = -1;

    for (int iteration = 0; iteration < iterations; iteration++) {
        for (int i = 0; i < A; i++) {
            ant(n, i, matrix, paths);
        }

        // Find the best path in this iteration
        int iteration_best_length = INT_MAX;
        int iteration_best_index = -1;
        for (int i = 0; i < A; i++) {
            float total_length = 0;
            for (int j = 0; j < n - 1; j++) {
                int city1 = paths[i][j];
                int city2 = paths[i][j + 1];
                total_length += matrix[city1][city2].distance;
            }

            // Add the return distance to the first city
            total_length += matrix[paths[i][n - 1]][paths[i][0]].distance;

            if (total_length < iteration_best_length) {
                iteration_best_length = total_length;
                iteration_best_index = i;
            }
        }

        if (iteration_best_length < overall_best_length) {
            overall_best_length = iteration_best_length;
            overall_best_index = iteration_best_index;
        }

        update_pheromones(A, n, paths, matrix, Q, rho);
    }

    // Output the best path found
    cout << "Best Overall Path: ";
    for (int i = 0; i < n; i++) {
        cout << paths[overall_best_index][i] << " ";
    }
    cout << endl << "Length: " << overall_best_length << endl;
}

void generator(int n, points *list) {
    for (int i = 1; i <= n; i++) {
        list[i].X = rand() % 20;
        list[i].Y = rand() % 20;
        cout << i << " " << list[i].X << " " << list[i].Y << endl;
    }
}

int main() {
    srand(time(NULL));

    int n = 20;

    // Load data from file or generate random data
    ifstream plik("dane.txt");
    plik >> n;

    points *list = new points[n + 1];
    for (int i = 1; i <= n; i++) {
        plik >> i >> list[i].X >> list[i].Y;
    }
    plik.close();

    path **matrix = new path *[n + 1];
    for (int i = 0; i <= n; i++) {
        matrix[i] = new path[n + 1];
    }

    // Initialize matrix
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            matrix[i][j].distance = 0;
            matrix[i][j].pheromons = 0.1;
        }
    }

    matrixfill(n, matrix, list);

    int **paths = new int *[A];
    for (int i = 0; i < A; i++) {
        paths[i] = new int[n];
    }

    antcolony(n, matrix, paths);

    // Clean up memory
    delete[] list;
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
