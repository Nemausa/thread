#include <thread>
#include <future>
#include <chrono>
#include <iostream>

namespace vsomeip
{
    namespace promise
    {
        // 使用时没有此类限制std::promise。Promise可以随时（不仅在函数调用结束时）
        // 显式地为将来设置值。让我们看一个这种行为的例子。经理和机械师
        // 想象一下汽车维修店。顾客想买车。她想说服汽车的发动机正常。车间经理指示
        // 机械师检查发动机。一旦确定引擎正常，经理就可以与客户协商合同。
        // 让我们用程序为这个故事建模。我们有一个代表汽车的课程。

        class Car
        {
        public:
            Car()
                : hood(true), electricity(true),
                  exhaust(true), engine(true)
            {
            }

            void remove_the_hood()
            {
                hood = false;
            }

            void add_the_hood()
            {
                hood = true;
            }

            void disconnect_electricity()
            {
                electricity = false;
            }

            void connect_electricity()
            {
                electricity = true;
            }

            void remove_exhaust_system()
            {
                exhaust = false;
            }

            void add_exhaust_system()
            {
                exhaust = true;
            }

            bool is_engine_ok() const
            {
                return engine;
            }

        private:
            bool hood;

            bool electricity;

            bool exhaust;

            bool engine;
        };

        void make_break(int millisec)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(millisec));
        }
        // 将Car有四个数据成员：hood， electricity，exhaust和 engine。它们都由布尔
        // 变量表示。如果某个数据成员true不是万事大吉，则说明存在某些错误（该部分已
        // 损坏或丢失）。
        // 此外，还有一些数据功能可以删除 
        // 添加汽车的某些部分。在.is_engine_ok() 告诉我们，发动机是否正常。
        // 下一部分描述了机械师的行为。
         void mechanic(Car &car, std::promise<bool> engine_ok)
        {
            car.remove_the_hood();
            car.disconnect_electricity();
            car.remove_exhaust_system();

            std::cout << "Mechanic: took a car apart." << std::endl;

            engine_ok.set_value(car.is_engine_ok());

            std::cout << "Mechanic: engine is ok." << std::endl;

            if (car.is_engine_ok())
            {
                make_break(10);
                car.add_exhaust_system();
                car.connect_electricity();
                car.add_the_hood();

                std::cout << "Mechanic: put car back together." << std::endl;
            }
        }

        // 机械师首先拆下发动机罩，电力和排气系统，以便检修发动机。然后他通过承诺将
        // 发动机的状态传达给经理。如果发动机正常，则技工将汽车放回原处。
        // 关键是，promise在功能的中间传达引擎的状态。使用std::async或不可能这样做
        // std::packaged_task，因为它们仅在函数调用结束时传递值。

        // 最后一部分介绍了经理
        void manager()
        {
            Car car;

            std::cout << "Manager: I would need to know if the car engine is ok."
                      << std::endl;

            std::promise<bool> promise;
            std::future<bool> answer = promise.get_future();

            // 经理想知道汽车发动机是否正常。因此，他指示机械师检查发动机。
            // 这是通过创建代表该机制的新线程来完成的
            std::thread thread(mechanic, std::ref(car), std::move(promise));
            thread.detach();

            if (answer.get())
            {
                std::cout << "Manager: ensures the client that the engine is ok.\n"
                          << "Manager: negotiates the contract for selling the car."
                          << std::endl;
            }
            else
            {
                std::cout << "Manager: buys a new engine." << std::endl;
            }

            make_break(10);
        }

        void run()
        {
            manager();
        }
    }
    
}