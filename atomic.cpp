#include<iostream>
#include<thread>
#include<mutex>
#include<chrono>
#include<condition_variable>
#include<queue>
#include<functional>
#include<vector>
#include<atomic>


std::atomic<int> shared_variable = 0;

//atomic是原子操作，线程安全
//atomic<int> a(0);
//a.load()//读取
//a.store(1)//写入
//a.exchange(2)//交换
std::mutex mtx;
void func(){
    for(int i=0;i<100000;i++)
    {
        std::unique_lock<std::mutex> lock(mtx);

        //std::cout<<"thread function"<<std::endl;
    }
}

void func2(){
    for(int i=0;i<100000;i++)
    {
        //std::unique_lock<std::mutex> lock(mtx);
        shared_variable++;
    }
}

int main()
{
    auto last=std::chrono::duration_cast<std::chrono::microseconds>
    (std::chrono::system_clock::now().time_since_epoch());
    std::thread t1(func); 
    t1.join();
    auto now=std::chrono::duration_cast<std::chrono::microseconds>
    (std::chrono::system_clock::now().time_since_epoch());
    std::cout<<"time1:"<<now.count()-last.count()<<std::endl;

    last=std::chrono::duration_cast<std::chrono::microseconds>
    (std::chrono::system_clock::now().time_since_epoch());
    std::thread t2(func2);
    t2.join();
    now=std::chrono::duration_cast<std::chrono::microseconds>
    (std::chrono::system_clock::now().time_since_epoch());  
    std::cout<<"time2:"<<now.count()-last.count()<<std::endl;
    return 0;
}