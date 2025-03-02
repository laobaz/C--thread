#include <iostream>
#include <thread>
#include <memory>
#include <functional>

class A
{
public:
    void operator1()
    {
        std::cout << "A::operator1" << std::endl;
    }

private:
    friend void test1(); // 友元函数
    void operator2()
    {
        std::cout << "A::operator2" << std::endl;
    }
};

void foo(int &x) // 引用传递，修改x的值，不是传递x的值
{
    for (int i = 0; i < 100000; i++)
        ;

    x = 10;
}
std::thread t2;

void test()
{
    int x = 9;
    t2 = std::thread(foo, std::ref(x)); // 临时变量会被直接销毁，所以需要用引用传递
    // t2.join();
    // std::cout << x << std::endl;
}
void test1()//友元函数
{
    A a;
    std::thread t1(&A::operator2, &a); // 错误，无法访问私有成员
    t1.join();
    return;
}

int main()
{
    int x = 9; // 临时变量
    // std::ref(x);//转化为引用
    std::thread t(foo, std::ref(x)); // 创建线程
    t.join();                        // 等待线程结束
    std::cout << x << std::endl;

    test();
    t2.join();

    A a;                               // 无法判断释放时间
    std::thread t1(&A::operator1, &a); // 创建operator1线程,传递a的地址
    // std::thread t3(a);//无法判断释放时间
    t1.join(); // 等待线程结束
    // t3.join();

    // std::shared_ptr<A> p(new A);
    std::shared_ptr<A> p = std::make_shared<A>();
    // 不同于new，make_shared是一个函数，可以直接传递参数
    std::thread t4((*p)); // 创建线程

    t4.join(); // 等待线程结束

    test1();//友元函数

    return 0;
}