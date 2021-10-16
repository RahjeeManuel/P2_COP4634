#include <iostream>
#include <thread>
#include <mutex>
#include <time.h>
#define MAX_THREADS 100
#define HISTOGRAM_SIZE 1000
#define BILLION 1E9
using namespace std;
int collatz(int n, int i = 0) {
    if (n == 1)
        return i;
    else if (n % 2 == 0)
        return collatz(n / 2, ++i);
    else
        return collatz(3 * n + 1, ++i);
}
int main(int argc, char* argv[]) {
    const int N = argc >= 1 && argv[1] ? atoi(argv[1]) : 10000;
    const int T = argc >= 2 && argv[2] ? atoi(argv[2]) : 8;
    const bool NO_LOCK = argc >= 3 && argv[3] ? string(argv[3]) == string("-nolock") : false;

    thread threads[MAX_THREADS];
    int histogram[HISTOGRAM_SIZE] = {};

    int counter = 2;
    mutex counter_mutex;
    auto record = [&]() {
        while (true) {
            if (NO_LOCK == true) {
                if (counter < N)
                    histogram[collatz(counter++)]++;
                else
                    return;
            } else {
                lock_guard<mutex> lock(counter_mutex);
                if (counter < N)
                    histogram[collatz(counter++)]++;
                else
                    return;
            }
        }
    };

    struct timespec start, stop;
    double elapsed_time;

    clock_gettime(CLOCK_REALTIME, &start);

    for (auto i = 0; i < T; i++){
        threads[i] = thread(record);
    }

    for (auto i = 0; i < T; i++) {
        if (threads[i].joinable())
            threads[i].join();
    }

    clock_gettime(CLOCK_REALTIME, &stop);
    elapsed_time = (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) / BILLION;

    for (auto i = 0; i < HISTOGRAM_SIZE; i++)
        cout << i << "," << histogram[i] << endl;

    cerr << N << "," << T << "," << elapsed_time << endl;

    return 0;
}
