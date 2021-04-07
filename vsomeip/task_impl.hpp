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

#define CAMERA_SIZE 1024

class task_impl {
public:
    task_impl();
    ~task_impl();
    void stop(bool is_stop);
    bool stop();
    void refresh(bool is_fresh);
    void push();
    void worker_thread();
    void push_thread();
    
   
private:
    std::thread worker_;
    std::thread push_;
    std::mutex mutex_;
    std::condition_variable condition_;
    bool is_stop_;
    bool is_refresh_;
    char* send_;
    char* recv_;
};



inline task_impl::task_impl()
    :  is_stop_(false), is_refresh_(false) {
    send_ = (char *)malloc(CAMERA_SIZE*sizeof(char));
    recv_ = (char *)malloc(CAMERA_SIZE*sizeof(char));
    this->worker_ = std::thread(&task_impl::worker_thread, this);
    this->push_ = std::thread(&task_impl::push_thread, this);

}


void task_impl::stop(bool is_stop) {
    is_stop_ = is_stop;
}

bool task_impl::stop() {
    return is_stop_;
}

void task_impl::refresh(bool is_refresh) {
    is_refresh_ = is_refresh;
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
            memcpy(recv_, send_, CAMERA_SIZE);
            this->is_refresh_ = false;
            printf("pop task, count=%d\n", ++count);
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

    }
}



static int push_count = 0;
void task_impl::push_thread() {
       
        for (;;) {
            {
                std::unique_lock<std::mutex> lock(mutex_);
                // 拷贝数据
                char buffer[CAMERA_SIZE] = {0};
                memcpy(send_, buffer, CAMERA_SIZE);
                this->is_refresh_ = true;
                this->condition_.notify_one();
                printf("push task, count=%d\n", ++push_count);
                if (this->is_stop_ )
                    return;
                
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            

            // if(push_count>100) 
            // {
            //     is_stop_ = true;
            //     return;
                
            // }
        }
   
    
}

// the destructor joins all threads
inline task_impl::~task_impl() {
    {
        std::unique_lock<std::mutex> lock(mutex_);
        is_stop_ = true;
    }
    condition_.notify_all();
    if (worker_.joinable())
        worker_.join();
    if (push_.joinable())
        push_.join();
    free(send_);
    free(recv_);
    printf("task_impl::~task_impl\n");
}


#endif // TASK_IMPL