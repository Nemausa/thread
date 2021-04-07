#/**
* @file TASK_IMPL.hpp
*
* @brief This message displayed in Doxygen Files index
*
* @ingroup PackageName
* (note: this needs exactly one @defgroup somewhere)
*
* @date	2021-04-07-09-15
* @author Megrez
* @contact: tappanmorris@outlook.com
*
*/
#ifndef TASK_IMPL
#define TASK_IMPL

#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <stdexcept>

#define CAMER_SIZE 1024

class task_impl {
public:
    task_impl();
    void push();
    void worker_thread();
    void stop(bool is_stop);
    bool stop();
    ~task_impl();
private:
    std::thread worker_;
    std::mutex mutex_;
    std::condition_variable condition_;
    bool is_stop_;
    bool is_refresh_;
    char* send_;
    char* recv_;
};



inline task_impl::task_impl()
    :  is_stop_(false), is_refresh_(false) {
    send_ = (char *)malloc(CAMER_SIZE*sizeof(char));
    recv_ = (char *)malloc(CAMER_SIZE*sizeof(char));
    this->worker_ = std::thread(&task_impl::worker_thread, this);
}


void task_impl::stop(bool is_stop) {
    is_stop_ = is_stop;
}

bool task_impl::stop() {
    return is_stop_;
}

static int count = 0;
void task_impl::worker_thread() {
    for (;;) {
        {
            std::unique_lock<std::mutex> lock(this->mutex_);
            this->condition_.wait(lock,
                                [this] { return this->is_stop_ || this->is_refresh_; });
            if (this->is_stop_ && !this->is_refresh_)
                return;
            // task do something
            memcpy(recv_, send_, CAMER_SIZE);
            this->is_refresh_ = false;
        }
        //printf("pop task, count=%d\n", ++count);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
    }
}



static int push_count = 0;
void task_impl::push() {
  
       
        for (int i=0; i<1000; i++) {
            if(i==200) 
            {
                is_stop_ = true;
                return;
            }

            {
                std::unique_lock<std::mutex> lock(mutex_);
                // 拷贝数据
                char buffer[CAMER_SIZE] = {0};
                memcpy(send_, buffer, CAMER_SIZE);
                this->is_refresh_ = true;
                this->condition_.notify_one();
                
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
           // printf("push task, count=%d\n", ++push_count);

        }
   
    
}

// the destructor joins all threads
inline task_impl::~task_impl() {
    {
        std::unique_lock<std::mutex> lock(mutex_);
        is_stop_ = true;
    }
    condition_.notify_all();
    worker_.join();
    free(send_);
    free(recv_);
    printf("task_impl::~task_impl()");
}


#endif // TASK_IMPL