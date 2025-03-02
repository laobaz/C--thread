#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <functional>
#include <vector>
#include <atomic>

// std::queue<int> q;
// std::mutex mtx;
// std::condition_variable cv;

std::function<void()> task1 =
    []
{ std::cout << "task1" << std::endl; };

// std::function<void()>
// 是标准库中的通用函数包装器，
// 可以存储任意可调用对象
// （如函数指针、Lambda、类成员函数等）。
// task();//执行任务

class ThreadPool
{
private:
    std::vector<std::thread> pool;           // 线程池
    std::queue<std::function<void()>> tasks; // 任务队列
    std::mutex mtx;                          // 互斥锁
    std::condition_variable cv;              // 通知
    std::atomic<bool> stop;                  // 原子操作，线程安全//是否停止线程池
    // stop.load()//读取
    // stop.store(true)//写入
    // stop.exchange(true)//交换
    // stop.compare_exchange_weak(false,true)//比较交换
    // stop.compare_exchange_strong(false,true)//比较交换

public:

    ThreadPool() = delete;//不允许拷贝
    ThreadPool(const ThreadPool &) = delete;//不允许拷贝
    ThreadPool &operator=(const ThreadPool &) = delete;//不允许拷贝
    ThreadPool(ThreadPool &&) = delete;//不允许移动
    ThreadPool &operator=(ThreadPool &&) = delete;//不允许移动
    ThreadPool(int number_threadpool) : stop(false)
    {
        for (int i = 0; i < number_threadpool; i++)
        {//创建线程池，线程数目
            pool.emplace_back([this] { // 添加线程，用lambda表达式实现
                while (true)//这几个线程会一直运行
                {
                    std::function<void()> task;
                    { // 加锁
                        std::unique_lock<std::mutex> lock(mtx);
                        std::cout<<"creating thread id:"<<std::this_thread::get_id()<<std::endl;
                        cv.wait(lock, [this]
                                { return stop.load() || !tasks.empty(); });
                                //等待，直到线程池停止或者任务队列不为空
                                //期间会解锁，等待唤醒
                        if (stop.load() && tasks.empty())
                        { // 线程池停止并且任务队列为空
                            return;
                        }
                        std::cout<<"using thread id:"<<std::this_thread::get_id()<<std::endl;
                        task = tasks.front(); // 取出任务
                        //std::cout<<"task"<<std::endl;
                        tasks.pop();
                    } // 解锁
                    task();
                    // 执行任务，使用完后又会回到上面的while循环，再次等待
                }
            });
        }
    }
    ~ThreadPool()
    {
        stop.store(true);//停止线程池
        cv.notify_all();//唤醒所有线程
        for (std::thread &t : pool)//执行所有线程//结果线程池停止
        {
            t.join();
        }
    }
    void append(std::function<void()>task)
    {
        std::unique_lock<std::mutex> lock(mtx);//共享变量，需要加锁
        tasks.push(task);
        cv.notify_one();//唤醒一个线程,没有需要唤醒的线程就不唤醒
    }
    
};
int main()
{
    ThreadPool pool(4);//创建线程池,有四个线程
    std::this_thread::sleep_for(std::chrono::seconds(5));//睡眠5秒
    for (int i = 0; i < 8; i++)
    {
        pool.append(task1);
        std::this_thread::sleep_for(std::chrono::seconds(1));//睡眠一秒
    }
    return 0;
}