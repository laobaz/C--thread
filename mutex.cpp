#include<iostream>
#include<thread>
#include<mutex>

std::mutex g_lock;

int g_num = 0;
void thread_function(int n, int number)
{
    for (int i = 0; i < n; i++)
    {
        g_lock.lock();//加锁
        g_num++;
        std::cout <<number <<"thread function"<<g_num<<std::endl;
        g_lock.unlock();//解锁
    }
    return;
}

int main()
{
    std::thread t(thread_function,10,1);
    std::thread t1(thread_function,10,2);
    std::cout << "main thread\n";
    t.join();
    t1.join();
    return 0;
}