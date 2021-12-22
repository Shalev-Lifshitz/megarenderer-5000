//
// Created by Callum Sh on 2021-12-22.
//

#include "main.h"

#include <iostream>

int main() {
    for (int i = 0; i < 11; ++i) {
        auto result = "iteration: " + std::to_string(i) + "\n";
        std::cout << result;
    }
}
