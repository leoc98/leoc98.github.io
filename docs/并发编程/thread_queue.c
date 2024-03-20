#include <queue>
#include <mutex>
#include <condition_variable>
#include <shared_ptr>

template<typename T>
class threadsafe_queue
{
private:
    std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;
public:
    void push(T new_value)
    {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(new_value);
        data_cond.notify_one();
    }
    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        value=data_queue.front();
        data_queue.pop();
    }
    // 智能指针版
    // std::shared_ptr<T> try_pop() 
    // {
    //     std::unique_lock<std::mutex> lk(mut);
    //     data_cond.wait(lk, [this]{return data_queue.empty();});
    //     auto res = std::make_shared(data_queue.top());

    //     data_queue.pop();
    //     return res;
    // }
};