#ifndef TEST_HPP
#define TEST_HPP

#include <string>
#include <cassert>
#include <functional>
using namespace std;

template<typename T>
struct TestObject{
    string name;
    T result;
    T expected;
};

template<typename T>
void RunTests(const std::string& name, std::vector<function<TestObject<T>()>> tests){
    int passed = 0;
    for(auto& test : tests){
        const TestObject<T> t = test();
        if(t.result == t.expected){
            cout << t.name + " passed." << endl;
            passed++;
        }
        else{
            cout << t.name + " failed." << endl;
            cout << "Result: " << t.result << endl;
            cout << "Expected: " << t.expected << endl;
        }
    }
    std::cout << passed << "/" << tests.size() << " " + name + " tests passed." << std::endl;
}

#endif