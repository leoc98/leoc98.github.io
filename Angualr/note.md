# 环境搭建
## npm
npm可以通过nodejs在电脑上进行安装
## angular/cli
angular/cli通过命令

    npm install -g @angular/cli
安装。在默认的国外npm官方源“https://registry.npmjs.org”中获取很慢甚至不成功，可以通过命令

    npm config set registry registry.npm.taobao.org // 旧写法，会自动重定向到新源
    npm config set registry https://registry.npmmirror.com // 新写法
改用淘宝源
## 创建项目
ng是angular的命令，可以用来启动，打包，快捷创建等

    ng new 项目名
在当前文件夹下新建项目，如果该文件夹下已有部分项目，则会对项目进行补全，不会覆盖（但如果存在版本问题，比如官方源中途下载失败后转换淘宝源继续创建，可能会导致版本对不上而报错，此时删除原文件夹再重新创建即可

    npm run start
该命令可以将当前angular项目本地运行，会展示对应的本机端口号，可ctrl加左键直接在默认浏览器中打开观看效果
## 创建组件
    ng generate 修饰器 修饰器名
使用该指令可以快速创建一个对应的修饰器


# 修饰器
Angular有多种修饰器，修饰器需要从@angular/core中引入使用
* @Component [组件](#组件)
* @Directives [指令](#指令)
* @Pipe [管道](#管道)
* @Injectable [注入组件](#注入组件)
...

# 元素绑定
* HTML绑定
{{}}
    * 不可绑定new对象和JSON.stringify()

* 属性绑定
[]
* 事件绑定 
()
    * 事件调用的小括号不能省略
* 指令(可以当成一种属性)绑定
    * \* 可影响DOM的指令（结构型指令） 
    * []不影响DOM的指令（属性型指令）
* 双向
[(ngModel)]
    * 是一个双向绑定的语法糖

# 基本结构
    // 导入需要的模块或修饰器
    import {...} from "@angular/..."

    // 定义修饰器
    @修饰器名({
        // 定义修饰器属性
    })

    export class 类名 implement 接口名 {
        // 类成员和方法的定义
    }


## 构造函数
构造函数需要被声明在类方法区域，可直接通过以下写法进行成员变量的声明和初始化（类似Python和C++的初始化构造结合）

    constructor(private member1: Type1, public member2: Type2)
    // 等效以下写法
    private member1:Type1
    public member2:Type2

    constructor(m1,m2) {
        this.member1 = m1
        this.member2 = m2
    }


# 组件
每一个angular组件遵循Model，View，Actions原则。其中model定义在typeScript文件中，主要是相关数据（成员变量—）；actions也定义在typeScript文件中，主要负责对对应的数据进行操作；view定义在html和css文件中，负责展示实际视图。
## Typescript文件
TypeScript主要负责数据和逻辑，在修饰器内，可通过如下属性对组件进行定义，其中template和style相关的都只能各取其一

    selector // 名字
    template // 行内html
    templateUrl // 导入html文件
    styles // 行内css样式
    styleUrl // 导入单个css文件
    styleUrls // 导入多个css文件
    interpolation // 用自定义的插值符代替"{{"和"}}"

# 指令
指令可以当作一个属性放入一个html元素中，它可以像其他的属性一样接收外界值，并通过内部方法影响元素
## Typescript文件
在修饰器内，可通过如下属性对组件进行定义

    selector // 名字，**需要被[]包裹**
在类定义中，可以通过注入对应依赖来对所在DOM节点进行修改，也可注入监听方法来对事件做出响应

## 预定义指令
### ngIf
ngIf 属于结构型指令，如果

# 管道

# 注入组件

