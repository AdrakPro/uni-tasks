#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

void write_tour(const vector<int>& best_tour, const string& filename, int dimension) {
	ofstream out(filename);
	out << "DIMENSION : " << dimension << "\nTOUR_SECTION\n";
	for (int city : best_tour) out << city + 1 << "\n";
	out << "-1\n";
	out.close();
}

struct City {
	double x, y;
};

double euclidean_distance(const City& a, const City& b) {
	double dx = a.x - b.x;
	double dy = a.y - b.y;
	return sqrt(dx * dx + dy * dy);
}

vector<double> compute_distance_matrix(const vector<City>& cities) {
	int n = cities.size();
	vector<double> dist(n * n, 0.0);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (i != j) {
				dist[i * n + j] = euclidean_distance(cities[i], cities[j]);
			}
		}
	}
	return dist;
}

double total_distance(const vector<int>& tour, const vector<double>& dist, int n) {
	double total = 0.0;
	for (size_t i = 0; i < tour.size(); ++i) {
		int current_city_idx = tour[i];
		int next_city_idx = tour[(i + 1) % tour.size()];
		total += dist[current_city_idx * n + next_city_idx];
	}
	return total;
}

pair<int, vector<City>> read_cities(const string& filename) {
	ifstream file(filename);
	string line;
	int dimension = 0;
	bool reading_coords = false;

	while (getline(file, line)) {
		if (line.find("DIMENSION") != string::npos) {
			size_t colon_pos = line.find(":");
			if (colon_pos != string::npos) {
				string dim_str = line.substr(colon_pos + 1);
				istringstream iss(dim_str);
				iss >> dimension;
			} else {
				istringstream iss(line);
				string temp;
				iss >> temp >> temp;
				if (temp == ":") {
					iss >> dimension;
				} else {
					istringstream iss_val(temp);
					iss_val >> dimension;
				}
			}
		}
		if (line.find("NODE_COORD_SECTION") != string::npos) {
			reading_coords = true;
			break;
		}
	}

	if (dimension == 0) {
		cerr << "Error: Could not determine problem dimension from file: " << filename << endl;

		file.clear();
		file.seekg(0);
		while (getline(file, line)) {
			if (line.rfind("DIMENSION", 0) == 0) {
				istringstream iss(line);
				string keyword, colon;
				iss >> keyword >> colon >> dimension;
				if (iss.fail()) {
					iss.clear();
					iss.str(line);
					iss >> keyword >> dimension;
				}
				break;
			}
		}
		if (dimension == 0) {
			cerr << "Error: Still could not determine problem dimension." << endl;
			exit(1);
		}
	}

	vector<City> cities(dimension);
	if (reading_coords) {
		for (int i = 0; i < dimension; ++i) {
			int id;
			double x, y;
			file >> id >> x >> y;
			if (file.fail()) {
				cerr << "Error reading city data for city index " << i << endl;
				exit(1);
			}
			if (id < 1 || id > dimension) {
				cerr << "Error: City ID out of bounds: " << id << endl;
				exit(1);
			}
			cities[id - 1] = {x, y};
		}
	} else {
		file.clear();
		file.seekg(0);
		bool found_coord_again = false;
		while (getline(file, line)) {
			if (line.find("NODE_COORD_SECTION") != string::npos) {
				found_coord_again = true;
				for (int i = 0; i < dimension; ++i) {
					int id;
					double x, y;
					file >> id >> x >> y;
					if (file.fail()) {
						cerr << "Error reading city data for city index " << i << " (second attempt)" << endl;
						exit(1);
					}
					if (id < 1 || id > dimension) {
						cerr << "Error: City ID out of bounds: " << id << " (second attempt)" << endl;
						exit(1);
					}
					cities[id - 1] = {x, y};
				}
				break;
			}
		}
		if (!found_coord_again) {
			cerr << "Error: NODE_COORD_SECTION not found in file: " << filename << endl;
			exit(1);
		}
	}
	return make_pair(dimension, cities);
}

vector<vector<int>> compute_candidate_set(const vector<double>& dist, int n, int candidates_size = 40) {
	vector<vector<int>> candidate_set(n);
	for (int i = 0; i < n; ++i) {
		vector<pair<double, int>> neighbors;
		for (int j = 0; j < n; ++j) {
			if (i != j) {
				neighbors.emplace_back(dist[i * n + j], j);
			}
		}
		sort(neighbors.begin(), neighbors.end());
		candidate_set[i].reserve(min(candidates_size, (int)neighbors.size()));
		for (int k = 0; k < min(candidates_size, (int)neighbors.size()); ++k) {
			candidate_set[i].push_back(neighbors[k].second);
		}
	}
	return candidate_set;
}

vector<int> nearest_neighbor_tour(const vector<double>& dist, int n, mt19937& gen) {
	vector<int> tour;
	tour.reserve(n);
	vector<bool> visited(n, false);
	uniform_int_distribution<int> start_dist(0, n - 1);
	int current = start_dist(gen);
	tour.push_back(current);
	visited[current] = true;

	for (int i = 0; i < n - 1; ++i) {
		double min_val = numeric_limits<double>::max();
		int next_city = -1;
		for (int j = 0; j < n; ++j) {
			if (!visited[j] && dist[current * n + j] < min_val) {
				min_val = dist[current * n + j];
				next_city = j;
			}
		}
		if (next_city == -1) {
			cerr << "Error: No next city found in nearest neighbor construction. Visited count: " << tour.size() << "/"
			     << n << endl;
			break;
		}
		tour.push_back(next_city);
		visited[next_city] = true;
		current = next_city;
	}
	return tour;
}

vector<int> double_bridge_move(const vector<int>& tour, mt19937& gen) {
	int n = tour.size();
	if (n < 8) return tour;

	vector<int> new_tour;
	new_tour.reserve(n);

	uniform_int_distribution<int> dist_i(1, n - 7);
	int i = dist_i(gen);
	uniform_int_distribution<int> dist_j(i + 2, n - 5);
	int j = dist_j(gen);
	uniform_int_distribution<int> dist_k(j + 2, n - 3);
	int k = dist_k(gen);
	uniform_int_distribution<int> dist_l(k + 2, n - 1);
	int l = dist_l(gen);

	int p[4];
	p[0] = uniform_int_distribution<int>(0, n - 1 - 3 * 2)(gen);
	p[1] = uniform_int_distribution<int>(p[0] + 2, n - 1 - 2 * 2)(gen);
	p[2] = uniform_int_distribution<int>(p[1] + 2, n - 1 - 1 * 2)(gen);
	p[3] = uniform_int_distribution<int>(p[2] + 2, n - 1)(gen);
	sort(p, p + 4);

	new_tour.insert(new_tour.end(), tour.begin(), tour.begin() + i);
	new_tour.insert(new_tour.end(), tour.begin() + k, tour.begin() + l);
	new_tour.insert(new_tour.end(), tour.begin() + j, tour.begin() + k);
	new_tour.insert(new_tour.end(), tour.begin() + i, tour.begin() + j);
	new_tour.insert(new_tour.end(), tour.begin() + l, tour.end());

	return new_tour;
}

double compute_initial_temp(const vector<int>& tour, const vector<double>& dist, int n, mt19937& gen,
                            int samples = 1000) {
	double max_delta = 0.0;
	double min_delta = 0.0;
	double sum = 0.0;
	double sum_sq = 0.0;
	int count = 0;
	uniform_int_distribution<int> int_dist_indices(0, n - 1);

	for (int s = 0; s < samples; s++) {
		int i_idx = int_dist_indices(gen);
		int j_idx = int_dist_indices(gen);
		if (i_idx == j_idx) {
			j_idx = (j_idx + 1) % n;
		}
		if (i_idx > j_idx) swap(i_idx, j_idx);

		int city_at_i_idx = tour[i_idx];
		int city_at_j_idx = tour[j_idx];

		int prev_city_to_i_idx = tour[(i_idx - 1 + n) % n];
		int next_city_to_j_idx = tour[(j_idx + 1) % n];

		double delta = (dist[prev_city_to_i_idx * n + city_at_j_idx] + dist[city_at_i_idx * n + next_city_to_j_idx]) -
		               (dist[prev_city_to_i_idx * n + city_at_i_idx] + dist[city_at_j_idx * n + next_city_to_j_idx]);

		if (delta < min_delta) min_delta = delta;
		if (delta > max_delta) max_delta = delta;
		sum += delta;
		sum_sq += delta * delta;
		count++;
	}

	if (count == 0) return 10000.0;

	double mean = sum / count;
	double variance = (sum_sq / count) - (mean * mean);
	if (variance < 0) variance = 0;
	double stddev = sqrt(variance);

	double initial_temp = max(1000.0, 5.0 * stddev);

	initial_temp = max(10000.0, 20.0 * abs(mean) + 50.0 * stddev);

	cout << "Computed initial temperature: " << initial_temp << endl;
	return initial_temp;
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cerr << "Usage: " << argv[0] << " <tsp_file>\n";
		return 1;
	}
	string filename = argv[1];
	auto [dimension, cities] = read_cities(filename);
	vector<double> dist_matrix = compute_distance_matrix(cities);
	int n = dimension;

	mt19937 gen(static_cast<unsigned>(time(0)));

	vector<int> current_tour = nearest_neighbor_tour(dist_matrix, n, gen);
	double current_distance = total_distance(current_tour, dist_matrix, n);
	vector<int> best_tour = current_tour;
	double best_distance = current_distance;

	auto candidate_set = compute_candidate_set(dist_matrix, n, 40);

	double initial_temp = compute_initial_temp(current_tour, dist_matrix, n, gen, 1000);
	double temp = initial_temp;

	double cooling_rate = 0.9999;
	double min_temp = 1e-10;
	long max_iterations = 100000000;
	long iteration = 0;
	long no_improve_streak = 0;
	const long reheat_interval = 500000;

	uniform_real_distribution<double> real_dist_uni(0.0, 1.0);
	uniform_int_distribution<int> int_dist_uni(0, n - 1);

	cout << "Optimizing TSP with " << n << " cities\n";
	cout << "Initial tour (Nearest Neighbor): " << fixed << setprecision(1) << current_distance << endl;
	cout << "Target optimal: 11300" << endl;

	while (temp > min_temp && iteration < max_iterations) {
		vector<int> candidate_tour_data;
		double candidate_distance_val;

		bool use_double_bridge = (real_dist_uni(gen) < 0.05);

		if (use_double_bridge && n >= 8) {
			candidate_tour_data = double_bridge_move(current_tour, gen);
			candidate_distance_val = total_distance(candidate_tour_data, dist_matrix, n);
		} else {
			candidate_tour_data = current_tour;

			int i = int_dist_uni(gen);
			int j;

			if (real_dist_uni(gen) < 0.3) {
				j = int_dist_uni(gen);
				while (j == i) j = int_dist_uni(gen);
			} else {
				if (!candidate_set[current_tour[i]].empty()) {
					int idx1 = int_dist_uni(gen);
					int idx2 = int_dist_uni(gen);
					while (idx1 == idx2) idx2 = int_dist_uni(gen);

					int city_for_candidates = current_tour[i];
					if (!candidate_set[city_for_candidates].empty()) {
						int candidate_city = candidate_set[city_for_candidates][uniform_int_distribution<int>(
						    0, candidate_set[city_for_candidates].size() - 1)(gen)];

						auto it = find(current_tour.begin(), current_tour.end(), candidate_city);
						if (it != current_tour.end()) {
							j = distance(current_tour.begin(), it);
							if (j == i) {
								j = int_dist_uni(gen);
								while (j == i) j = int_dist_uni(gen);
							}
						} else {
							j = int_dist_uni(gen);
							while (j == i) j = int_dist_uni(gen);
						}
					} else {
						j = int_dist_uni(gen);
						while (j == i) j = int_dist_uni(gen);
					}
				} else {
					j = int_dist_uni(gen);
					while (j == i) j = int_dist_uni(gen);
				}
			}

			if (i == j) {
				j = (i + 1) % n;
			}
			if (i > j) swap(i, j);

			reverse(candidate_tour_data.begin() + i, candidate_tour_data.begin() + j + 1);

			int city_before_i = current_tour[(i - 1 + n) % n];
			int city_at_i = current_tour[i];
			int city_at_j = current_tour[j];
			int city_after_j = current_tour[(j + 1) % n];

			double delta_cost =
			    (dist_matrix[city_before_i * n + city_at_j] + dist_matrix[city_at_i * n + city_after_j]) -
			    (dist_matrix[city_before_i * n + city_at_i] + dist_matrix[city_at_j * n + city_after_j]);
			candidate_distance_val = current_distance + delta_cost;
		}

		double delta_abs = candidate_distance_val - current_distance;
		if (delta_abs < 0 || exp(-delta_abs / temp) > real_dist_uni(gen)) {
			current_tour = candidate_tour_data;
			current_distance = candidate_distance_val;

			if (current_distance < best_distance) {
				best_tour = current_tour;
				best_distance = current_distance;
				no_improve_streak = 0;
				cout << "New best: " << fixed << setprecision(1) << best_distance << " at iteration " << iteration
				     << " temp=" << scientific << setprecision(2) << temp << fixed << endl;
			}
		} else {
			no_improve_streak++;
		}

		temp *= cooling_rate;
		iteration++;

		if (no_improve_streak >= reheat_interval && initial_temp > 0) {
			temp = max(temp * 2.0, initial_temp * 0.1);
			no_improve_streak = 0;
			cout << "Reheating to " << scientific << setprecision(2) << temp << fixed << " at iteration " << iteration
			     << endl;
		}

		if (iteration % 100000 == 0) {
			cout << "Iteration: " << iteration << " Temp: " << scientific << setprecision(2) << temp
			     << " Curr: " << fixed << setprecision(1) << current_distance << " Best: " << best_distance << endl;
		}

		if (iteration % 500000 == 0) {
			current_distance = total_distance(current_tour, dist_matrix, n);
		}
	}

	write_tour(best_tour, "best_tour.txt", n);
	cout << "\nOptimization complete\n";
	cout << "Final best distance: " << fixed << setprecision(1) << best_distance << endl;
	cout << "Optimal target: 11300" << endl;
	if (11300 > 0) {
		cout << "Deviation: " << fixed << setprecision(1) << 100.0 * (best_distance - 11300) / 11300 << "%\n";
	}
	cout << "Tour saved to best_tour.txt" << endl;

	return 0;
}
