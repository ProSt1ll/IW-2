#include <gtest/gtest.h>
#include "recommendation.h"
#include <ctime>

#define NUM_OF_TESTS 5

int main() {
    double timer = 0;
    data_source base = create_random_data(100000, 16000);
    for (size_t i = 0; i < NUM_OF_TESTS; i++) {
        clock_t begin = clock();
        refresh_rec(&base);
        recommend(&base, i);
        clock_t end = clock();
        timer += (double) (end - begin) / CLOCKS_PER_SEC;
    }
    delete_data(&base);
    double avg_time = timer / NUM_OF_TESTS;
    std::cout << "time: " << avg_time << std::endl;
    return 0;
}