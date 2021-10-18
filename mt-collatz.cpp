#include <iostream>
#include <thread>
#include <mutex>
#include <time.h>
#define MAX_THREADS 100
#define HISTOGRAM_SIZE 1000
#define BILLION 1E9
using namespace std;

//Calculates the collatz # for n
int collatz(long n) {
    int i = 0;
    while (n != 1) {
        if (n % 2 == 0)
            n /= 2;
        else
            n = (3 * n) + 1;
        i++;
    }
    return i;
}

int main(int argc, char* argv[]) {
    //Program args
    const int N = argc >= 1 && argv[1] ? atoi(argv[1]) : 10000;
    const int T = argc >= 2 && argv[2] ? atoi(argv[2]) : 8;
    const bool NO_LOCK = argc >= 3 && argv[3] ? string(argv[3]) == string("-nolock") : false;

    //Create thread and histogram array
    thread threads[MAX_THREADS];
    int histogram[HISTOGRAM_SIZE] = {};
    
    //Lambda function to find the collat # for each N
    //Records each frequency into the histogram array
    //Uses lock_guard if -nolock parameter exists
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
                lock_guard<mutex> lock(counter_mutex);  //lock_guard locks all code in the same scope. this unlocks when leaving its scope
                if (counter < N)
                    histogram[collatz(counter++)]++;
                else
                    return;
            }
        }
    };

    //Clock variables
    struct timespec start, stop;
    double elapsed_time;

    //Get start time
    clock_gettime(CLOCK_REALTIME, &start);

    //record() is ran for T # of threads
    for (auto i = 0; i < T; i++){
        threads[i] = thread(record);
    }

    //Join all threads when finished
    for (auto i = 0; i < T; i++) {
        if (threads[i].joinable())
            threads[i].join();
    }

    //Get stop time
    clock_gettime(CLOCK_REALTIME, &stop);
    elapsed_time = (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) / BILLION;

    //Print histogram array to cout
    for (auto i = 0; i < HISTOGRAM_SIZE; i++)
        cerr << i << "," << histogram[i] << endl;

    //Print elapsed_time to cerr
    //cerr << N << "," << T << "," << elapsed_time << endl;
    
    return 0;
}
