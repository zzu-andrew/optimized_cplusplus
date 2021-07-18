[toc]

#### 何时调用`copy`构造函数

在构造函数调用的时候，有的时候调用默认构造函数，有的时候调用`copy`构造函数，特别是`copy`构造函数的调用让人容易和`copy`赋值的函数产生混淆。

如下对其进行了测试：

```cpp
class WidgetOperator {
public:
    WidgetOperator() = default;;
    ~WidgetOperator() = default;;
    WidgetOperator(const WidgetOperator & wo) {
        std::cout << "call WidgetOperator ctor" << std::endl;
    }

    WidgetOperator& operator=(const WidgetOperator& wo) {
        std::cout << "call WidgetOperator operator= " << std::endl;
        return *this;
    }
};

void WidgetOperatorTest() {
    std::cout << "W1" << std::endl;
    WidgetOperator W1;  // 调用无参构造函数
    std::cout << "W2(W1)" << std::endl;
    WidgetOperator W2(W1); // 调用copy构造函数
    std::cout << "W1 = W2" << std::endl;
    W1 = W2;  // 调用 operator=函数
    std::cout << "WidgetOperator W3 = W1" << std::endl;
    WidgetOperator W3 = W1; // 调用copy构造函数
}
```

执行输出结果：

```bash
W1
W2(W1)
call WidgetOperator ctor
W1 = W2
call WidgetOperator operator= 
W3 = W1
call WidgetOperator ctor
```

通过上述测试的输出可以看出，当调用`=`操作符的时候，如果一个新对象被定义。如：`WidgetOperator W3 = W1;`，一定会有一个构造函数被调用，不可能调用赋值操作，反之，如果没有一个新的对象被定义，就不会有构造函数被调用，而只会调用赋值操作符。



### 视`C++`为一个语言联邦

一开始`C++`只是C加上一些面向对象特性，但是随着这个语言的成熟他变得更加无拘无束，接受不同于`C with classes`的各种观念、特性和编程战略。异常对函数的结构化带来了不同的做法，`templates`将我们带来到新的设计思考方式，`STL`则定义了一个前所未见的伸展性做法。

今天`C++`已经是个多重范型编程语言，一个同时支持过程形式、面向对象形式、函数形式、泛型形式、元编程形式的语言。这些能力和弹性使`C++`成为一个无可匹敌的工具，因此、将`C++`视为一个语言联邦。

### 尽量以`cosnt、enum、inline`替换`#define`

因为、宏定义会被预处理器处理，编译器并未看到宏定义的信息，当出现一个编译错误信息的时候，可能会带来困惑。

解决之道就是使用一个常量替换宏定义(`#define`)

```cpp
const double AspectRatio = 1.653;   // 大写名称通常代表宏定义，因此这里可以使用首字母大写的方法表示const全局变量
```

作为一个语言常量，`AspectRatio`肯定会被编译器看到，当然就会进入符号表内。另外、使用常量也可以有较小的码、因为使用预处理会导致预处理器盲目的将宏名称替换为对应的数值，可能会导致目标码出现多份宏定义的数值。

基于数个理由`enum hack`值得我们认识。

```cpp
class GamePlayer{
  private:
    enum {NumTurns = 5}; // enum hack 令NumTurns成为5的一个标记
    int scores[NumTurns]; //
};
```

- `enum hack`的行为某方面来说比较像`#define`而不像`const`，有的时候这正是你想要的，例如取一个`const`的地址是合法的，但是取一个`enum`的地址就是不合法的，而取一个`#define`的地址通常也不合法。如果你不想让别人获得一个`pointer`或者`reference`指向你的某个整数常量，`enum`可以帮助你实现这个约束。
- 虽然优秀的编译器不会为`const`对象设置存储空间，但是不够优秀的编译器可能会设置另外的储存空间，`enum`和`#define`一样绝对不会导致非必要的内存分配。
- 出于实用主义考虑，很多代码特别是模板元编程中用到了它，因此、看到它你必须认识他。

对于单纯的常量，最好以`const`对象或者`enums`替换`#define`

对于形似函数的宏(`macros`)，最好改用`inline`函数替换`#define`

### 尽可能使用`const`

`const`的一件奇妙的事情是，它允许你指定一个语义约束，而编译器会强制实施这项约束。它允许你告诉拜你一起和其他程序员某值应该保持不变。

```cpp
char greeting[] = "Hello"; 
char *p = greeting;    // non-const pointer, non-const data
const char* p = greeting;  // non-const pointer, const data
char* const p = greeting;  // const pointer non-const data
const char* const p = greeting; // const pointer, const data 
```

`const`语法虽然变化多端，但并不是莫测高深，如果关键字`const`出现在型号的左边，表示被指物是常量，如果出现在星号的右边，表示指针自身是常量，如果出现在星号两边，表示被指物和指针两者都是常量。

如果被指物是常量，有些程序员会将关键字`const`写在类型之前，有些人会把它写在类型之后、星号之前，这两种写法的意义相同，所以下列两个函数的参数类型是一样的：

```cpp
void f(const Widget* pw);   // 一个指向常量的指针
void f2(Widget const* pw); // 一个指向常量的指针
```

两种形式都有人使用，是否是指向常量的指针，要看`const`相对于星号的位置，星号左边为指向常量的指针，星号右边为常量指针。

`const`修饰函数返回值，可以降低编码出现的低级错误

```cpp
class Rational {};
const Rational operator*(const Rational& lhs, const Rational& rhs);
Rational a, b, c;
if (a*b = c) // 其实是想做个比较，当operator*返回值声明为const的时候将会返回错误，也就防止了编码不小心带来的异常
```

`const`修饰成员函数

- 可以通过`const`得知哪些函数可以改动对象内容，哪些函数不可以
- 使得操作`const`对象成为可能

### 确定对象被使用之前已先被初始化

关于将变量初始化这件事，`C++`似乎总是反复无常。但是有一点是可以确定的是，读取没有初始化的值会导致不确定行为

### 了解`C++`默默编写并调用哪些函数

什么时候`empty class`不再是个空类呢？当`C++`处理过之后，是的，如果你没有自己声明，并一起就会为它声明(编译器版本)一个`copy`构造函数、一个`copy assignment`操作符和一个析构函数。

因此、如果你声明了一个`empty class`如下：

```cpp
class Empty{};
```

编译器处理之后就好像你写了如下的代码：

```cpp
class Empty {
public:
    Empty() {}                               // default构造函数
    Empty(const Empty& rhs) {}               // copy构造函数
    ~Empty() {}                              //析枸函数
    Empty& operator=(const Empty& rhs) {}    // copy assignment 操作符
};
```

唯有当这些函数被需要(被调用)，它们才会被编译器创建出来。

好了，我们知道编译器会常见这些函数，但这些函数做了什么？`default`构造函数和析构函数，主要是给编译器一个地方放置藏在幕后的代码，像是调用`base class`和`non-static`成员变量的构造函数和析构函数。需要注意的是编译器默认的析构函数是`non-virtual`的。

### 若不想使用编译器自动生成的函数，就明确拒绝

有时你不想让用户使用某个函数，不对函数进行声明就行了。但是这样做对`copy`构造函数和`copy assignment`操作符却不起作用，因为、如果你不进行声明，编译器会声明一个默认的出来。

这就把你逼到一个困境，如果你不想让用户使用`copy`构造函数和`copy assignment`函数，你既不能不声明也不能进行声明。这个问题的解决方案就是，将函数声明为私有的函数，这样你即可以阻止编译器创建它们，又因为是私有函数，使得别人不能调用。

但是这样做并不是绝对安全的，因为`member`函数和`friend`函数还是可以调用`private`函数的。除非你足够聪明不去定义它们，那么如果任何人不慎调用了任何一个函数，将会导致一个链接错误，将成员函数声明为私有，而又故意不去实现它们是如此的受欢迎。、

```cpp
class HomeForSale {
public:
   ...
private:
    HomeForSale(const HomeForSale&);    // 因为根本没有人能调用，写参数名称也是浪费
    HomeForSale& operator=(const HomeForSale&);
};
```

有了上述的定义之后，当用户企图调用拷贝`HomeForSale`对象的时候，编译器会阻止他，如果不慎在`member`或者`friend`函数中调用，连接器也会发出抱怨。

为了驳回编译器自动提供的功能，可将相应的成员函数声明为`private`并且不予实现。

### 为多态基类声明`virtual`析构函数

如果多条基类没有声明虚析构函数，那么当通过基类指针指向一个子类对象，调用`delete`的时候只会调用基类的析构函数，不会调用子类的，这样就会造成资源部分释放的现象。

如果`class`不含有`virtual`函数，通常表示它并不意图被用作基类：

如一个二维空间点坐标的`class`：

```cpp
class Point {   // 二维空间点(2D point)
public:
	Point(int xCoord, int yCoord);
    ~Point();
private:
    int x, y;
};
```

如果`int`占`32bits`那么`Point`对象可以塞进一个`64-bit`缓存器中。更有甚者，这个类完全可以作为一个`64-bit`量，传递给其他语言，如C，但是当`Point`的析构函数是`virtual`时，形式就会发生变化。

欲实现`virtual`函数，对象必须携带某些信息，主要在运行期间决定哪个`virtual`函数被调用。这类信息通常由一个`vptr`虚函数表指针之处。`vptr`指向一个由函数指针构成的数组，称为`vtbl`；每一个带有虚函数的`class`都有一个相应的`vtbl`。

因此、无端的将所有的`class`的析构函数声明为`virtual`，就像从未声明它们为`virtual`一样，都是错误的。

因为标准容器都是`non-virtual`的，不要试图将其作为`base-class`。

### 别让异常逃离析构函数

`C++`并不禁止析构函数吐出异常，但它不鼓励你这样做。

析枸函数绝对不要吐出任何异常，如果一个被析枸函数调用的函数可能抛出异常，析枸函数应该捕获任何异常，然后吞下让梦或结束程序

如果客户需要对某个操作函数运行期间抛出的异常做出反应，那么`class`应该提供给一个普通函数执行该操作

### 绝对不再构造和析构过程中调用`virtual`函数

你不应该在构造函数和析构函数中调用`virtual`函数，因为这样的调用不会带来你预想的结果。

构造函数调用时，因为`derived classes`没有初始化好，会调用`base class`的虚函数

析构函数调用时，一旦进入析构函数，对象中的`derived classes`对象便呈现出未定义值，所以`C++`视它们仿佛不再存在。

在构造和析构期间不要调用`virtual`函数，因为这类掉用，从不降低`derived class`

### 令`operator=`返回一个`reference to *this`

关于赋值，有趣的是你可以把它们写成连锁的形式：

```cpp
int x, y, z;
x = y = z = 5;
```

同样有趣的是，赋值采用右结合律，所以上述的连锁赋值被解析为：

```cpp
x = (y = (z = 15));
```

为了实现连锁赋值，赋值操作符必须返回一个`reference`指向操作符的左侧实参

```cpp
class Widget {
public:
    Widget& operator=(const Widget*rhs) {
        return *this;
    }
};
```

### 在`operator=`中处理`自我赋值`

自我赋值发生在对象被赋值给自己时：

```cpp
class Widget {};
Widget w;
w = w; // 赋值给自己
```

看起来有点傻，但是它是合法的，所以不要认定客户不会这样做，此外赋值动作并不总是那么可以被一眼辨认出来：

```cpp
a[i] = a[j]; // 潜在的自我赋值
```

一个不安全的`operator=`使用示例：

```cpp
class BitMap {};
class Widget {
    
private:
    BitMap* pb;
}

Widget& Widget::operator=(const Widget& rhs) {
    delete pb;  // 停止使用当前的bitmap
    pb = new BitMap(*rhs.pb); // 使用rhs's bitmap的副本(复件)
    return *this;
}
```

这里的问题是，当`operator=`进行自我赋值的时候，`delete pb`相当于把自己的`pb`给删掉了

为了防止这种错误，传统的做法是进行证同测试，达到自我赋值的检验的目的：

```cpp
Widget& Widget::operator=(const Widget& rhs) {
    if (this == &rhs) return *this;
    delete pb;  // 停止使用当前的bitmap
    pb = new BitMap(*rhs.pb); // 使用rhs's bitmap的副本(复件)
    return *this;
}
```

`swap`版本的：

```cpp
Widget& Widget::operator=(const Widget& rhs) {
    Widget temp(rhs);
    swap(temp);
    return *this;
}
// 或者
Widget& Widget::operator=(const Widget& rhs) {
    swap(rhs);
    return *this;
}
```

- 确保对象自我赋值时，`operator=`有良好的行为，其中技术包括比较来源对象和目标对象的地址、精心周到的语句顺序、以及`copy-and-swap`
- 确定任何函数如果操作一个以上的对象，其中多个对象是同一个对象时，其行为仍然正确。

### 以对象管理资源

许多资源分配后用于单一的区域或者函数内，它们应该在控制流离开那个区块或函数时被释放。标准库`auto_ptr`正是对这种形势而设计的特制产品。`auto_ptr`是个类指针对象，也就是所谓智能指针，其析枸函数自动对其所指向对象调用`delete`

获得资源后立即放进管理对象内，实际上以对象管理资源的观念被称为`资源取得时机便是初始化时机`(`Resource Acquisitioon Is Initialzation; RAIL`)

管理对象利用析枸函数确保资源被释放

- 为防止资源泄露，请使用`RAIL`对象，它们在构造函数中获得资源并在析构函数中释放资源



### 在资源管理类中提供对原始资源的访问

资源管理类很棒，它们是你对抗资源泄露的堡垒。但是这个世界并不是总是那么的完美，许多的`APIs`直接指涉资源，所以除非你发誓用不录用这样的`APIs`，否则就只能绕过资源管理对象直接访问原始资源。

- `APIs`往往要求访问原始资源，所以每一个`RAIL Class`应该提供一个取得其所管理之资源的方法
- 对原始资源的访问可能经由显示转换或隐式转换。一般而言显式转换比隐式转换更加安全，但是隐式转换对客户来说比较方便

### 成对的使用`new`和`delete`时要采取相同的形式

一下动作有什么错？

```cpp
std::string* stringArray = new std::string[100];

delete stringArray;
```

每件事情开起来都是井然有序的，使用了`new`也搭配了对应的`delete`。但还是有样东西完全错误，你的程序行为不明确，`stringArray`所包含的个`string`对象中的99个不太可能被适当删除，因为他们的析构函数很可能没有被调用。

当你使用`new`有两件事情发生，第一，内存被分配出来；第二、针对此内存会有一个(或更多)构造函数被调用。当你调用`delete`也有两件事情发生，针对此内存会有一个(或更多)析构函数被调用，然后内存才被释放。`delete`的最大问题在于：即将删除的内存究竟存在多少对象，这个问题的答案决定了有多少析构函数必须被调用起来。

因此、为了降低不必要的麻烦，不要对数组形式做`typedef`等操作

- 如果你在`new`中使用了`[]`，必须在相应的`delete`表达式中国捏也使用`[]`。如果你在`new`表达式中没有使用`[]`，一定不要在相应的`delete`表达式中使用`[]`。

### 以独立语句将`newed`对象置入智能指针

`RAIL`风格的代码也不是什么地方都能使用，如有以下代码：

```cpp
processWidget(std::tr1::shared_ptr<Widget>(new Widget), priority());
```

虽然这里借助`shared_ptr`实现了对象管理式资源，但是却可能造成资源泄露

如果上述`processWidget`的调用按照如下顺序进行：

1. 调用`new Widget`
2. 调用`tr1::shared_ptr`的构造函数
3. 调用`priority`函数

按照上述过程调用是没有问题的，但是`C++`编译器会以什么样的次序完成这些事情呢？答案是不一定。这正是`C++`区别`java`和`C#`的不同，那两种语言总是以特定的次序完成函数参数的核算。

如果`C++`编译器按照一下的顺序执行：

1. 调用`new Widget`
2. 调用`priority`函数
3. 调用`tr1::shared_ptr`的构造函数

现在你想象下，如果`priority`执行出现异常，会发生什么事情？在这种情况下`new Widget`返回的指针将会遗失，从而造成资源泄露。上述的复合语句正是造成这种资源泄露的元凶。

如果想解决这种问题，可以通过将复合语句拆分进行解决

```cpp
std::tr1::shared_ptr<Widget> pw(new Widget);   // 在单独语句内以只能指针存储 newed出来的对象
processWidget(pw, priority()); // 这个调用即使出现异常也不会造成资源泄露
```

- 一独立语句将`newed`对象存储于(置于)智能指针内。如果不这样做，一旦异常被抛出，有可能导致难以察觉的资源泄露。

### 将成员变量声明为`private`

如果成员变量不是`public`，客户唯一能够访问对象的办法就是通过成员函数。如果`public`内都是成员函数，那么客户也就不必花费时间纠结调用成员的时候是否需要加`()`。

使用函数可以让你对成员变量实现更精确的控制。如果你令成员变量为`public`那么每个人都可以方位它，而通过函数就可以实现不准访问、者只读访问、只写访问和读写访问。

如果你通过函数访问成员变量，日后可以更改某个计算替换这个成员变量，而`class`客户一点也不会知道`class`的内部实现已经起了变化。

因此、一旦你将一个成员变量声明为一个`public`或者`protect`并且客户开始使用，那么这个成员变量的去除将会影响所有调用它的地方，所有相关的代码文档测试接口都将进行重写。

- 切记将成员变量声明为`private`。这可赋予客户访问数据的一致性、可细微划分访问控制、允许约束条件获得保证，并提供`class`作者以充分的实现弹性。
- `protect`并不比`public`更具封装性。

### 尽可能延后变量定义式的出现时间

只要你定义了一个变量而其类型带有一个构造函数或者析构函数，那么程序的控制流到达这个变量定义式时，你就得承受构造成本，当这个变量离开其作用域时，你便得承受析构成本，即使这个变量最终并未被使用，仍需耗费这些成本，所以你应该尽可能避免这种情形。

```cpp
std::string encryptPassword(const std::string& password) {
    using namespace std;
    string encrypted;
    // 这一旦发生异常，encrypted虽然定义并被释放，但是却根本没有用到
    if (password.length() < MinimuPasswordLength) {
        throw logic_error("password is too short");
    }
    ...
   	return encrypted;
}
```

- 尽可能延后变量定义式的出现，这样做可以增加程序的清晰度并改善程序的效率。

### 尽量少做转型动作

`C++`除了C语言中的强制类型转换，还新增了如下新的类型转换：

```cpp
// 将对象的常量性转除，也就是去除const限制
const_cast<T>(expression)
// 主要用于执行安全向下转型，也就是用来决定某个对象是否归属继承体系中的某个类型
// 它是唯一无法由旧式语法执行的动作，也是唯一可能耗费重大运行成本的转型动作
dynamic_cast<T>(expression)
// 意图执行低级转型实际动作可能取决于编译器，这也就表示它不可移植，例如将一个pointer to int 转型为int
reinterpret_cast<T>(expression)
// 用来强迫隐士转换，例如将non-const对象转换为const对象，或将int转换为double等等，他也可以执行上述操作的反向转换，例如将
// void * 指针转换为typed指针，将pointer-to-base转为pointer-to-derived，但它无法将const转为non-const这个只有const_cast才办得到
static_cast<T>(expression)
```

```cpp
class Base {};
class Derived : public Base {};
Derived d;
Base* pd = &d; // 隐喻地将Derived*转换为Base*
// 加入进入一个函数，你只能拿到Base* 但是你想调用Derived的函数
// 你又不确认传进来的是否是 Derived的对象指针，这个时候可以使用dynamic_cast
// 如：
if (Derived *pDerived = dynamic_cast<Derived*>pd)
```

这里我们不过是建立一个`base class`指针指向一个`derived class`对象，但有时候上述两个指针的值并不 相同。这种情况下会有一个偏移量(`offset`)在运行期间被施于`Derived*`指针上，用意取得正确的`Base*`指针值。

以上例子说明，单一对象可能拥有一个以上的地址，这种现象C不可能发生，`java`和`C#`也不可能发生这种事，但是`C++`可能！实际上一旦使用多重继承，这种事几乎一直发生着，即使单一继承中也可能发生。虽然这还有其他意涵，但是至少意味着你通常应该避免做出对象在`C++`中如何如何布局的假设。当然更不应该以此为假设的基础上执行任何转型动作。

因此、依赖对象布局方式济南西给你的地址设计方式转型，在有的编译器上行得通，在其他平台可能就行不通了。

- 如果可以，尽量避免转型，特别是在注重效率的代码中避免`dynamic_cast`，如果有个设计需要转型操作，试着发展无需转型的替代设计
- 如果转型是必须的，试着将它隐藏于某个函数背后。客户随后可以调用该函数，而不需将转型放进它们自己的代码内
- 宁可使用`C++`新式风格的转型，不要使用旧式的转型。前者容易辨认出来。



### 认识模板(`template`)元编程

```cpp
//
// Created by andrew on 2021/6/26.
//
#include <iostream>

// 模板递归实现编译期间进行阶乘
template<unsigned n>
class Factorial { // 一般情况下: Factorial 的值
public:
    enum {value = n * Factorial<n - 1>::value};
};

// 特殊情况进行特化
template<>
class Factorial<0> {
public:
    enum {value = 1};   // 特殊情况下 Factorial<0>的值是1
};

int main(int argc, char *argv[]) {

    std::cout << Factorial<3>::value << std::endl;  // 6
    std::cout << Factorial<5>::value << std::endl;  // 120

    return 0;
}
```



### 绝对不要重新定义继承而来的缺省参数值

`virtual`函数是动态绑定，而缺省参数是静态绑定的。





















