#ifndef TEST_HPP
#define TEST_HPP

#include <string>
#include <cassert>
#include <functional>

// TODO: delete this file
template<typename T>
struct TestObject{
    std::string name;
    T result;
    T expected;
};

template<typename T>
void RunTests(const std::string& name, std::vector<std::function<TestObject<T>()>> tests){
    int passed = 0;
    for(auto& test : tests){
        const TestObject<T> t = test();
        if(t.result == t.expected){
            std::cout << t.name + " passed." << std::endl;
            passed++;
        }
        else{
            std::cout << t.name + " failed." << std::endl;
            std::cout << "Result: " << t.result << std::endl;
            std::cout << "Expected: " << t.expected << std::endl;
        }
    }
    std::cout << passed << "/" << tests.size() << " " + name + " tests passed." << std::endl;
}

#endif