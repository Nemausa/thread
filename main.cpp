#include <iostream>
#include <vector>
#include <chrono>
#include <string>

#include <threadpool.h>
#include <promise.hpp>
#include <condition.hpp>
#include <condition2.hpp>
#include <future.hpp>
#include <con_test.hpp>
#include "task_impl.hpp"


int main()
{
    task_impl task;
    int count = 1;
    while (count <200)
    {
        printf("main\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
       
        count++;
    }
    printf("-----------\n");
    return 0;
}
