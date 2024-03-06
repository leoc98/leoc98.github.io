# Nodejs的组成
* import 依赖模块导入
* 创建服务器
* 监听/处理事件

# 模块
nodejs中每个文件都是一个模块（js，JSON，C/C++编译的扩展文件）

    var a = require("module_path") // 引入模块
    exports.导出对象名 = ... // 导出单个对象
    module.exports = functionName // 打包导出对象
    // 注意其中的变量要加this才能在外部被使用
    // 注意这样导出的是一个函数（类？），需要new实例化后才能使用成员变量和方法

# 事件循环机制
Nodejs是单进程单线程模型，它会循环提取队列中的任务进行处理。为了避免阻塞，任何需要阻塞的操作都会提供异步接口，在进入阻塞状态后直接退出等待继续执行，直到阻塞结束，回调事件被放入队列，当线程重新提取到该回调时继续执行。
## 事件驱动
Nodejs通过观察者模式来进行异步的操作。

### 事件管理
    var evter = events.EventEmitter()
    evter.on("eventName", callback) // 绑定事件与回调
    evter.addListener("eventName", callback) // 绑定事件与回调(等同on)
    evter.once("eventName", callback) // 绑定单次事件与回调
    evter.emit("eventName", args...) // 产生事件
    evter.removeListener("eventName", listener) // 移除监听事件
    evter.listenters("eventName") // 返回监听事件列表
### error事件
error是一个特殊事件，如果该事件触发而不存在响应的监听器时，node会认为是个异常直接退出

### 事件coding
原则上不需要手动调用events.EventEmitter的函数，而是继承它。因为事件的产生者和处理者应当相同（？存疑）

# 流
## 类型
* 只读
* 只写
* 读写
* 转换（数据处理）
## 常用事件（不需要自己定义）
* data 对应write函数
* end 没有更多的数据可读时触发， 对应end函数
* error
* finish 所有数据已被写入到底层系统时触发

## 管道流
    readerStream.pipe(writerSteram)
    readerStream
        .pipe(secondStream)
        .pipe(firstSream);

# 路由
使用url.parse解析路径
* pathname 获取路径
* query获取参数
* 可通过第二个参数将query获得的参数变为对象形式，可用[]提取
使用querystring.parse解析参数
* 用[]提取



# 缓冲区
因为js本身不支持二进制数据，所以提供了Buffer类来进行数据转换

    // 创建
    buf = Buffer.alloc(size,[,fill [,encoding]]) // 默认fill 0，类似new
    buf = Buffer.allocUnsafe(size) // 返回的buffer未进行预处理，类似malloc
    buf = Buffer.from(array/string/buffer [,encoding]) // 返回转换为二进制数据对应的buffer实例

    // 写入
    len = buf.write(string[, offset[, length]][, encoding])

    // 读取
    s = buf.toString([encoding[, start[, end]]]) // 默认为utf-8，读取整个buffer
    json_var = buf.toJSON()

    // 合并
    buf3 = Buffer.concat([buffer_list] [, totalLength])

    // 比较
    bool = buf1.compare(buf2)

    // 拷贝
    buf.copy(targetBuffer[, targetStart[, sourceStart[, sourceEnd]]])
    buf2 = buf1.slice([start[, end]])

# Express
Express是一个简洁而灵活的node.js Web应用框架
* 可以设置中间件来响应 HTTP 请求
* 定义了路由表用于执行不同的 HTTP 请求动作
* 可以通过向模板传递参数来动态渲染 HTML 页面

>var app =  require('express')() // 创建app

## 中间件
    var app =  require('express')()
    app.use(中间件1)
    app.use(中间件2)
    ...
    // 中间件定义
    (req, res, next) => {
        // 处理req和res
        // 调用next()进行下一层中间件调用
    }

    app.use('/public', express.static('public')) // 设置静态文件
需要注意中间件的**执行顺序按照声明顺序**
## 路由
    app.get("path", (req,res)=>{}) // 处理get请求
    app.post("path", (req,res)=>{})

post方法不会自动处理body（为了防止恶意攻击），可以通过express.json()中间件来解析到req.body

# 多进程
    child_process.exec("node child.js" + args, callback)
    child_process.spawn("node", ["child.js", args])
    child_process.fork("child.js", [args])
以上三个方法都能创建子进程。其中fork方法会在父进程和子进程之间创建管道，父进程可以收到子进程的事件，其callback也只接收一个退出码code参数。其他两种方式都
## 子进程事件
父进程可以通过on方法去监听子进程发生的事件已进行异步响应
### exit
exit事件代表子进程已退出
### stop
stop事件代表子进程的标准输出流被关闭

正常会在exit后触发（**但这不一定**）

# 打包工具
JxCore

# 连接mysql数据库
    var mysql      = require('mysql');
    var connection = mysql.createConnection({
    host     : 'localhost',
    user     : 'root',
    password : '123456',
    database : 'test'
    });
    
    connection.connect();
    
    connection.query(
        'SELECT 1 + 1 AS solution', 
        function (error, results, fields) {
            if (error) throw error;
            console.log('The solution is: ', results[0].solution);
        }
    );
