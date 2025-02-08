#include "dijkstra.h"

const int SIZE = 3200;
const double DENSITY = 1;
const double MIN_WEIGHT = 1.0;
const double MAX_WEIGHT = 10.0;
const double INF = numeric_limits<double>::infinity();

using Edge = pair<int, double>;
using Vertex = pair<int, double>;

Graph::Graph(int size, double density, double min_weight,
						 double max_weight, int seed) : num_vertices(size), num_edges(0) {
	mt19937 gen;
	gen.seed(seed);
	uniform_real_distribution<double> rand_weight(min_weight, max_weight);
	uniform_real_distribution<double> probability(0, 1);

	adj_list = new vector<Edge>[size];
	adj_matrix = vector<vector<Edge>>(
			num_vertices, vector<Edge>(num_vertices, make_pair(-1, INF)));

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			seed += 2 * j + 1;
			gen.seed(seed);
			if (i != j && probability(gen) < density) {
				double weight = rand_weight(gen);
				addEdge(i, j, weight);
			}
		}
	}
}


void Graph::addEdge(int source, int target, double weight) {
	// List
	adj_list[source].emplace_back(target, weight);
	adj_list[target].emplace_back(source, weight);
	// Matrix
	adj_matrix[source][target] = make_pair(target, weight);
	adj_matrix[target][source] = make_pair(source, weight);
	num_edges++;
}

int Graph::getNumVertices() const {
	return num_vertices;
}


int Graph::getNumEdges() const {
	return num_edges;
}

double Graph::dijkstraAdjList(int source) {
	priority_queue<Vertex, vector<Vertex>, greater<>> Q;
	vector<double> dist(getNumVertices(), INF);

	dist[source] = 0;
	Q.emplace(source, 0);

	while (!Q.empty()) {
		// Locator
		int u = Q.top().first;
		Q.pop();

		for (auto &e: adj_list[u]) {
			// End vertex
			int z = e.first;
			double weight = e.second;
			double r = dist[u] + weight;

			// Relaxation
			if (r < dist[z]) {
				dist[z] = r;
				Q.emplace(z, dist[z]);
			}
		}
	}

	return avgDistances(dist);
}

double Graph::dijkstraAdjMatrix(int source) {
	priority_queue<Vertex, vector<Vertex>, greater<>> Q;
	vector<double> dist(num_vertices, INF);

	dist[source] = 0;
	Q.emplace(source, 0);

	while (!Q.empty()) {
		// Locator
		int u = Q.top().first;
		Q.pop();

		for (int v = 0; v < num_vertices; ++v) {
			// End vertex
			int z = adj_matrix[u][v].first;
			double weight = adj_matrix[u][v].second;
			double r = dist[u] + weight;

			// Relaxation
			if (r < dist[z]) {
				dist[z] = r;
				Q.emplace(z, dist[z]);
			}
		}
	}

	return avgDistances(dist);
}

double Graph::avgDistances(const vector<double> &dist) const {
	double result = 0.0;

	for (double i: dist) {
		result += i;
	}

	return result / (getNumVertices() - 1);
}

void Graph::printAdjList() {
	cout << "AdjList Locator (Weight)" << endl;
	for (int i = 0; i < num_vertices; ++i) {
		cout << "Vertex " << i << " :";
		for (const auto &edge: adj_list[i]) {
			cout << " -> " << edge.first << " (" << edge.second << ")";
		}
		cout << endl;
	}
}

void Graph::printAdjMatrix() {
	cout << "AdjMatrix Locator (Weight)" << endl;
	for (int i = 0; i < num_vertices; ++i) {
		for (int j = 0; j < num_vertices; ++j) {
			if (adj_matrix[i][j].first != -1) {
				cout << adj_matrix[i][j].first << " (" << adj_matrix[i][j].second << ") ";
			} else {
				cout << " INF ";
			}
		}
		cout << endl;
	}
}

Driver::Driver(Graph* graph) {
	this->G = graph;
}

void Driver::test() const {
	getGraph()->printAdjList();
	cout << "Avg distance: " << getGraph()->dijkstraAdjList(0) << endl;
	cout << "============================" << endl;
	cout << "Avg distance: " << getGraph()->dijkstraAdjList(0) << endl;
	getGraph()->printAdjMatrix();
}

Graph* Driver::getGraph() const {
	return G;
}

int main() {
	// Driver
 	auto* driver_graph = new Graph(5, DENSITY, MIN_WEIGHT, MAX_WEIGHT, 50);
 	auto* driver = new Driver(driver_graph);
 	driver->test();
 	delete driver_graph;
 	delete driver;

	// Measurements
	long result_list = 0.0;
	long result_matrix = 0.0;


	cout << "Density: " << DENSITY << endl;

	std::chrono::high_resolution_clock::time_point start, end, start2, end2;

	for (int i = 0; i < 100; ++i) {
		Graph graph = Graph(SIZE, DENSITY, MIN_WEIGHT, MAX_WEIGHT, 50 + i);

		start = std::chrono::high_resolution_clock::now();
		graph.dijkstraAdjList(0);
		end = std::chrono::high_resolution_clock::now();

		start2 = std::chrono::high_resolution_clock::now();
		graph.dijkstraAdjMatrix(0);
		end2 = std::chrono::high_resolution_clock::now();

		result_list += std::chrono::duration_cast<std::chrono::nanoseconds>(
				end - start
		).count();

		result_matrix += std::chrono::duration_cast<std::chrono::nanoseconds>(
				end2 - start2
		).count();
	}

	result_list /= 100;
	result_matrix /= 100;

	cout << "Avg time for adj_list: " << result_list << " ns!" << endl;
	cout << "Avg time for adj_matrix: " << result_matrix << " ns!" << endl;

	return EXIT_SUCCESS;
}

