#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <condition_variable>
#include <queue>
#include<chrono>

std::queue<int> q;
std::mutex mtx;
std::condition_variable cv;

void Producer()
{
    int i = 0;
    while (i < 100)
    {
        std::unique_lock<std::mutex> lock(mtx,std::defer_lock); // 自动加锁和解锁
        lock.lock();
        q.push(i);
        lock.unlock();
        // 队列不为空，通知消费者
        std::cout << "push" << i << std::endl;
        i++;
        cv.notify_one(); // 通知一个线程
        std::this_thread::sleep_for(std::chrono::seconds(1)); // 睡眠1s
    }
    return;
}

void Consumer()
{
    int i = 0;
    while (i < 100)
    {
        std::unique_lock<std::mutex> lock(mtx); 
        cv.wait(lock, []                 
                { return !q.empty(); }); // 等待，直到队列不为空
        int data = q.front();
        // 队列为空，等待
        q.pop();
        std::cout << "pop" << data << std::endl;
        i++;
    }
    return;
}

int main()
{
    std::thread t1(Producer);
    std::thread t2(Consumer);
    std::thread t3(Consumer);
    t1.join();
    t2.join();
    t3.join();
    return 0;
}