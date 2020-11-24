#include <iostream>
#include <vector>
#include <chrono>
#include <string>


#include "threadpool.h"
#include "promise.hpp"
#include "condition.hpp"
#include "condition2.hpp"
#include "future.hpp"


class A
{
private:
    int data_;

public:
    A(int _data):data_(_data)
    {
        std::cout << "constructor: " << this << std::endl;
        _data += 10;
    }

    A(const A& a):data_(a.data_)
    {
        std::cout << "copy constructor: " << this << std::endl;
    }

    ~A()
    {
        std::cout << "destructor: " << this << std::endl;
    }

    void threadFunc(std::string &str, int a)
    {
        str = "change by threadFunc";
        a = 13;
    }
};

void run()
{
    std::string str("main");

    A t(10);
    int a = 10;
    std::thread th(&A::threadFunc, std::ref(t), std::ref(str), a);
    th.detach();

    std::cout << "str = " << str << std::endl;
    std::cout << "a = " << a << std::endl;
    
    
}


int main()
{
    vsomeip::condition::main();
    vsomeip::condition2::main();
    vsomeip::future::main();
    
    return 0;
}
