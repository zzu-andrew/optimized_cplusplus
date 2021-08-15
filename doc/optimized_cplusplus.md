## `C++`性能优化

[点击获取源码](https://github.com/zzu-andrew/optimized_cplusplus/tree/main/src/optimize_file

[https://github.com/zzu-andrew/optimized_cplusplus/tree/main/src/optimize_file]: https://github.com/zzu-andrew/optimized_cplusplus/tree/main/src/optimize_file

### `RAIL`风格

`RAII`是指C++语言中的一个惯用法（idiom），它是“**R**esource **A**cquisition **I**s **I**nitialization”的首字母缩写。中文可将其翻译为“资源获取就是初始化”。虽然从某种程度上说这个名称并没有体现出该惯性法的本质精神，但是作为标准C++资源管理的关键技术，`RAII`早已在C++社群中深入人心。

因为系统的中的资源不是无限的，因此、通常的使用方式是

1. 获取资源
2. 使用资源
3. 释放资源

如：

```cpp
void fileHandler() {
    FILE * f = fopen("test.txt", "rw");
    someFunction();
    fclose(f);
}
```

但是资源过多的时候就会出现漏释放的情况导致资源泄露，`RAIL`的使用方式在`C++`中随处可见，比如标准库中的`lock_guard<std::mutex>`的实现。

因为，锁的特殊性。锁的申请没有放到`lock_guard`中实现，但是锁的加锁和锁的释放是采用了`RAIL`的模式。

```cpp
/** @brief A simple scoped lock type.
 *
 * A lock_guard controls mutex ownership within a scope, releasing
 * ownership in the destructor.
*/
template<typename _Mutex>
class lock_guard
{
    public:
    typedef _Mutex mutex_type;
	// 在构造函数中加锁
    explicit lock_guard(mutex_type& __m) : _M_device(__m)
    { _M_device.lock(); }

    lock_guard(mutex_type& __m, adopt_lock_t) noexcept : _M_device(__m)
    { } // calling thread owns mutex
	// 析构函数中释放锁
    ~lock_guard()
    { _M_device.unlock(); }
	// 拒绝隐式转换
    lock_guard(const lock_guard&) = delete;
    lock_guard& operator=(const lock_guard&) = delete;

    private:
    mutex_type&  _M_device;
};
```

### `const`函数和`const`对象



- `const`修饰函数时，表明该函数不能修改成员变量
- `const`修饰对象时，表明该对象不能引用成员中非`const`的成员函数

```cpp
#include <iostream>
#include <mutex>

using namespace std;

/*
 * 1. const成员函数不能修改成员变量
 * 2. const对象不能引用非const成员函数
 * */

class WatchData {
public:
    WatchData() = default;
    ~WatchData() = default;

    virtual int GetData() const  {
        return m_index;
    }
    // 函数后面的const是修饰所有的成员变量在该函数中为const
    /*void SetData(int &&idx) const {  // error: assignment of member ‘WatchData::m_index’ in read-only object
        m_index = idx;
    }*/

    virtual void SetData(int &&idx) {
        m_index = idx;
    }

private:
    int m_index{};
};

class StandData : public WatchData {
public:
    StandData() = default;
    ~StandData() = default;
    int GetData() const override {
        return m_index;
    }
    // 函数后面的const是修饰所有的成员变量在该函数中为const
    /*void SetData(int &&idx) const {  // error: assignment of member ‘WatchData::m_index’ in read-only object
        m_index = idx;
    }*/

    void SetData(int &&idx) override {
        m_index = idx;
    }

private:
    int m_index{};
};

int main(int argc, const char** argv) {

    WatchData watchData{};
    watchData.SetData(6);
    cout << watchData.GetData() << endl;
    // const 对象不能引用非const的成员函数
    const WatchData *pWatchData = &watchData;
    //  pWatchData->SetData(4);  error: passing ‘const WatchData’ as ‘this’ argument discards qualifiers [-fpermissive]
    StandData standData{};
    standData.WatchData::SetData(43);
    cout << standData.WatchData::GetData() << endl;

    cout << "test" << endl;
    return 0;
}
```



### 字符串的优化

#### 糟糕的字符串连接函数

在`C++`中字符串是按照值的形式实现的，又因为`C++`中字符串底层是使用动态内存实现的，因此、在项目中对字符串的优化必不可少，也是性能优化的重点。

假如代码中有如下`remove_ctrl`函数的实现：

```cpp
std::string remove_ctrl(std::string s) {
    std::string result;
    for (int i=0; i<s.length(); ++i) {
        if(s[i] >= 0x20)
            result = result + s[i];
    }
    return result;
}
```

`remove_ctrl()` 在循环中对通过参数接收到的字符串 s 的每个字符进行处理。循环中的代码就是导致这个函数成为热点的原因。 `if` 条件语句从字符串中得到一个字符，然后与一个字面常量进行比较。这里没有什么问题。但是第 5 行的语句就不一样了。

正如之前所指出的，字符串连接运算符的开销是很大的。它会调用内存管理器去构建一个新的临时字符串对象来保存连接后的字符串。如果传递给 `remove_ctrl()` 的参数是一个由大于`0x2`(可打印)的字符组成的字符串，那么 `remove_ctrl()` 几乎会为 s 中的每个字符都构建一个临时字符串对象。对于一个由 100 个字符组成的字符串而言，这会调用 100 次内存管理器来为临时字符串分配内存，调用 100 次内存管理器来释放内存

对上述代码进行基准测试，结果如下：

```bash
-------------------------------------------------------------------------
Benchmark                               Time             CPU   Iterations
-------------------------------------------------------------------------
bench_remove_ctrl_operator          12433 ns        12432 ns        54195
```

如果你的代码编辑器带自动检查，就会发现这样的代码连代码编辑器都受不了，给你显示一大堆波浪号。

#### 使用复合赋值操作避免临时字符串

注意观察上面实现的第5行代码`result = result + s[i];`，因为字符串是按照值处理的，等号右边的值想加后会在挨个赋值到等号左边，这样如果参数字符串有n个字符，那么`remove_ctrl`会复制O(n[^2])个自负，所有这些内存分配和复制都会导致性能变差。

为了减少字符串的复制，可以使用复合操作符`+=`，使用复合操作符之后的代码如下:

```cpp
std::string remove_ctrl(std::string s) {
    std::string result;
    for (int i=0; i<s.length(); ++i) {
        if(s[i] >= 0x20)
            result += s[i];
    }
    return result;
}
```

改进之后测试结果：

```bash
-------------------------------------------------------------------------
Benchmark                               Time             CPU   Iterations
-------------------------------------------------------------------------
bench_remove_ctrl_operator          12433 ns        12432 ns        54195
bench_remove_ctrl_operator_opt      12400 ns        12400 ns        56281
```

可以看出测试结果中改进之后的代码明显比没有改进之前快很多，而且这个时间会随着传入的字符串的长度增长而增加，传入的字符串越长，没有改进的代码需要复制的就越多。

#### 通过预留存储空间来减少内存的重新分配

经过上述优化之后，`result`仍然存在在执行的过程中，由于缓存区不够需要重新分配的情况，导致频繁分配内存，可以通过提前分配内存优化该问题，结果如下：

```cpp
std::string remove_ctrl_reserve(std::string s) {
    std::string result;
    result.reserve(s.length());
    for (int i=0; i< (int)s.length(); ++i) {
        if(s[i] >= 0x20)
            result += s[i];
    }
    return result;
}
```

运行结果：

```bash
-------------------------------------------------------------------------
Benchmark                               Time             CPU   Iterations
-------------------------------------------------------------------------
bench_remove_ctrl_operator          12170 ns        12168 ns        56990
bench_remove_ctrl_operator_opt      12390 ns        12388 ns        57518
bench_remove_ctrl_reserve             767 ns          767 ns       908214
```

#### 消除对参数字符串的复制

上述方法中还是会对参数字符串进行一次复制，由于内存分配是非常昂贵的，因此哪怕是一次内存分配也值得从程序中去除，优化之后参数按照引用传递，结果如下：

```cpp
std::string remove_ctrl_ref_args(std::string &s) {
    std::string result;
    result.reserve(s.length());
    for (int i=0; i< (int)s.length(); ++i) {
        if(s[i] >= 0x20)
            result += s[i];
    }
    return result;
}
```

更改之后运行结果：

```cpp
-------------------------------------------------------------------------
Benchmark                               Time             CPU   Iterations
-------------------------------------------------------------------------
bench_remove_ctrl_operator          12158 ns        12159 ns        56439
bench_remove_ctrl_operator_opt      12044 ns        12046 ns        57409
bench_remove_ctrl_reserve             762 ns          762 ns       921342
bench_remove_ctrl_ref_args            737 ns          738 ns       938882
```

#### 移除对返回值的复制

经过上述的优化之后，性能已经有了很大的改善。但是仔细观察代码还是能发现存在不必要的内存申请的地方，那就是返回值，每次在处理好字符串之后，返回时还需要复制一次，这时也会申请内存。我们完全可以将字符串通过引用传出去，改进之后的代码如下：

```cpp
void remove_ctrl_ref_result_it(std::string &result, std::string const &s) {
    result.clear();
    result.reserve(s.length());
    for (auto &it:s) {
        if (it >= 0x20)
            result += it;
    }
}
```

改进之后的运行结果如下：

```bash
--------------------------------------------------------------------------
Benchmark                                Time             CPU   Iterations
--------------------------------------------------------------------------
bench_remove_ctrl_operator           11948 ns        11949 ns        55770
bench_remove_ctrl_operator_opt       12088 ns        12089 ns        58264
bench_remove_ctrl_reserve              767 ns          767 ns       899659
bench_remove_ctrl_ref_args             742 ns          742 ns       948853
bench_remove_ctrl_ref_args_it          758 ns          758 ns       922450
bench_remove_ctrl_ref_result_it        493 ns          493 ns      1382138
```

可以看到，当返回值也通过引用返回时，函数调用的耗时一下子降低到了493纳秒。

#### 用字符串代替数组

当程序有严格的性能要求的时候，不是使用`C++`风格的字符串，而应该使用`C`风格的字符数组，使用字符数组需要程序员自己控制内存的申请和释放，能够最大程度提升性能，但是带来的后果就是维护的成本增加

```cpp
void remove_ctrl_cstrings(char* destp, char const* srcp, size_t size) {
    for (size_t i=0; i<size; ++i) {
        if (srcp[i] >= 0x20)
            *destp++ = srcp[i];
    }
    *destp = 0;
}
```

使用`C`风格的字符数组测试结果：

```bash
--------------------------------------------------------------------------
Benchmark                                Time             CPU   Iterations
--------------------------------------------------------------------------
bench_remove_ctrl_operator           12103 ns        12103 ns        55630
bench_remove_ctrl_operator_opt       12175 ns        12175 ns        57324
bench_remove_ctrl_reserve              765 ns          765 ns       918301
bench_remove_ctrl_ref_args             746 ns          746 ns       938322
bench_remove_ctrl_ref_args_it          751 ns          751 ns       912842
bench_remove_ctrl_ref_result_it        497 ns          497 ns      1394954
bench_remove_ctrl_cstrings             270 ns          270 ns      2575663
```

#### 使用更好的算法

有没有即好维护又有高性能的的方法，答案是有的，那就是使用更好的算法。

经过对以上的改进的思考，我们发现只要想办法能够减少内存的申请和释放，字符串也能有很高的性能，这就是新算法需要下手的地方。

因为、老的实现方式，是每个值都要对比之后，挨个添加到`result`中，这就导致频繁的操作内存，新算法中使用`append`一段一段的取符合条件的字符串添加到`result`中，实际测试结果141纳秒左右，已经优于`C`风格的字符串，这就要感谢算法带来的`buff`加成。

```cpp
void remove_ctrl_block_append(std::string &result, const std::string &s) {
    result.clear();
    result.reserve(s.length());
    for (size_t b=0,i=b; b < s.length(); b = i+1) {
        for (i=b; i<s.length(); ++i) {
            if (s[i] < 0x20) break;
        }
        result.append(s, b, i-b);
    }
}
```

运行结果：

```bash
--------------------------------------------------------------------------
Benchmark                                Time             CPU   Iterations
--------------------------------------------------------------------------
bench_remove_ctrl_operator           12258 ns        12257 ns        54639
bench_remove_ctrl_operator_opt       12347 ns        12347 ns        56488
bench_remove_ctrl_reserve              765 ns          765 ns       896059
bench_remove_ctrl_ref_args             736 ns          736 ns       947049
bench_remove_ctrl_ref_args_it          746 ns          746 ns       926601
bench_remove_ctrl_ref_result_it        494 ns          494 ns      1394869
bench_remove_ctrl_cstrings             270 ns          270 ns      2562077
bench_remove_block_append              141 ns          141 ns      4941262
```

当然，还可以使用剔除不符合要求字符的方式实现：

```cpp
void remove_ctrl_erase(std::string &s) {
    for (size_t i = 0; i < s.length();)
    if (s[i] < 0x20)
        s.erase(i,1);
    else ++i;
}
```

测试结果：

```bash
--------------------------------------------------------------------------
Benchmark                                Time             CPU   Iterations
--------------------------------------------------------------------------
bench_remove_ctrl_operator           12146 ns        12146 ns        56901
bench_remove_ctrl_operator_opt       12254 ns        12254 ns        56758
bench_remove_ctrl_reserve              756 ns          756 ns       909901
bench_remove_ctrl_ref_args             735 ns          735 ns       938884
bench_remove_ctrl_ref_args_it          748 ns          748 ns       922788
bench_remove_ctrl_ref_result_it        494 ns          494 ns      1422658
bench_remove_ctrl_cstrings             275 ns          275 ns      2534856
bench_remove_block_append              143 ns          143 ns      4871896
bench_remove_erase                     169 ns          169 ns      4104307
```

当然、还可以使用更优秀的库，使用优化性能更好的编译器，或者为自己的业务专门定制一个字符串的类，来达到最优。







### 移动语义

#### 移动语义的微妙之处

开发人员必须将移动构造函数和移动赋值运算符声明为`noexcept`。这很有必要，因为 `std::vector` 提供了强异常安全保证（`strong exception safety guarantee`）：当一个 `vetcor` 执行某个操作时，如果发生了异常，那么该 `vetcor` 的状态会与执行操作之前一样。复制构造函数并不会改变源对象。移动构造函数则会销毁它。任何在移动构造函数中发生的异常都会与强异常安全保证相冲突。

如果没有将移动构造函数和移动赋值运算符声明为 `noexcept`， `std::vector` 会使用比较低效的复制构造函数。当发生这种情况时，编译器可能不会给出警告，代码仍然可以正常运行，不过会变慢。

`noexcept` 是一种强承诺。使用 `noexcept` 意味着不会调用内存管理器、 `I/O` 或是其他任何可能会抛出异常的函数。同时，它也意味着必须忍受所有异常，因为没有任何办法报告在程序中发生了异常。

#### 右值的引用参数是左值

右值引用有折叠的效果，`&& -> &, && && -> &&`，右值的参数经过函数传递之后就会折叠成左值引用。

当一个函数接收一个右值引用作为参数时，他会使用右值引用来构建形参，因为、形参是有名字的，所以尽管它构建于一个右值引用，它仍然是一个左值。

幸运的是，开发人员可以显式地将左值转换为右值引用。如代码清单 `6-4` 所示，标准库提供了漂亮的 <`utility`> 中的模板函数 `std::move()` 来完成这项任务。

```cpp
std::string MoveExample(std::string&& s) {
std::string tmp(std::move(s));
// 注意！现在s是空的
return tmp;
}
...
std::string s1 = "hello";
std::string s2 = "everyone";
std::string s3 = MoveExample(s1 + s2);
```

`RVO`返回值优化，就是当返回值需要先构造`tmp`临时对象，在根据赋值构造函数将临时对象赋值给接收的左值得情况下，直接接收值的引用作为隐藏参数使用，这样就不会在返回的时候创建临时对象。

从概念上讲， `tmp` 的值会被复制到匿名返回值中，接着 `tmp` 会被删除。`MoveExample()` 的匿名返回值会被复制构造到 `s3` 中。不过，实际上，在这种情况下编译器能够进行 `RVO`，这样参数 `s` 会被直接移动到 `s3` 的存储空间中。通常， `RVO` 比移动更高效。

#### 不要返回右值引用

移动语义的另外一个微妙之处在于不应当定义函数返回右值引用。直觉上，返回右值引用是合理的。在像 `x = foo(y)` 这样的函数调用中，返回右值引用会高效地将返回值从未命名的临时变量中复制到赋值目标 x 中。但是实际上，返回右值引用会妨碍返回值优化，即允许编译器向函数传递
一个指向目标的引用作为隐藏参数，来移除从未命名的临时变量到目标的复制。返回右值引用会执行两次移动操作，而一旦使用了返回值优化，返回一个值则只会执行一次移动操作。因此，只要可以使用 `RVO`，无论是返回语句中的实参还是函数的返回类型，都不应当使用右值引用。



### 优化热点语句

#### 提前计算固定值

先观察下面的性能测试代码：

```cpp
static void find_blank(benchmark::State& state) {

    for (auto _: state) {
        char s[] = "This string has many space (0x20) chars. ";

        for (size_t i = 0; i < strlen(s); ++i)
            if (s[i] == ' ')
                s[i] = '*';
    }
}
BENCHMARK(find_blank);
```

这段代码对字符串中的每个字符都会判断循环条件 `i < strlen(s)` 是否成立 1。调用`strlen()` 的开销是昂贵的，遍历参数字符串对它的字符计数使得这个算法的开销从 `O(n)`变为了 `O(n2)`。这是一个在库函数中隐藏了循环的典型例子

既然，每次`strlen()`调用都会导致一次遍历，并且计算结果不会随着函数的运行而改变，可以尝试先求出`strlen`保存，然后后期就直接使用计算结果，而不是每次循环都进行计算，如下：

```cpp
static void find_blank_init_length(benchmark::State& state) {

    for (auto _: state) {
        char s[] = "This string has many space (0x20) chars. ";

        for (size_t i = 0, len = strlen(s); i < len; ++i)
            if (s[i] == ' ')
                s[i] = '*';
    }
}

BENCHMARK(find_blank_init_length);
```

测试结果如下：

```cpp
-----------------------------------------------------------------
Benchmark                       Time             CPU   Iterations
-----------------------------------------------------------------
find_blank                    191 ns          191 ns      3431752
find_blank_init_length       72.4 ns         72.4 ns      9635766
```

在禁用变异优化的选项下编译，从计算结果可以看出，更改之后整个函数性能提升了将近3倍左右。

#### 使用更加高效的循环

通常`for`循环将会被编译成如下的代码：

```assembly
	初始化表达式；
L1: if (!循环条件) goto L2;
	语句；
	继续表达式;
	goto L1;
L2:
```

而`do-while`编译之后一般为：

```assembly
L1: 控制语句
	if (循环条件) goto L1;
```

当然不同的编译器可能实现不一样，按照上述分析使用`do-while`肯定要比`for`循环要好很多，但是，在`ubuntu20.04`上实际测试`for`循环的速度基本上和`do-while`保持一致，也可能是`for`循环用的多，所以编译器哪些大佬特意特意进行了优化。

`do-while`的实现：

```cpp
static void find_blank_do_while(benchmark::State& state) {

    for (auto _: state) {
        char s[] = "This string has many space (0x20) chars. ";
        size_t i = 0, len = strlen(s);
        do {
            if (s[i] == ' ')
                s[i] = '*';
            ++ i;
        }while (i < len);
    }
}

BENCHMARK(find_blank_do_while);
```

实际测试结果：

```bash
-----------------------------------------------------------------
Benchmark                       Time             CPU   Iterations
-----------------------------------------------------------------
find_blank                    191 ns          191 ns      3431752
find_blank_init_length       72.4 ns         72.4 ns      9635766
find_blank_do_while          71.6 ns         71.6 ns      9629498
```

#### 使用编译器进行优化

在不更改代码的情况下，可以更改优化选项，告诉编译器可以对代码进行优化，当编译器选项由`O0`更改为`O3`之后的测试结果如下：

```bash
-----------------------------------------------------------------
Benchmark                       Time             CPU   Iterations
-----------------------------------------------------------------
find_blank                   60.4 ns         60.4 ns     10536782
find_blank_init_length       34.6 ns         34.6 ns     20298248
find_blank_do_while          34.4 ns         34.4 ns     20249507
-----------------------------------------------------------------
Benchmark                       Time             CPU   Iterations
-----------------------------------------------------------------
find_blank                   60.2 ns         60.2 ns      9566706
find_blank_init_length       34.4 ns         34.4 ns     20362644
find_blank_do_while          34.6 ns         34.6 ns     20355712
```

多次运行之后，使用`for`循环和使用`do-while`结构基本上没有任何差别，而代价就是编译过程慢了一点。

#### 将循环放入函数以减少调用开销



如果程序要遍历字符串、数组或是其他数据结构，并会在每次迭代中都调用一个函数，那么可以通过一种称为循环倒置（`loop inversion`）的技巧来提高程序性能。循环倒置是指将在循环中调用函数变为在函数中进行循环。

看下面这个例子：

```cpp
# include <ctype>
void replace_nonprinting(char& c) {
    if (!isprint(c))
        c = '.';
}

for (unsigned i = 0, e = str.size(); i < e; ++i)
	replace_nonprinting(str[i]);
```

将循环体放进函数内效果更好，省去了函数的调用

```cpp
void replace_nonprinting(std::string& str) {
    for (unsigned i = 0, e = str.size(); i < e; ++i)
    	if (!isprint(str[i]))
    		c = '.';
}
```

#### 将虚析构函数移至基类中

任何有继承类的类的析构函数都应当被声明为虚函数。这是有必要的，这样 `delete` 表达式将会引用一个指向基类的指针，继承类和基类的析构函数都会被调用。

另外一个在继承层次关系顶端的基类中声明虚函数的理由是：确保在基类中有虚函数表指针。继承层次关系中的基类处于一个特殊的位置。如果在这个基类中有虚成员函数声明，那么虚函数表指针在其他继承类中的偏移量是 0；如果这个基类声明了成员变量且没有声明任何虚成员函数，但是有些继承类却声明了虚成员函数，那么每个虚成员函数调用都会在 this 指针上加上一个偏移量来得到虚函数表指针的地址。确保在这个基类中至少有一个
成员函数，可以强制虚函数表指针出现在偏移量为 0 的位置上，这有助于产生更高效的代码。

而析构函数则是最佳候选。如果这个基类有继承类，它就必须是虚函数。





你遇见谁，都是注定的

你相信吗，你身上发生的所有的事情都是应该发生的，都是有姻缘的，起心动念介是因，当下所受皆是果，你与见谁，你喜欢谁、你恨谁，谁伤害了你，谁又辜负了你，你失去了什么，你拥有什么，无一不是因果，坦然接受当下的自己，一切都是最好的安排































