#include<iostream>
#include<thread>
#include<mutex>

int shared_variable = 0;
std::mutex mtx;//不支持时间锁
std::timed_mutex tmtx;//支持时间锁

void func(int n)
{
    for (int i = 0; i < n; i++)
    {
        // mtx.lock();
        // shared_variable++;
        // std::cout << "thread function" << shared_variable << std::endl;
        // mtx.unlock();
        std::lock_guard<std::mutex> lock(mtx);
        //不能直接复制mutex对象，因为mutex对象是不可复制的

        //自动加锁和解锁
        shared_variable++;
        std::cout << "thread function" << shared_variable << std::endl;

    }
    return;
}


void func2(int n)
{
    for (int i = 0; i < n; i++)
    {
        // std::lock_guard<std::mutex> lock(mtx);
        // shared_variable++;
        // std::cout << "thread function" << shared_variable << std::endl;
         

        std::unique_lock<std::timed_mutex> lock(tmtx,std::defer_lock);
        //unique_lock会在{ }内部加锁，出了{ }自动解锁
        //defer_lock不会在构造函数中加锁
        
        //支持swap

        //lock.lock();
       
        lock.try_lock();//尝试加锁
         //可以延时加锁
        if( lock.try_lock_for(std::chrono::seconds(2)))//尝试加锁等待2秒,超时不等返回
        {
            std::cout<<"lock success"<<std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));//等待五秒
            std::cout<<"unlock"<<std::endl;
            shared_variable++;
        }
        
         std::cout << "thread function" << shared_variable << std::endl;
        //lock.unlock(); 


    }
    return;
}