#include<iostream>
#include<thread>
#include<mutex>
#include<string>

class Log;

static Log* m_instance=nullptr;//静态变量，只初始化一次，全局只有这一个log
std::mutex m_mutex;
static std::once_flag m_flag;//once_flag 有一个标志位，保证只初始化一次
class Log //全局只有一个，需要直接获取实例，不需要拷贝
{
    public:
    Log (){};
    ~Log(){};
    Log(const Log&)=delete;//不允许拷贝
    Log& operator=(const Log&)=delete;//不允许拷贝


    static Log* get_instance()//懒汉模式
    {
        
        //使用call_once,保证只初始化一次
        //Log* tmp=m_instance;
        std::call_once(m_flag,init);//保证多线程，只初始化一次
        //只能在函数内部调用，不能在外部调用
        return m_instance;
    }
    static void init()
    {
        if(!m_instance)
        {
            m_instance=new Log();
        }
    }
    void write_log(const std::string& log)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::cout<<log<<std::endl;
    }
    static Log * get_instance2()//饿汉模式
    {
        return m_instance;
    }
    //static  Log *m_flag;
};

int main   ()
{
    // Log* log=Log::get_instance();
    // log->write_log("hello");
    Log::get_instance()->write_log("world");//静态函数，直接调用，不需要实例化
    return 0;


}