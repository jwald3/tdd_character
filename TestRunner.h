#pragma once
#include <iostream>
#include <string>
#include <functional>

class TestRunner {
    public:
     static void runTest(const std::string& testName, std::function<bool()> test);
 };
 
 #define ASSERT_EQ(expected, actual)   \                                       
     ((expected) == (actual)) ? true : \
     (std::cout << "Assertion failed: " << expected << " != " << actual << std::endl,  false)