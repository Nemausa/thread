/**
* @file stream.hpp
*
* @brief This message displayed in Doxygen Files index
*
* @ingroup PackageName
* (note: this needs exactly one @defgroup somewhere)
*
* @date	2021-03-31-17-40
* @author Nemausa
* @contact: tappanmorris@outlook.com
*
*/
#ifndef CON_TEST
#define CON_TEST

#include <iostream>           // std::cout
#include <thread>             // std::thread
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
#include <cstring>

namespace vsomeip {
namespace stream {

typedef enum {
    NEW_DATA = 0X0,
    OLD_DATA = 0X1
} buffer_info_e;

std::mutex mutex_;
std::condition_variable cv_;
buffer_info_e  buffer_info = buffer_info_e::OLD_DATA;


char *send_buffer;
char *recv_buffer;

void make_break(int milisec)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milisec));
}


bool is_buffer_new()
{
    return buffer_info == buffer_info_e::NEW_DATA;
}

bool is_buffer_old()
{
    return buffer_info == buffer_info_e::OLD_DATA;
}


static int push_count = 0;

void push() {
    while (true) {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            // 拷贝数据
            char buffer[1024];
            memcpy(send_buffer, buffer, 1024);
            buffer_info = buffer_info_e::NEW_DATA;
            cv_.notify_one();
        }
        printf("push task, count=%d\n", ++push_count);
        // do something
        make_break(10);


    }
}

static int count = 0;
void pop() {

    while (true)
    {
        
        {
            std::unique_lock<std::mutex> lock(mutex_);
            cv_.wait(lock, is_buffer_new);
            memcpy(recv_buffer, send_buffer, 1024);
            buffer_info = buffer_info_e::OLD_DATA;
        }
        // do something
        printf("pop task, count=%d\n", ++count);
        make_break(50);
            
    }
}

void run() {
    char a[1024], b[1024];
    send_buffer = a;
    recv_buffer = b;

    std::thread t(push);
    std::thread u(pop);
    t.join();
    u.join();


}



} // namespace stream
    
} // namespace vsomeip





#endif // CON_TEST
