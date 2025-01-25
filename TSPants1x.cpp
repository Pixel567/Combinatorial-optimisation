#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <climits>
#include <cstdlib>
#include <ctime>

using namespace std;

const int A = 20;        // Number of ants
const int fer = 3;       // Power of pheromones in choosing the next city
const int dis = 3;       // Power of distance in choosing the next city
const float rho = 0.2;   // Pheromone evaporation coefficient
const float Q = 100.0;   // Pheromone deposit constant
const int iterations = 500; // Number of iterations

struct points {
    int X;
    int Y;
};

struct path {
    float pheromons;
    float distance;
};

// Filling matrix and initializing pheromones
void matrixfill(int n, path **matrix, points *list) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) {
                matrix[i][j].distance = sqrt(pow(list[i].X - list[j].X, 2) + pow(list[i].Y - list[j].Y, 2));
                matrix[i][j].pheromons = 0.1; // Initial pheromones
            }
        }
    }
}

// Generating path for an ant
void ant(int n, int m, path **matrix, int **paths) {
    vector<bool> visited(n, false);
    int node = rand() % n; // Losowy start
    paths[m][0] = node;
    visited[node] = true;

    for (int length = 1; length < n; length++) {
        vector<float> probabilities(n, 0.0);
        float total_prob = 0.0;

        // Probability caluclating
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                probabilities[i] = pow(matrix[node][i].pheromons, fer) * pow(1.0 / matrix[node][i].distance, dis);
                total_prob += probabilities[i];
            }
        }

        // If probability equals 0, visit randomly chosen city
        if (total_prob <= 0) {
            for (int i = 0; i < n; i++) {
                if (!visited[i]) {
                    node = i;
                    break;
                }
            }
        } else {
            for (int i = 0; i < n; i++) {
                probabilities[i] /= total_prob;
            }

            float rand_val = (float)rand() / RAND_MAX;
            float cumulative_prob = 0.0;
            for (int i = 0; i < n; i++) {
                if (!visited[i]) {
                    cumulative_prob += probabilities[i];
                    if (rand_val <= cumulative_prob) {
                        node = i;
                        break;
                    }
                }
            }
        }

        paths[m][length] = node;
        visited[node] = true;
    }

    // Going back to the first city
    paths[m][n] = paths[m][0];
}

// Updating pheromones
void update_pheromones(int A, int n, int **paths, path **matrix, float Q, float rho) {
    // Pheromone evaporation
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j].pheromons *= (1 - rho);
        }
    }

    for (int ant = 0; ant < A; ant++) {
        float path_length = 0.0;
        for (int i = 0; i < n; i++) {
            int city1 = paths[ant][i];
            int city2 = paths[ant][i + 1];
            path_length += matrix[city1][city2].distance;
        }

        for (int i = 0; i < n; i++) {
            int city1 = paths[ant][i];
            int city2 = paths[ant][i + 1];
            matrix[city1][city2].pheromons += Q / path_length;
            matrix[city2][city1].pheromons += Q / path_length; // Symmetry
        }
    }
}

void antcolony(int n, path **matrix, int **paths) {
    int overall_best_length = INT_MAX;
    int *best_path = new int[n + 1];

    for (int iteration = 0; iteration < iterations; iteration++) {
        for (int i = 0; i < A; i++) {
            ant(n, i, matrix, paths);
        }

        for (int i = 0; i < A; i++) {
            float total_length = 0.0;
            for (int j = 0; j < n; j++) {
                int city1 = paths[i][j];
                int city2 = paths[i][j + 1];
                total_length += matrix[city1][city2].distance;
            }

            if (total_length < overall_best_length) {
                overall_best_length = total_length;
                for (int j = 0; j <= n; j++) {
                    best_path[j] = paths[i][j];
                }
            }
        }

        update_pheromones(A, n, paths, matrix, Q, rho);
    }

    cout << "Best Overall Path: ";
    for (int i = 0; i <= n; i++) {
        cout << best_path[i] << " ";
    }
    cout << endl << "Length: " << overall_best_length << endl;

    delete[] best_path;
}

int main() {
    srand(time(NULL));

    ifstream plik("dane.txt");
    int n;
    plik >> n;

    points *list = new points[n];
    for (int i = 0; i < n; i++) {
        int index; // Read and discard the index from the file
        plik >> index >> list[i].X >> list[i].Y;
    }
    plik.close();

    path **matrix = new path *[n];
    for (int i = 0; i < n; i++) {
        matrix[i] = new path[n];
    }

    matrixfill(n, matrix, list);

    int **paths = new int *[A];
    for (int i = 0; i < A; i++) {
        paths[i] = new int[n + 1];
    }

    antcolony(n, matrix, paths);

    // Clearing memory
    delete[] list;
    for (int i = 0; i < n; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;

    for (int i = 0; i < A; i++) {
        delete[] paths[i];
    }
    delete[] paths;

    return 0;
}
