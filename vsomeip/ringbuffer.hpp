/**
* @file ringbuffer.hpp
*
* @brief This message displayed in Doxygen Files index
*
* @ingroup PackageName
* (note: this needs exactly one @defgroup somewhere)
*
* @date	2021-03-31-17-20
* @author Nemausa
* @contact: tappanmorris@outlook.com
*
*/
#ifndef RINGBUFFER
#define RINGBUFFER

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
 
using namespace std;

namespace vsomeip {
    namespace ringbuffer {
    #define RING_BUFFER_SIZE 12 /*Number of circular queue elements*/
    
    /*Circular queue*/
    class RingBuffer{
    public:
        RingBuffer():head(0),rear(0){}
        void Encode(int value){
            cout<<"head="<<head<<" rear="<<rear<<" Encode value:"<<value<<endl;
            data[rear] = value;
            rear = (rear + 1)%RING_BUFFER_SIZE;
        }
        
            /*The focus of the circular queue is the following formulas, add, delete, judge empty, judge full, and count the number. With these formulas, the problem is actually simple*/
        int Decode(){
            int value = data[head];
            head = (head + 1)%RING_BUFFER_SIZE;
            cout<<"head="<<head<<" rear="<<rear<<" Encode value:"<<value<<endl;
            return value;
        }
        
        bool Empty(){
            return (head == rear)?true:false;
        }
        
        bool Full(){
            return ((rear+1)%RING_BUFFER_SIZE == head)?true:false;
        }
        
        int DataNum(){
            return (rear - head + RING_BUFFER_SIZE)%RING_BUFFER_SIZE;
        }
        
        void show(){
            cout<<"num = "<<DataNum()<<endl;
        }
        
    private:
        int head;
        int rear;
        int data[RING_BUFFER_SIZE];
    };
        
    
    void EncodeThread(RingBuffer *rb, int num){
        for(int i = 0 ; i < num; i++){
            rb->Encode(i);
        }
    }
    
    void DecodeThread(RingBuffer *rb, int num){
        for(int i = 0 ; i < num; i++){
            rb->Decode();
        }
    }
    
    
    
    int run(){
        RingBuffer ringBuffer;
        std::vector<std::thread> threads;
        threads.push_back(std::thread(EncodeThread,&ringBuffer,13));
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        cout<<"show ringBuffer size:"<<ringBuffer.DataNum()<<endl;
        threads.push_back(std::thread(DecodeThread,&ringBuffer,5));
        cout<<"ringBuffer is empty:"<<ringBuffer.Empty()<<endl;
        cout<<"show ringBuffer size:"<<ringBuffer.DataNum()<<endl;
        threads.push_back(std::thread(DecodeThread,&ringBuffer,8));
        cout<<"ringBuffer is empty:"<<ringBuffer.Empty()<<endl;
        cout<<"show ringBuffer size:"<<ringBuffer.DataNum()<<endl;
        
        for(auto &t:threads){
            t.join();
        }
        
    }
    }
}
 


#endif // RINGBUFFER