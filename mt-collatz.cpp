#include <iostream>
#include <thread>
using namespace std;
int collatz(int n, int i = 0) {
    if (n == 1)
        return i;
    else if (n % 2 == 0)
        return collatz(n / 2, ++i);
    else
        return collatz(3 * n + 1, ++i);
}
int main(int argc, char *argv[]) {
    //Program args
    const int N = argc >= 1 && argv[1] ? atoi(argv[1]) : 10000; //number of collatz stopping times to compute
    const int T = argc >= 2 && argv[2] ? atoi(argv[2]) : 8;     //number of threads to distribute computation

    //Create histogram
    const int HISTOGRAM_SIZE = 1000;
    int histogram[HISTOGRAM_SIZE] = {};

    //Calculate collatz # for each number from 2 to N
    //Increment collatz # frequency in histogram array
    for (int i = 2; i < N; i++)
        histogram[collatz(i)]++;

    //Print frequency of collatz #'s for frequencies > 0
    for (unsigned int i = 0; i < HISTOGRAM_SIZE; i++) {
        if (histogram[i] > 0)
            cout << i << ":" << histogram[i] << endl;
    }

    //TODO: Distribute the calculation of each collatz # to # of threads

    return 0;
}