#include <iostream>
#include <cmath>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

double partial_sum(int start, int end) {
    double sum = 0.0;
    for (int i = start; i <= end; ++i) {
        sum += 1.0 / i;
    }
    return sum;
}

void client(int start, int end, double &result, mutex &m) {
    double sum = partial_sum(start, end);

    lock_guard<mutex> lock(m);
    result += sum;
}

void server(int n, int p) {
    double total_sum = 0.0;
    mutex m;

    int base = n / p;
    int rem = n % p;

    vector<thread> threads;

    int current = 1;
    for (int i = 0; i < p; ++i) {
        int count = base + (i < rem ? 1 : 0);
        int start = current;
        int end = current + count - 1;
        current = end + 1;

        threads.push_back(thread(client, start, end, ref(total_sum), ref(m)));
    }

    for (auto& t : threads) {
        t.join();
    }

    double g_n = total_sum - log(n);

    cout << "g_n = " << g_n << endl;
}

int main() {
    int n, p;

    cout << "Podaj liczbę elementów sumy (n): ";
    cin >> n;
    cout << "Podaj liczbę wątków (p): ";
    cin >> p;

    server(n, p);

    return 0;
}
