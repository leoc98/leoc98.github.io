# 视频来源
https://www.bilibili.com/video/BV1FP411x73X/?spm_id_from=333.999.0.0&vd_source=854a4ed7439fe761acc3938801db9a47
[仿函数注意](#handler是仿函数的情形)
# 语法
handler 可以是函数,可以是lambda表达式,可以是仿函数(见[仿函数注意](#handler是仿函数的情形))

    // 启动线程
    std::tread t1(handler, arg)
    // 等待线程
    t1.join()
    
# 不join线程崩溃
因为线程内部的terminate,里面有assert断言会导致崩溃.

在thread的移动构造中进行了判断,如果本线程是joinable的(即所有权仍被持有),就会抛出异常

# handler是仿函数的情形
join会崩溃,因为handler会把仿函数解释为函数指针,可以通过以下方式解决
    std::tread t2((handler()));
    std::tread t3{handler()};

# detach隐患
如果存在子线程引用父线程的局部变量,父线程使用detach时会导致局部变量被回收,从而导致访问出错
## 解决方案
使用一个智能指针,保证所有引用都析构后再被释放
## 分析
如果在子线程构造函数中进行了隐式类型转变,该变量不会立刻被使用,而是到线程启动时才使用,而在那时可能局部变量已经被释放了

# 主线程崩溃
有些时候子线程会做一些很重要的事(比如写数据库或重要日志),此时需要去捕获异常去join子线程,防止主线程资源被回收导致子线程未完成.
## 自动处理
使用一个保护 类持有该线程,当该保护类析构时,join子线程(需要判断是否joinable)

# 引用传递
引用直接通过arg传递会因为内部的完美转发变成右值,使用std::ref封装
## std::ref封装实现
内部通过元编程记录的传入的类型和引用地址,随后重载其类型转换运算符(原对象引用转换)和get方法,使其返回通过引用对象的地址解引用而获取的引用对象

# 线程归属权
类似unique_ptr,线程也没有拷贝构造函数,需要使用std::move或返回值进行转移所有权.如果对一个已经绑定线程的变量进行线程赋值(move),如果前一个线程还没结束,可能会有terminate风险.

# 互斥机制
## 手动机制
    std::mutex m1;
    m1.lock();
    m1.unlock();

    


## 自动机制
**注意,锁代管后就不要使用原锁进行操作了,它会失去他的解锁操作,调用会崩溃**
    {
        std::lock_guard<std::mutex> lk_guard(m1);
        // adopt_lock可以省略加锁操作,防止重复加锁
        std::lock_guard<std::mutex> lk_guard(m1, std::adopt_lock);

        // c++17, 同时加两个锁且省略加锁操作
        std::scoped_lock guard(lock1, lock2); 
    }
    // 退出作用域后自动解锁

    // 生成时自动加锁,可手动或自动解锁(离开作用域)的锁
    std::unique_lock ul(m1);
    // 生成时不加锁
    std::unique_lock ul(m1, std::defer_lock);
    std::unique_lock ul(m1, std::adopt_lock);

## 读写锁
    // c++17
    // 读
    std::shared_mutex<std::shared_mutex>
    // 写
    std::unqiue_lock<std::shared_mutex>
    // 或者
    std::guard_locks<std::shared_mutex>

## 递归锁
    std::recursive_mutex;

# 死锁
## 同时加锁
    // 同时加两个锁
    std::lock(lock1, lock2);
## 层级锁
在继承类中设计层级,并有一个静态的线程层级(tread_local变量),加锁时只能加比当前线程层级高的锁并同时更新层级,否则异常;解锁时还原层级

这种方法能在运行时直接暴露潜在死锁

# stack先top后pop后return的复制问题
* 用shared_ptr来封装stack元素
* 使用引用参数直接获取

# 单例模式
## C++11 后
因为C++11后保证了静态局部对象的线程安全,所以可以直接使用普通的单例写法(即初始化不使用锁)

## C++11 前
* 饿汉式:由主线程同一初始化后使用
    * 不一定会被遵守
    * 浪费资源
    * 拖慢启动时间
* 懒汉式:使用时初始化
    * 获取时不需要加锁
    * 初始化时需要加锁
    * 获取锁后要重复判断单例是否存在,防止两个同时初始化的线程初始化两次
        * 双重锁仍然不能保证,因为编译器优化可能导致静态单例对象先被赋值再被初始化,而后续的getInstance可能会拿到没初始化的非空对象
            * 使用call_once
    * 使用shared_ptr防止资源泄露
        * 指定析构函数(仿函数友元类)
        * 私有析构函数来保证不会被获取到裸指针析构

# 条件变量
    std::condition_variable cv;
    // 尝试加锁,需要while去判断
    while(condition) {
        cv.wait(m1);
    }
    // 使用第二个参数谓词, 等价上面
    cv.wait(m1, [](){return condition});

    // 唤醒wait的线程
    cv.notify_one();
    cv.notify_all();

# 异步关键字


## std::future<T>
一个泛型类型,用于获取异步函数的返回值.

**future类型在析构时会等待对应任务的完成,需要当心此处生成死锁**(如和lock_guard一起使用时,如果lock_guard早于future构造,则在析构时要求后析构lock_guard(即后释放锁),但如果future中需要持有锁,future需要等待lock_guard析构,但future需要先析构才能使lock_guard析构,此时产生循环等待)

    // 阻塞获取结果
    // wait 不会直接获得结果,但其返回时,代表结果已经可用,且该操作不消耗future
    futureRes.wait();
    // 直接使用get会消耗future,并获取结果
    futureRes.get();

## std::shared_future<T>
可以被共享,且可从std::future直接转换

使用时不应当使用移动语义,会导致shared_future被废弃

## std::async
async 是一个异步函数模板,类似js的async实现,它所持有的函数将会被异步执行.即调用async后线程不会立刻阻塞,而是在对目标结果进行获取时(future::get)才会被阻塞.

需要注意的是,async函数的返回值是个future泛型,但其中的异步函数对此无感知,返回原本的类型.

    // 异步函数立刻启动
    std::future<returnType> futureRes = std::async(std::launch::async, std::function<returnType(argType)>, args...);
    // 异步函数在get/wait时刻再启动
    std::future<returnType> futureRes = std::async(std::launch::deferred, std::function<returnType(argType)>, args...);
    // 异步函数可能在任何时候启动(看实现,也是默认方式)
    std::future<returnType> futureRes = std::async(std::launch::async | std::launch::deferred, std::function<returnType(argType)>, args...);

    // 这部分的代码不会阻塞

    // 对future操作阻塞获取结果

## std::promise
promise是一个封装对象,是连接异步生产者消费者的桥梁. 它不能被拷贝,需要使用移动语义进行传递.它的一个优势在于,生产者可能在生产完对应future后不会立刻返回或结束,但消费者可以在生产者生产完后立刻获取到对应返回.

    std::promise<returnType> prom;

    // 获取未来的返回值future
    std::future<returnType> fut = prom.get_future();

    // 其他线程
    prom.set_value(...);
    /* ...其他无关操作 */

    // 主线程在之后set_value可获得值
    fut.get();

### 异常
在promise中可以设置异常,主线程需要捕获该异常防止崩溃

# 原子类型
    atmoic<>
    // c++17 是否硬件上支持无锁形式
    std::atomic::is_always_lock_free
## 内存模型
* 全局一致
    * std::memory_order_seq_cst
    * 如同加了锁
    * 写操作会同步的到内存从而同步到其他CPU
* 同步模型(不能保证全局顺序一致)
    * std::memory_order_acquire
    * std::memory_order_release
    * memory_order_acq_rel
* 宽松顺序
    * std::memory_order_relaxed
    * 保证操作原子性
    * 影响不一定立刻可见
    * 不能保证修改顺序
## 内存序列

# 无锁并发
## 无锁队列
使用compare_exchange来获取编辑位置
* std::atomic<T>::compare_exchange_weak(T &exp, T desired)
* std::atomic<T>::compare_exchange_strong(T &exp, T desired)

### 并发思考点
* 当两个数据同时写入时,如果先对目标写入再对尾指针进行更新,会导致后修改的数据覆盖先修改的数据,然后先修改的数据先成功修改尾指针退出,从而导致先写数据丢失
    * 先竞争写入位置,竞争到后再对目标位置写入
* 空队列写入后直接读取时,由于先(竞争并)修改了写入位置,再写入,可能导致其他线程读取时发现存在数据(尾指针变动)但读出旧数据(写入未完成)
    * 引入update原子量来保证读取到写入后的数据
* 单个update原子量可能导致后续写入被前序block的问题?


# 并行设计模式
## Actor
点对点发送msg

## CSP
像队列中投递和从队列中取出,发送者和接收者不关心对方是谁

# 返回值
* 如果有移动构造优先返回移动构造
