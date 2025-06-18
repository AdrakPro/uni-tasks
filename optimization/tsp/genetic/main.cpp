#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <vector>
#include <random>
#include <unordered_set>

using namespace std;

struct City {
    double x, y;
};

pair<int, vector<City>> read_cities(const string &filename) {
    ifstream file(filename);
    string line;
    int dimension = 0;

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

double tour_length(const vector<int> &tour, const vector<double> &dist) {
    int n = static_cast<int>(sqrt(dist.size()));
    double length = 0.0;
    for (size_t i = 0; i < tour.size() - 1; ++i) {
        length += dist[tour[i] * n + tour[i + 1]];
    }
    length += dist[tour.back() * n + tour.front()];
    return length;
}

// Initialize population with random permutations
vector<vector<int>> initialize_population(int pop_size, int num_cities) {
    vector<vector<int>> population(pop_size, vector<int>(num_cities));
    for (int i = 0; i < pop_size; ++i) {
        for (int j = 0; j < num_cities; ++j) {
            population[i][j] = j;
        }
        random_shuffle(population[i].begin() + 1, population[i].end());
    }
    return population;
}

vector<int> tournament_selection(const vector<vector<int>>& population,
                                 const vector<double>& fitness,
                                 int tournament_size) {
    int pop_size = population.size();
    vector<int> best_indv;
    double best_fitness = numeric_limits<double>::max();

    for (int i = 0; i < tournament_size; ++i) {
        int idx = rand() % pop_size;
        if (fitness[idx] < best_fitness) {
            best_fitness = fitness[idx];
            best_indv = population[idx];
        }
    }
    return best_indv;
}

pair<vector<int>, vector<int>> ordered_crossover(const vector<int>& parent1,
                                                const vector<int>& parent2) {
    int n = parent1.size();
    vector<int> child1(n, -1), child2(n, -1);
    int start = rand() % n;
    int end = start + rand() % (n - start);

    // Copy segment from parents
    for (int i = start; i <= end; ++i) {
        child1[i] = parent1[i];
        child2[i] = parent2[i];
    }

    auto fill_child = [n](const vector<int>& parent, vector<int>& child, int start, int end) {
        vector<int> remaining;
        for (int city : parent) {
            if (find(child.begin(), child.end(), city) == child.end()) {
                remaining.push_back(city);
            }
        }

        int idx = 0;
        for (int i = 0; i < n; ++i) {
            int pos = (end + i) % n;
            if (child[pos] == -1) {
                child[pos] = remaining[idx++];
            }
        }
    };

    fill_child(parent2, child1, start, end);
    fill_child(parent1, child2, start, end);
    return make_pair(child1, child2);
}

void swap_mutation(vector<int>& individual, double mutation_prob) {
    if ((rand() / static_cast<double>(RAND_MAX)) >= mutation_prob) return;

    int n = individual.size();
    int idx1 = rand() % n;
    int idx2 = rand() % n;
    while (idx1 == idx2) idx2 = rand() % n;
    swap(individual[idx1], individual[idx2]);
}

int main() {
    srand(time(0));

    // Parameters
    const string filename = "lu980.txt";
    const int population_size = 100;
    const int generations = 1000;
    const int tournament_size = 5;
    const double crossover_prob = 0.8;
    const double mutation_prob = 0.2;
    const double elitism_rate = 0.05;

    auto result = read_cities(filename);
    int dimension = result.first;
    vector<City> cities = result.second;
    vector<double> dist = compute_distance_matrix(cities);

    vector<vector<int>> population = initialize_population(population_size, dimension);
    vector<double> fitness(population_size);
    double global_best = numeric_limits<double>::max();
    vector<int> global_best_tour;

    for (int gen = 0; gen < generations; ++gen) {
        for (int i = 0; i < population_size; ++i) {
            fitness[i] = tour_length(population[i], dist);
            if (fitness[i] < global_best) {
                global_best = fitness[i];
                global_best_tour = population[i];
            }
        }

        // Create new population with elitism
        vector<vector<int>> new_population;
        int elite_count = static_cast<int>(population_size * elitism_rate);

        // Select elite individuals
        vector<pair<double, int>> fitness_index;
        for (int i = 0; i < population_size; ++i) {
            fitness_index.emplace_back(fitness[i], i);
        }
        sort(fitness_index.begin(), fitness_index.end());
        for (int i = 0; i < elite_count; ++i) {
            new_population.push_back(population[fitness_index[i].second]);
        }

        while (new_population.size() < population_size) {
            // Selection
            vector<int> parent1 = tournament_selection(population, fitness, tournament_size);
            vector<int> parent2 = tournament_selection(population, fitness, tournament_size);

            pair<vector<int>, vector<int>> children;
            if ((rand() / static_cast<double>(RAND_MAX)) < crossover_prob) {
                children = ordered_crossover(parent1, parent2);
            } else {
                children = {parent1, parent2};
            }

            // Mutation
            swap_mutation(children.first, mutation_prob);
            swap_mutation(children.second, mutation_prob);

            new_population.push_back(children.first);
            if (new_population.size() < population_size) {
                new_population.push_back(children.second);
            }
        }

        population = new_population;

        if (gen % 10 == 0) {
            cout << "Generation " << gen
                 << ", Best = " << global_best
                 << ", Current best = " << fitness_index[0].first << endl;
        }
    }

    cout << "\nBest tour length: " << global_best << endl;
    ofstream out("ga_best_tour.txt");
    out << "DIMENSION : " << dimension << "\nTOUR_SECTION\n";
    for (int city : global_best_tour) {
        out << city + 1 << "\n";
    }
    out << "-1\n";
    out.close();

    return 0;
}
