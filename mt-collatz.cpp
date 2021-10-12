#include <iostream>
#include <thread>
#define MAX_THREADS 100
#define HISTOGRAM_SIZE 1000
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

    thread threads[MAX_THREADS];
    int histogram[HISTOGRAM_SIZE] = {};

    int counter = 2;
    auto record = [&]() {
        while (counter <= N)
            histogram[collatz(counter++)]++;    
    };

    for (auto i = 0; i < T; i++)
        threads[i] = thread(record);

    for (auto i = 0; i < T; i++) {
        if (threads[i].joinable())
            threads[i].join();
    }

    for (auto i = 0; i < HISTOGRAM_SIZE; i++) {
        if (histogram[i] > 0)
            cout << i << ":" << histogram[i] << endl;
    }

    //TODO: race condition fix
    
    return 0;
}