#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <future>

int func()
{
    int i = 0;
    for (; i < 100; i++)
    {
        std::cout << "thread function" <<i << std::endl;
    }
    return i;
}


void func2(std::promise<int> &promise_result)
{
   promise_result.set_value(100);
}
int main()
{
    std::future<int> future_result = std::async(std::launch::async, func);
    //func的结果会被保存在future_result中,并且会在主线程中返回
    std::cout << "main thread" << std::endl;
    std::cout <<"other thread:\n "<< func() << std::endl;
    //主线程和子线程同时执行
    int result = future_result.get();
    std::cout << "result:" << result << std::endl;


    std::packaged_task<int()> task(func);//创建任务.任务是一个函数
    std::function<int()> task2 = func;//函数对象
    std::thread t1(task2);//创建线程
    std::future<int> result2 = task.get_future();//获取future,用于获取结果
    std::thread t(std::move(task));//这里开始执行任务
    //move 作用:将task的内容移动到t线程中,task不再有内容
    //但是future对象仍然可以获取结果
    t.join();
    std::cout << "result2:" << result2.get() << std::endl;

    // std::launch::async表示异步执行，std::launch::deferred表示延迟执行
    //异步执行指的是在调用get()函数时，子线程和主线程同时执行
    return 0; 

    std::promise <int> promise_result;//创建promise对象,用于保存结果
    std::future<int> future_result2 = promise_result.get_future();//获取future对象
    std::thread t2(func2, std::ref(promise_result));//创建线程
    std::thread t3(func2, std::move(promise_result));//创建线程,
    //转移promise对象，原来的promise对象不再有内容，但是future对象仍然可以获取结果
    t2.join();
    t3.join();
}   
