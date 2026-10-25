#include "lms/LatePenalty.h"
#include <cassert>
#include <cmath>
#include <iostream>
int main() {
    lms::SchoolLatePenalty penalty;
    assert(std::abs(penalty.apply(80, 100, 0) - 80) < 1e-9);
    assert(std::abs(penalty.apply(80, 100, 1) - 70) < 1e-9);
    assert(std::abs(penalty.apply(80, 100, 3) - 50) < 1e-9);
    assert(std::abs(penalty.apply(80, 100, 4) - 0) < 1e-9);
    assert(std::abs(penalty.apply(5, 100, 1) - 0) < 1e-9);
    std::cout << "Late penalty boundary tests passed.\n";
}
