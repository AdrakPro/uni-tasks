#include <mpi.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <cassert>
#include <random>

using namespace std;

struct Job {
    int p; // processing time
    int a; // earliness penalty
    int b; // tardiness penalty
};

MPI_Datatype create_job_type() {
    MPI_Datatype job_type;
    int blocklengths[3] = {1, 1, 1};
    MPI_Aint offsets[3];
    offsets[0] = offsetof(Job, p);
    offsets[1] = offsetof(Job, a);
    offsets[2] = offsetof(Job, b);
    MPI_Datatype types[3] = {MPI_INT, MPI_INT, MPI_INT};

    MPI_Type_create_struct(3, blocklengths, offsets, types, &job_type);
    MPI_Type_commit(&job_type);
    return job_type;
}

// Cost function
double evaluate_schedule(const vector<Job>& jobs, const vector<int>& schedule, int d) {
    int n = schedule.size();
    int completion_time = 0;
    double cost = 0.0;
    for (int i = 0; i < n; ++i) {
        int idx = schedule[i];
        completion_time += jobs[idx].p;
        if (completion_time < d)
            cost += jobs[idx].a * (d - completion_time);
        else
            cost += jobs[idx].b * (completion_time - d);
    }
    return cost;
}

void simulated_annealing(const vector<Job>& jobs, int d, vector<int>& best_schedule, double& best_cost) {
    int n = jobs.size();

    // Start with a random schedule.
    vector<int> current_schedule(n);
    iota(current_schedule.begin(), current_schedule.end(), 0);
    random_device rd;
    mt19937 gen(rd());
    shuffle(current_schedule.begin(), current_schedule.end(), gen);

    double current_cost = evaluate_schedule(jobs, current_schedule, d);
    best_schedule = current_schedule;
    best_cost = current_cost;

    double T = 1000.0;
    const double T_min = 1e-3;
    const double alpha = 0.995;  // cooling factor
    const int iter_per_temp = 1000;

    uniform_real_distribution<double> uniform_dist(0.0, 1.0);

    while (T > T_min) {
        for (int i = 0; i < iter_per_temp; i++) {
            // Create a neighbor solution by swapping two random jobs.
            vector<int> neighbor = current_schedule;
            int idx1 = gen() % n;
            int idx2 = gen() % n;
            swap(neighbor[idx1], neighbor[idx2]);

            double neighbor_cost = evaluate_schedule(jobs, neighbor, d);
            double delta = neighbor_cost - current_cost;

            if (delta < 0 || exp(-delta / T) > uniform_dist(gen)) {
                current_schedule = neighbor;
                current_cost = neighbor_cost;
                if (current_cost < best_cost) {
                    best_schedule = current_schedule;
                    best_cost = current_cost;
                }
            }
        }
        T *= alpha;
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Datatype job_type = create_job_type();

    vector<Job> jobs;
    int num_jobs = 0;
    double h = 0.6; // due date restrictiveness parameter;

    if (rank == 0) {
        if (argc < 2) {
            cerr << "Usage: " << argv[0] << " instance_filename" << endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        ifstream infile(argv[1]);
        if (!infile.is_open()) {
            cerr << "Error opening file " << argv[1] << endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        int num_problems;
        infile >> num_problems;

        infile >> num_jobs;
        jobs.resize(num_jobs);
        for (int i = 0; i < num_jobs; i++) {
            infile >> jobs[i].p >> jobs[i].a >> jobs[i].b;
        }
        infile.close();
    }

    MPI_Bcast(&num_jobs, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank != 0) {
        jobs.resize(num_jobs);
    }

    MPI_Bcast(jobs.data(), num_jobs, job_type, 0, MPI_COMM_WORLD);

    int sum_p = 0;
    for (int i = 0; i < num_jobs; i++) {
        sum_p += jobs[i].p;
    }
    int d = static_cast<int>(floor(sum_p * h));

    vector<int> local_best_schedule;
    double local_best_cost = numeric_limits<double>::max();
    simulated_annealing(jobs, d, local_best_schedule, local_best_cost);

    vector<double> all_costs(size);
    MPI_Gather(&local_best_cost, 1, MPI_DOUBLE, all_costs.data(), 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    vector<int> local_schedule_vec = local_best_schedule;
    vector<int> all_schedules;
    if (rank == 0) {
        all_schedules.resize(size * num_jobs);
    }
    MPI_Gather(local_schedule_vec.data(), num_jobs, MPI_INT, all_schedules.data(), num_jobs, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        int best_proc = 0;
        double global_best_cost = all_costs[0];
        for (int i = 1; i < size; i++) {
            if (all_costs[i] < global_best_cost) {
                global_best_cost = all_costs[i];
                best_proc = i;
            }
        }

        vector<int> global_best_schedule(num_jobs);
        copy(all_schedules.begin() + best_proc * num_jobs,
             all_schedules.begin() + (best_proc + 1) * num_jobs,
             global_best_schedule.begin());

        cout << "Best schedule found with cost " << global_best_cost << endl;
        cout << "Due date d = " << d << ", Sum of processing times = " << sum_p << endl;
        cout << "Job order (indices are zero-based):" << endl;
        for (int i = 0; i < num_jobs; i++) {
            cout << global_best_schedule[i] << " ";
        }
        cout << endl;

        ofstream outfile("best_schedule.txt");
        outfile << "Best schedule cost: " << global_best_cost << "\n";
        outfile << "Due date d: " << d << "\n";
        outfile << "Job order:" << "\n";
        for (int i = 0; i < num_jobs; i++) {
            outfile << global_best_schedule[i] << "\n";
        }
        outfile.close();
    }

    MPI_Type_free(&job_type);
    MPI_Finalize();
    return 0;
}
