#include <iostream>
#include <vector>
#include <chrono>
#include <string>


#include <threadpool.h>
#include <promise.hpp>
#include <condition.hpp>
#include <condition2.hpp>
#include <future.hpp>
#include <stream.hpp>

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
    void test()
    {
        std::cout << "test" << std::endl;
    }
};

void func(A &a)
{
    std::cout << "fun(A &)" << std::endl;
    a.test();
}

void run()
{

    A t(10);
    std::thread th(&func, std::ref(t));
    th.join();
   
}


int main()
{
    // vsomeip::condition::main();
    // vsomeip::condition2::main();
    // vsomeip::future::main();
    //vsomeip::promise::manager();
    // run();
    vsomeip::stream::run();

    return 0;
}
