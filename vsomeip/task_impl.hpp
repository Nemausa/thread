#/**
* @file task_impl.h
*
* @brief This message displayed in Doxygen Files index
*
* @ingroup PackageName
* (note: this needs exactly one @defgroup somewhere)
*
* @date	2021-04-11-00-10
* @author Megrez
* @contact: tappanmorris@outlook.com
*
*/
#ifndef TASK_IMPL
#define TASK_IMPL

#include <vector>
#include <memory>
#include <thread>
#include <atomic>
#include <string.h>
#include <unistd.h>

class task_impl {
public:
    task_impl();
    ~task_impl();
private:
    std::vector<std::thread> workers_;
    bool is_stop_;
    std::atomic<bool> is_copy_;
    std::atomic<bool> is_ready_[4];
    unsigned char *send_;
    unsigned char *recv_;

};

#endif // TASK_IMPL