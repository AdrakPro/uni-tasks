#include <algorithm>
#include <cassert>
#include <climits>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <mpi.h>
#include <vector>
#include <sstream>

using namespace std;

struct City {
    double x, y;
};

// Create MPI datatype for City
MPI_Datatype create_city_type() {
    MPI_Datatype city_type;
    int blocklengths[2] = {1, 1};
    MPI_Aint offsets[2] = {offsetof(City, x), offsetof(City, y)};
    MPI_Datatype types[2] = {MPI_DOUBLE, MPI_DOUBLE};
    MPI_Type_create_struct(2, blocklengths, offsets, types, &city_type);
    MPI_Type_commit(&city_type);
    return city_type;
}

pair<int, vector<City>> read_cities(const string &filename) {
    ifstream file(filename);
    string line;
    int dimension = 0;

    // Read dimension from file
    while (getline(file, line)) {
        if (line.find("DIMENSION") != string::npos) {
            size_t colon_pos = line.find(":");
            if (colon_pos != string::npos) {
                string dim_str = line.substr(colon_pos + 1);
                istringstream iss(dim_str);
                iss >> dimension;
            }
        }
        if (line.find("NODE_COORD_SECTION") != string::npos)
            break;
    }

    if (dimension == 0) {
        cerr << "Error: Could not determine problem dimension" << endl;
        exit(1);
    }

    vector<City> cities(dimension);
    for (int i = 0; i < dimension; ++i) {
        int id;
        double x, y;
        file >> id >> x >> y;
        cities[id - 1] = {x, y};
    }
    return make_pair(dimension, cities);
}

vector<double> compute_distance_matrix(const vector<City> &cities) {
    int n = cities.size();
    vector<double> dist(n * n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            double dx = cities[i].x - cities[j].x;
            double dy = cities[i].y - cities[j].y;
            dist[i * n + j] = sqrt(dx * dx + dy * dy);
        }
    }
    return dist;
}

vector<int> construct_tour(int start_city, const vector<double> &pheromone,
                           const vector<double> &dist, double alpha,
                           double beta) {
    int n = sqrt(pheromone.size()); // Get dimension from pheromone matrix size
    vector<int> tour;
    vector<bool> visited(n, false);
    tour.push_back(start_city);
    visited[start_city] = true;

    int current = start_city;
    for (int step = 1; step < n; ++step) {
        vector<double> probs;
        vector<int> candidates;
        double sum = 0.0;

        for (int next = 0; next < n; ++next) {
            if (!visited[next]) {
                double pheromone_level = pheromone[current * n + next];
                double heuristic = 1.0 / (dist[current * n + next] + 1e-10);
                double prob = pow(pheromone_level, alpha) * pow(heuristic, beta);
                probs.push_back(prob);
                candidates.push_back(next);
                sum += prob;
            }
        }

        if (sum == 0) { // Fallback to unvisited cities
            for (int next = 0; next < n; ++next) {
                if (!visited[next]) {
                    candidates.push_back(next);
                    probs.push_back(1.0);
                    sum += 1.0;
                }
            }
        }

        double rand_val = static_cast<double>(rand()) / RAND_MAX * sum;
        double cumulative = 0.0;
        int selected = -1;
        for (size_t i = 0; i < probs.size(); ++i) {
            cumulative += probs[i];
            if (cumulative >= rand_val) {
                selected = candidates[i];
                break;
            }
        }

        tour.push_back(selected);
        visited[selected] = true;
        current = selected;
    }
    assert(tour.size() == n);
    return tour;
}

double tour_length(const vector<int> &tour, const vector<double> &dist) {
    int n = sqrt(dist.size()); // Get dimension from distance matrix size
    double length = 0.0;
    for (size_t i = 0; i < tour.size() - 1; ++i) {
        length += dist[tour[i] * n + tour[i + 1]];
    }
    length += dist[tour.back() * n + tour.front()];
    return length;
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Datatype city_type = create_city_type();

    vector<City> cities;
    vector<double> dist;
    int dimension = 0;

    // Parameters
    double alpha = 1.2, beta = 5.0, rho = 0.5, Q = 200.0;
    int num_ants_per_process = 30, iterations = 100;

    if (rank == 0) {
        auto result = read_cities("burma14.txt");
        dimension = result.first;
        cities = result.second;
        dist = compute_distance_matrix(cities);
    }

    // Broadcast dimension
    MPI_Bcast(&dimension, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Broadcast cities
    cities.resize(dimension);
    MPI_Bcast(cities.data(), dimension, city_type, 0, MPI_COMM_WORLD);

    // Broadcast distance matrix
    if (rank != 0) {
        dist.resize(dimension * dimension);
    }
    MPI_Bcast(dist.data(), dimension * dimension, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    vector<double> pheromone(dimension * dimension, 1.0);
    srand(42 + rank); // Different seed for each process

    double global_best_length = numeric_limits<double>::max();
    vector<int> global_best_tour;

    for (int iter = 0; iter < iterations; ++iter) {
        double local_best_length = numeric_limits<double>::max();
        vector<int> local_best_tour;

        for (int a = 0; a < num_ants_per_process; ++a) {
            int start = rand() % dimension;
            vector<int> tour = construct_tour(start, pheromone, dist, alpha, beta);
            double length = tour_length(tour, dist);
            if (length < local_best_length) {
                local_best_length = length;
                local_best_tour = tour;
            }
        }

        // Gather results
        vector<double> all_lengths(size);
        vector<int> all_tours(size * dimension);

        MPI_Gather(&local_best_length, 1, MPI_DOUBLE, all_lengths.data(), 1,
                   MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Gather(local_best_tour.data(), dimension, MPI_INT, all_tours.data(), dimension,
                   MPI_INT, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            // Update global best
            int best_idx = min_element(all_lengths.begin(), all_lengths.end()) -
                           all_lengths.begin();
            double current_best = all_lengths[best_idx];
            vector<int> current_tour(all_tours.begin() + best_idx * dimension,
                                     all_tours.begin() + (best_idx + 1) * dimension);

            if (current_best < global_best_length) {
                global_best_length = current_best;
                global_best_tour = current_tour;
            }

            // Update pheromone
            for (auto &p : pheromone)
                p *= (1 - rho);
            double delta = Q / current_best;
            for (size_t i = 0; i < current_tour.size(); ++i) {
                int from = current_tour[i];
                int to = current_tour[(i + 1) % dimension];
                pheromone[from * dimension + to] += delta;
                pheromone[to * dimension + from] += delta;
            }

            // Print iteration and best tour length
            cout << "Iteration: " << iter + 1
                 << ", Best tour length: " << global_best_length << endl;
        }

        // Broadcast updated pheromone
        MPI_Bcast(pheromone.data(), dimension * dimension, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        cout << "Best tour length: " << global_best_length << endl;
        ofstream out("best_tour.txt");
        out << "DIMENSION : " << dimension << "\nTOUR_SECTION\n";
        for (int city : global_best_tour)
            out << city + 1 << "\n";
        out << "-1\n";
        out.close();
    }

    MPI_Type_free(&city_type);
    MPI_Finalize();
    return 0;
}
