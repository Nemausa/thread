#include "task_impl.hpp"

#define CAMERA_SIZE 777600

bool controller[4] = {false, false, false, false};
unsigned char source[CAMERA_SIZE] = {1};

int read_count = 0;
task_impl::task_impl() 
    :  is_copy_(false), is_stop_(false) {
    send_ = new unsigned char[4*CAMERA_SIZE];
    recv_ = new unsigned char[4*CAMERA_SIZE];
       
    for (size_t i=0; i < 4; i++) {
        printf("workers=%d\n", i);
        workers_.emplace_back(
            [this, i] {
                for (;;) {

                    if (is_stop_)
                        return;
                    if (is_copy_)
                        continue;
                    is_ready_[i] = false;
                    printf("%d memcpy begin\n", i);
                    memcpy(send_+i*CAMERA_SIZE, source, CAMERA_SIZE);
                    printf("%d memcpy end\n", i);
                    is_ready_[i] = true;
                    controller[i] = true;
                    usleep(1000*33);
                }
            }
        );
    }

    
    // for (std::thread &worker : workers_)
    //     worker.join();

    workers_.emplace_back([this]() {
        printf("task_impl::read()\n");
        int wait = 0;
        while (!(controller[0] && controller[1] && controller[2] && controller[3]))
        {
            printf("wiat:%d\n", wait++);
            usleep(1000);
        }
        printf("wiat close:%d\n", wait++);
        
        for(;;) {
            if (is_stop_)
                return;
            is_copy_ = true;
            int i=0;
            while (!(is_ready_[0] && is_ready_[1] && is_ready_[2] && is_ready_[3]))
            {
                printf("%s i=%d\n", "while", i++);
                usleep(1000);
            }
            memcpy(recv_, send_, 4*CAMERA_SIZE);
            is_copy_ = false;

            printf("%s count=%d\n", __FUNCTION__, read_count++);
            usleep(1000*80);
        }
    });
}





task_impl::~task_impl() {
    is_stop_ = true;
    for (std::thread &worker: workers_)
        worker.join();
}




