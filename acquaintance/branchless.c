#include <stdlib.h>
#include <assert.h>

int main() {
    int array[100] = {};
    for (size_t i = 0; i < 100; ++i) {
        array[i] = i;
    }

    size_t counts[2] = {};
    for (size_t i = 0; i < 100; ++i) {
        if (array[i] % 10 == 0) {
            counts[1] += 1;
        } else {
            counts[0] += 1;
        }
        // counts[(array[i] % 10 == 0)] += 1;
    }

    assert(counts[0] > counts[1]);
}
