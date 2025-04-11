#include "TestRunner.h"

void TestRunner::runTest(const std::string& testName, std::function<bool()> test) {
    std::cout << "Running test: " << testName << "...";
    if (test()) {
        std::cout << "PASSED" << std::endl;
    } else {
        std::cout << "FAILED" << std::endl;
    }
}