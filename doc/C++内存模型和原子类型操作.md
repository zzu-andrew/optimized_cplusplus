### C++内存模型和原子类型操作

#### std::memory_order初探

动态内存模型可以理解为存储一致性模型，主要是从行为上来看多个线程对同一个对象读写操作时所做的约束，动态内存理解起来会有少许复杂，涉及到内存、Cache、CPU的各个层次的交互。

如下有两个线程，分别对a、R1、b、R2进行赋值，根据线程执行的顺序可能有以下几种情况

![img](image/16290324870631.png)

在不对线程进行任何限制，线程内部指令不进行重排的情况下。可以有4!/(2!*2!)=6中情况

在不考虑优化和指令重排的情况下，多线程有如下两种情况：

1. 程序最终执行的结果，是多个线程交织执行的结果
2. 从单个线程来看，该线程的指令是按照事先已经规定好的执行顺序执行

当然，现在的编译器都支持指令重排，上述的现象也只是理想情况下的执行情况，因为顺序一致性代价太大不利于程序的优化。但是有时候你需要对编译器的优化行为做出一定的约束，才能保证你的程序行为和你预期的执行结果保持一致，那么这个约束就是**内存模型。**

C++程序员想要写出高性能的多线程程序必须理解内存模型，编译器会给你的程序做静态优化，CPU为了提升性能也有动态乱序执行的行为。总之，实际编程中程序不会完全按照你原始代码的顺序来执行，因此内存模型就是程序员、编译器、CPU之间的契约。编程、编译、执行都会在遵守这个契约的情况下进行，在这样的规则之上各自做自己的优化，从而提升程序的性能。

**Memory Order**

内存的顺序描述了计算机CPU获取内存的顺序，内存的排序可能静态也可能动态的发生：

- 静态内存排序：编译器期间，编译器对内存重排
- 动态内存排序：运行期间，CPU乱序执行 

静态内存排序是为了提高代码的利用率和性能，编译器对代码进行了重新排序；同样为了优化性能CPU也会进行对指令进行重新排序、延缓执行、各种缓存等等，以便达到更好的执行效果。虽然经过排序确实会导致很多执行顺序和源码中不一致，但是你没有必要为这些事情感到棘手足无措。任何的内存排序都不会违背代码本身所要表达的意义，并且在单线程的情况下通常不会有任何的问题。

但是在多线程场景中，无锁的数据结构设计中，指令的乱序执行会造成无法预测的行为。所以我们通常引入内存栅栏这一概念来解决可能存在的并发问题。

**Memory Barrier**

内存栅栏是一个令 CPU 或编译器在内存操作上限制内存操作顺序的指令，通常意味着在 barrier 之前的指令一定在 barrier 之后的指令之前执行。

在 C11/C++11 中，引入了六种不同的 memory order，可以让程序员在并发编程中根据自己需求尽可能降低同步的粒度，以获得更好的程序性能。这六种 order 分别是：

```cpp
relaxed, acquire, release, consume, acq_rel, seq_cst
```

C++11中规定了如下6种访问次序(Memory Oreder)

```cpp
enum memory_order {
    memory_order_relaxed,
    memory_order_consume,
    memory_order_acquire,
    memory_order_release,
    memory_order_acq_rel,
    memory_order_seq_cst
};
```

上述6种访问次序可以分为3类

1. 顺序一致性模型 std::memory_order_seq_cst 
2. acquire-release 获取/释放语义模型std::memory_order_consume, std::memory_order_acquire, std::memory_order_release, std::memory_order_acq_rel
3.  Relax 宽松的内存序列化模型:又称为自由序列模型 ，(std::memory_order_relaxed)

以上三类可以组合成如下种类的模型：

**自由序列模型(宽松的内存序列化模型):**

> 在单个线程内，所有原子操作时顺序进行的。按照什么顺序？按照代码顺序，这也就是该模型的唯一限制。两个来自不同线程的原子操作顺序是任意的

**获取释放语义模型**

> Release -- acquire模型，在自由序列模型中来自两个线程之间的原子操作时顺序是不一定的，那么就需要把两个线程进行一下同步(synchronize-with)。同步什么？同步对一个变量的读写操作。线程A原子性的把值写入X(release)，然后线程B原子性地读取X的值(acquire)。这样线程B保证读取到X的新值。需要注意的是release有个牛逼的副作用-线程A中所有发生在releaseX之前的写操作，对线程BacqueireX之后的任何读操作都可见！本来A,B之间的读写操作顺序不定。这么已同步在X这个点前后，A,B线程之间有了个顺序关系，称作inter-thread happens-before

release-consume模型：不会吧，只是想要同步一个x的读写操作，结果把release之前的所有写操作都顺带同步了！！！这对于不需要所有都同步的操作来说开销也太大了。有没有办法能够有效降低开销，又能同步X呢？用release-consume呗，同步还是一样的同步。这一会的副作用弱了点：在线程B acqueire X之后的读操作中，有一些是依赖X的值的读操作。管这些依赖X的读操作叫做-赖B读。同理在线程A里面，release X也有一些它所依赖的其他写操作，这些操作自然发生在release X之前了，管这些操作叫做-赖A写。副作用总结来说就是，只有赖B读能看见赖A写

那么写到这里了，什么叫做赖，赖就是依赖的意思，release-acquire模型是大把抓的形式，把所有release之前的写和acquire之后的读都进行同步。而release-consume控制的更加精细，只有有依赖关系的才进行同步，依赖关系可以按照如下理解：

数据依赖：

```
S1. c = a + b;
S2. e = c + d;
```

数据S2是依赖S1的因为在计算S2的时候需要依赖S1中计算出来的c值。

**顺序一致性模型**

> Sequential consistency:前面的模型理解了，顺序一致性也就好理解了，release-acquire就同步了一个x，顺序一致就是对所有的变量的所有原子操作都进行同步。那么这样一来所有的原子操作就跟由一个线程顺序执行似的

#### 6种访问次序的说明

**memory_order_relaxed**

memory_order_relaxed: 只保证当前操作的原子性，不考虑线程间的同步，其他线程可能读到新值，也可能读到旧值。也就是说，原子操作标记 memory_order_relaxed不是同步操作；它们不会在并发内存访问之间强加顺序。它们只保证原子性和修改顺序的一致性。

Relaxed operation: there are no synchronization or ordering constraints imposed on other reads or writes, only this operation's atomicity is guaranteed(see [Relaxed ordering](https://en.cppreference.com/w/cpp/atomic/memory_order#Relaxed_ordering) below)

例如：

```cpp
// Thread 1:
r1 = y.load(std::memory_order_relaxed); // A
x.store(r1, std::memory_order_relaxed); // B
// Thread 2:
r2 = x.load(std::memory_order_relaxed); // C 
y.store(42, std::memory_order_relaxed); // D
```

上述代码会产生r1==r2, ==42的情况，尽管A在线程1中序列在B的前面，C的序列在线程2中在D的前面，没有什么能够阻止D排序到A的前面，也没有什么能够阻止B排到A的前面。唯一可以预见的是线程1中x的存储A对线程2中的C可见。也就是使用memory_order_relaxed除了保证当前原子变量的可见性和原子性。

即使是memory_order_relaxed也不能出现循环依赖的情况

```cpp
// Thread 1:
r1 = y.load(std::memory_order_relaxed);
if (r1 == 42) x.store(r1, std::memory_order_relaxed);
// Thread 2:
r2 = x.load(std::memory_order_relaxed);
if (r2 == 42) y.store(42, std::memory_order_relaxed);
```

上述代码不可能出现r1== r2 ,  == 42, 的情况，因为r1== 42, 依赖r2存储为42,而, r2 ==42依赖r1存储为42。上述代码直到C++14才被规则上允许，但是还是不建议使用上述方式给用户实现since C++14

relaxed的通常使用方式是用来计数，例如 [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr)中的引用计数，因为这只需要原子性，并不需要进行排序或者同步

```cpp
#include <vector>
#include <iostream>
#include <thread>
#include <atomic>
 
std::atomic<int> cnt = {0};

// memory_order_relaxed保证当前变量在不同线程中的原子性
void f()
{
    for (int n = 0; n < 1000; ++n) {
        cnt.fetch_add(1, std::memory_order_relaxed);
    }
}
 
int main()
{
    std::vector<std::thread> v;
    for (int n = 0; n < 10; ++n) {
        v.emplace_back(f);
    }
    for (auto& t : v) {
        t.join();
    }
    std::cout << "Final counter value is " << cnt << '\n';
}
```

Output:

```
Final counter value is 10000
```

**memory_order_release**

A store operation with this memory order performs the *release operation*: no reads or writes in the current thread can be reordered after this store. All writes in the current thread are visible in other threads that acquire the same atomic variable (see [Release-Acquire ordering](https://en.cppreference.com/w/cpp/atomic/memory_order#Release-Acquire_ordering) below) and writes that carry a dependency into the atomic variable become visible in other threads that consume the same atomic (see [Release-Consume ordering](https://en.cppreference.com/w/cpp/atomic/memory_order#Release-Consume_ordering) below).

 memory_order_release:（可以理解为 mutex 的 unlock 操作）

- 对写入施加 release 语义（store），在代码中这条语句前面的所有读写操作都无法被重排到这个操作之后，即 store-store 不能重排为 store-store, load-store 也无法重排为 store-load
- 当前线程内的所有写操作，对于其他对这个原子变量进行 acquire 的线程可见
- 当前线程内的与这块内存有关的所有写操作，对于其他对这个原子变量进行 consume 的线程可见

**memory_order_acquire**

 memory_order_acquire: （可以理解为 mutex 的 lock 操作）

A load operation with this memory order performs the *acquire operation* on the affected memory location: no reads or writes in the current thread can be reordered before this load. All writes in other threads that release the same atomic variable are visible in the current thread (see [Release-Acquire ordering](https://en.cppreference.com/w/cpp/atomic/memory_order#Release-Acquire_ordering) below)

对读取施加 acquire 语义（load），在代码中这条语句前面所有读写操作都无法重排到这个操作之前，即 load-store 不能重排为 store-load, load-load 也无法重排为 load-load

这里一定要理解原子操作、内存屏障的作用，在如下代码你可以想象运行期间绝对能保证 c.store(3, memory_order_release)与c.load(memory_order_acquire)是原子的，那么在acquire之前的不能进行重新排，就保证了a,b的值是自己想要的，记住是跨线程的原子性哦。

在这个原子变量上施加 release 语义的操作发生之后，acquire 可以保证读到所有在 release 前发生的写入，举个例子：

```cpp
c = 0;
thread 1:{ 
    a = 1; 
    b.store(2, memory_order_relaxed); 
    c.store(3, memory_order_release);
}
thread 2:{ 
    while (c.load(memory_order_acquire) != 3) ; // 以下 assert 永远不会失败 
    assert(a == 1 && b == 2);
     assert(b.load(memory_order_relaxed) == 2);
}
```



如果你个原子变量在线程A存储时被打上了memory_order_release标签，在线程B加载的时候被打上了memory_order_acquire标签。A线程中所有在原子变量存储之前的写操作(包括非原子变量和relaxed标记的原子变量)都会变得在B线程可见。也就是只要线程B中完成了load操作那么就可以保证在线程A中写入的所有数据在线程B中可见。

同步的效果只有在对同一个原子变量releasing和acquiring的线程间有效果，对于其他的线程无效

> Mutual exclusion locks, such as [std::mutex](https://en.cppreference.com/w/cpp/thread/mutex) or [atomic spinlock](https://en.cppreference.com/w/cpp/atomic/atomic_flag), are an example of release-acquire synchronization: when the lock is released by thread A and acquired by thread B, everything that took place in the critical section (before the release) in the context of thread A has to be visible to thread B (after the acquire) which is executing the same critical section.

```cpp
#include <thread>
#include <atomic>
#include <cassert>
#include <string>
 std::atomic<std::string*> ptr;
int data;
 void producer()
{
    std::string* p  = new std::string("Hello");
    data = 42;
    // 在调用release之后，其上的所有写操作对于acquire的线程可见
    ptr.store(p, std::memory_order_release);
}
 void consumer()
{
    std::string* p2;
    // load 调用acquire之后，其下的所有可读数据都在release之后
    while (!(p2 = ptr.load(std::memory_order_acquire)))
        ;
    assert(*p2 == "Hello"); // never fires
    assert(data == 42); // never fires
}
 int main()
{
    std::thread t1(producer);
    std::thread t2(consumer);
    t1.join(); t2.join();
}
```

如下代码通过原子变量实现了三个线程之间的同步

```cpp
#include <thread>
#include <atomic>
#include <cassert>
#include <vector>
#include <iostream>

std::vector<int> data;
std::atomic<int> flag = {0};

void thread_1()
{
    data.push_back(42);
    flag.store(1, std::memory_order_release);
}

void thread_2()
{
    int expected=1;
    // 执行顺序先acq 再rel
    while (!flag.compare_exchange_strong(expected, 2, std::memory_order_acq_rel)) {
        expected = 1;
    }
}

void thread_3()
{
    std::this_thread::sleep_for(std::chrono::milliseconds (10000));
    // 只有flag的值不小于2的时候才退出
    while (flag.load(std::memory_order_acquire) < 2)
        ;
    assert(data.at(0) == 42); // will never fire
}

/*
 * 经过上述操作执行顺序是，先线程1再线程2然后是线程3，通过原子变量实现了线程间的同步
 * */
int main(int argc, char*argv[])
{
    std::thread a(thread_1);
    std::thread b(thread_2);
    std::thread c(thread_3);
    a.join(); b.join(); c.join();

    return 0;
}
```

 **memory_order_consume**

对当前要读取的内存施加 release 语义（store），在代码中这条语句后面所有与这块内存有关的读写操作都无法被重排到这个操作之前。也就是说当一个原子变量在线程A中被标记为`memory_order_release`，同样 原子变量线程B被使用memory_order_consume标记进行加载的时候，所有发生在A线程中原子变量store之前的写操作，对于那些 依赖关系的元素都将在B线程中可见

```cpp
#include <thread>
#include <atomic>
#include <cassert>
#include <string>

std::atomic<std::string*> ptr;
int data;

void producer()
{
    auto * p  = new std::string("Hello");
    data = 42;
    ptr.store(p, std::memory_order_release);
}

void consumer()
{
    std::string* p2;
    while (!(p2 = ptr.load(std::memory_order_consume)))
        ;
    // 因为p2一定依赖原子变量ptr所有这里一定成功
    assert(*p2 == "Hello"); // never fires: *p2 carries dependency from ptr
    assert(data == 42); // may or may not fire: data does not carry dependency from ptr
}

int main()
{
    std::thread t1(producer);
    std::thread t2(consumer);
    t1.join(); t2.join();
}
```

> The synchronization is established only between the threads *releasing* and *consuming* the same atomic variable. Other threads can see different order of memory accesses than either or both of the synchronized threads.

使用场景

> Typical use cases for this ordering involve read access to rarely written concurrent data structures (routing tables, configuration, security policies, firewall rules, etc) and publisher-subscriber situations with pointer-mediated publication, that is, when the producer publishes a pointer through which the consumer can access information: there is no need to make everything else the producer wrote to memory visible to the consumer (which may be an expensive operation on weakly-ordered architectures). An example of such scenario is [rcu_dereference](https://en.wikipedia.org/wiki/Read-copy-update).

A load operation with this memory order performs a *consume operation* on the affected memory location: no reads or writes in the current thread dependent on the value currently loaded can be reordered before this load. Writes to data-dependent variables in other threads that release the same atomic variable are visible in the current thread. On most platforms, this affects compiler optimizations only (see [Release-Consume ordering](https://en.cppreference.com/w/cpp/atomic/memory_order#Release-Consume_ordering) below)

> 在这个原子变量上施加 release 语义的操作发生之后，acquire 可以保证读到所有在 release 前发生的并且与这块内存有关的写入，举个例子：

```cpp
a = 0;
c = 0;
thread 1:{
    a = 1; 
    c.store(3, memory_order_release);
}
thread 2:{ 
    while (c.load(memory_order_consume) != 3) ; 
    assert(a == 1); // assert 可能失败也可能不失败
}
```

**memory_order_acq_rel**

A read-modify-write operation with this memory order is both an *acquire operation* and a *release operation*. No memory reads or writes in the current thread can be reordered before or after this store. All writes in other threads that release the same atomic variable are visible before the modification and the modification is visible in other threads that acquire the same atomic variable.

对读取和写入施加 acquire-release 语义，无法被重排

> 可以看见其他线程施加 release 语义的所有写入，同时自己的 release 结束后所有写入对其他施加 acquire 语义的线程可见

 **memory_order_seq_cst**（顺序一致性）

A load operation with this memory order performs an *acquire operation*, a store performs a *release operation*, and read-modify-write performs both an *acquire operation* and a *release operation*, plus a single total order exists in which all threads observe all modifications in the same order (see [Sequentially-consistent ordering](https://en.cppreference.com/w/cpp/atomic/memory_order#Sequentially-consistent_ordering) below)

1. 如果是读取就是 acquire 语义，如果是写入就是 release 语义，如果是读取+写入就是 acquire-release 语义
    1. 同时会对所有使用此 memory order 的原子操作进行同步，所有线程看到的内存操作的顺序都是一样的，就像单个线程在执行所有线程的指令一样

通常情况下，默认使用 memory_order_seq_cst，所以你如果不确定怎么这些 memory order，就用这个。

### 扩展

评论里有很多关于**x86内存模型**的指正，放在这里：

> Loads are not reordered with other loads.Stores are not reordered with other stores.Stores are not reordered with older loads.

然后最重要的：

> Loads may be reordered with older stores to different locations.

因为 store-load 可以被重排，所以x86不是顺序一致。但是因为其他三种读写顺序不能被重排，所以x86是 acquire/release 语义。

aquire语义：load 之后的读写操作无法被重排至 load 之前。即 load-load, load-store 不能被重排。

release语义：store 之前的读写操作无法被重排至 store 之后。即 load-store, store-store 不能被重排。

