#include <iostream>
#include <thread>

void thread_function(int n)
{
    for (int i = 0; i < n; i++)
    {
        std::cout << "thread function\n";
    }
    return;//线程结束
}




int main()
{
    std::thread t(thread_function,10);//创建线程
    std::cout << "main thread\n";
    //t.join();//等到线程结束
    //t.detach();//分离线程
    if(t.joinable())//判断线程是否可以被join
    {
        t.join();
    }
    return 0;
}