Linux的Page Cache

## 1. Page Cache

### 何为Page Cache

为了了解Page Cache我们可以看一下Linux的文件I/O系统

![img](image/-16313515993491.png)

从图中可以看出，Page Cache是由Linux内核进行管理的，而且通过mmap以及bffered I/O将文件读取到内存空间实际上都是读取到Page Cache上的。

### 如何查看系统的Page Cache?

通过读取`cat /proc/meminfo `文件，查看系统实时内存情况。

```bash
...
Buffers:          117572 kB
Cached:          2738632 kB
SwapCached:        14560 kB
Active:          1444220 kB
Inactive:        5806180 kB
Active(anon):     509816 kB
Inactive(anon):  4946608 kB
Active(file):     934404 kB
Inactive(file):   859572 kB
...
Shmem:           1062864 kB
...
```



```bash
Buffers + Cached + SwapCached = Active(file) + Inactive(file) + Shmem + SwapCached
```

上述等式两边都是Page Cache

```bash
Page Cache = Buffers + Cached + SwapCached
```

1. ## Page 于Page Cache

Page 是内存管理分配的基本单位，Page Cache是由多个Page所构成的。Page在操作系统中通常都是为4KB大小，Page Cache的大小为4KB的整数倍。

另外一方面，并不是所有的Page都被认为是Page Cache

Linux系统中可供访问的两种内存：

- File-backed pages:文件备份页也就是Page Cache中的Page，对应于磁盘上的若干数据块；对于这些页最大的问题是脏数据回盘；
- Anonymous pages: 不对应磁盘上的任何数据块，也就是晋城运行时的内存空间(例如：方法栈、局部变量表等属性)

为啥不直接将Page Cache称为block cache

1. 从磁盘中加载的数据，不仅仅放在Page Cache中，还会放在Bufer cache。
2. 例如：通过Direct I/O技术的磁盘文件就可以不进入Page Cache中。
3. 历史设计原因，随着linux的演进也逐渐不同



对比一下file-backed pages和Anonymous pages在swap机制下的性能

内存是一种珍惜资源，当内存不够用的时候，内存管理单元MMU(memory Managment Unit)需要提供调度算法回收相关的内存空间。内存空间挥手的方式通常就是swap机制，将内存中的数据交换到持久化设设备上的过程。

File-backed pages(Page Cache)的回收代价比较低，因为：

- Page Cache通常对应于一个文件上的若干顺序块，因此我们在进行持久化落盘可以通过顺序I/O的方式进行落盘。
- 如果Page Cache上没有进行任何的写操作(也就是没有任何的脏页)，甚至Linux系统不会将Page Cache进行回盘，想要获取新的内容完全可以通过再次读取磁盘中的文件即可。

Page Cahe主要难点在于脏页回盘（后面进行说明）

Anonymous pages内存回收代价是比较高的。这时因为Anonymous pages通常随机地写入持久化设备。另外一方面，无论是否有写操作，为了确保数据不丢失，Anonymous pages在进行swap时必须持久化到磁盘上。



### swap与缺页中断

swap机制是指物理内存不够用的时候，内存管理单元MMU(memory Managment Unit)需要提供调度算法来回收相关的内存空间，然后将清理出来的内存空间给当前的内存申请方。

swap机制存在的原因是Linux系统提供了虚拟内存管理机制，每个一个进程都认为其独占内存空间，因此所有的进程的内存空间之和远远大于物理内存，所有进程的内存空间之和超过物理内存的分部分需要交换到磁盘上。

操作系统以page为单位管理内存，当进程发现需要访问的数据不再内存时，操作系统可能会将数据以页的方式加载到内存中。上述的整个过程被称之为缺页中断，当操作系统发生缺页中断时，就会通过系统调用将page再次读取到内存中。

但主内存的空间是有限的，当主内存中不包含可以使用的空间时，操作系统会从内存中选择合适的内存页驱逐回磁盘，为新的内存页让出位置，选择待驱逐页的过程在操作系统中叫做页面替换(page replacement)，替换操作又会触发swap机制。

如果物理内存足够大，那么可能不需要 Swap 机制，但是 Swap 在这种情况下还是有一定优势：对于有发生内存泄漏几率的应用程序（进程），Swap 交换分区更是重要，这可以确保内存泄露不至于导致物理内存不够用，最终导致系统崩溃。但内存泄露会引起频繁的 swap，此时非常影响操作系统的性能。

Linux可以通过swappiness参数控制swap机制，范围[0-100]，实现控制swap的优先级：

- 高数值：较高频率的swap，在进程不活跃时主动将其转换出物理内存
- 低数值：较低频率的swap，这可以确保交互式不因为内存空间频繁的=地交换到磁盘而提高响应延迟。

为什么buffers也是Page Cache的一部分

这是因为当匿名页（Inactive(anon) 以及 Active(anon)）先被交换（swap out）到磁盘上后，然后再加载回（swap in）内存中，由于读入到内存后原来的 Swap File 还在，所以 SwapCached 也可以认为是 File-backed page，即属于 Page Cache。这个过程如 Figure 2 所示。

![img](image/-16313515993493.jpeg)



老一点的系统

```bash
~ free -m
             total       used       free     shared    buffers     cached
Mem:        128956      96440      32515          0       5368      39900
-/+ buffers/cache:      51172      77784
Swap:        16002          0      16001
```

Cached 表示当前的页缓存(Page Cache)占用量，buffers表示的是当前块缓存(buffer Cache)占用量。

Page Cache ： 用于缓存文件的页数据

buffer Cache： 用于缓存块设备如磁盘的块数据

新系统

```bash
andrew@andrew-G3-3590:~$ free -m
              总计         已用        空闲      共享     缓冲/缓存        可用
内存：        7789        3190         627      1111       3970        3212
交换：        2047           1        2046
```

通过上述描述，我们可以 知道Page Cache与文件系统同级别的，buffer Cache是块物理上的概念，因此buffer Cache是与块设备驱动程序是同级别的。

Page Cache与buffer Cache共同的目的就是为了加速数据I/O: 写数据时首先要写到缓存，将写入的数据标记为dirty，想外部存储flash(简称回盘)。如果读取数据我们会先去缓存中读取，如果在缓存中未命中，我们再去外部存储中去读取，并将读取出来的数据加入到缓存中，并且操作系统总是积极的将所有的空闲内存都用作Page Cache和buffer Cache，当内存不够用时也会使用LRU等算法淘汰缓存。

Linux2.4之前，Page Cache与buffer Cache是完全分离的，但是块设备通常是磁盘，磁盘上的数据又大多通过文件系统来组织，这种设计方式会导致很多数据被缓存了两次。在LInux系统2.5版本之后，系统将两个快近似的融合在了一起，如果一个文件的页加载到了Page Cache，那么同时Buffer Cache只需要维护块指向页的指针就可以了。只有那些没有文件表示的块，或者绕过文件系统直接操作(dd命令)的块，才会真正的放到Buffer Cache里。后文所说的Page Cache基本上是Page Cache与buffer Cache的统称。

![img](image/-16313515993505.png)

### Page Cache的预读

操作系统会为基于Page Cache的读缓存机制提供预读机制(PAGE_READAHEAD)

举个例子：用户想通过系统调用获取磁盘上文件的一段数据

- 用户线程仅仅想读取磁盘上对应文件的0-3KB范围内的数据，由于磁盘的基本读写单位是block(4KB)，于是操作系统会至少读取4KB的内容，这4KB刚好能够在一个Page中装下。
- 由于操作系统出于局部最优原理，会将磁盘块offset[4,8],[9,12]以及[1316]都加载到内存

经过上述操作，我们只是想通过read读取4KB的内容，但是系统预读取了16KB的内容。

![img](image/-16313515993507.png)

## 2. Page Cache与文件持久化的一致性&可靠性

现在LInux的Page Cache正如其名，是对磁盘上Page的内存缓存，同时可以用于读写操作。任何系统引入缓存，就会引发一致性问题：内存中的数据与磁盘中的数据不一致，例如后端常见的Redis缓存和MySQL数据库的一致性问题

吞吐量与数据一致性是一对不可调和的矛盾(硬件一致的情况下)

### **什么是文件**

文件其实是数据和元数据的组合，文件=数据+元数据。

元数据：用来描述文件的各种属性，元数据也必须存储在磁盘上

因此，我们在保证数据一致性的时候其实包含了两个方面：数据一致+元数据一致

```bash
# drwxr-xr-x  5 andrew andrew  4096 4月  24 16:43 snap
# 元数据包含的内容
# 1. 文件的大小
# 2. 创建时间
# 3. 访问的时间
# 4. 属主和属组等信息
# 5. 属主权限信息
```

### 文件一致性

发生写操作的时候，我们会把对应的数据写到Page Cache中，如果此时数据还没有刷新到磁盘中，那么内存中的数据就领先于磁盘，此时Page就被称为Dirty page。

当前的lInux有两种方式实现文件一致性：

1. 写穿(write through): 内核向用户提供一种接口，通过该接口操作文件可以保证文件一致性
2. 写回(write back): 系统提供定时任务，该任务周期性同步文件系统脏数据，这时Linux的默认同步方案。

| 方法              | 含义                                                         |
| ----------------- | ------------------------------------------------------------ |
| fsync(int fd)     | 将fd代表的文件的脏数据和脏元数据刷新到磁盘中                 |
| fdatasync(int fd) | 将fd代表的文件的脏数据刷新到磁盘，同时对必要的元数据刷新到磁盘，必要信息是指对文件有关键作用的信息。文件大小、文件修改时间就不属于必要信息了。 |
| sync()            | 对系统中的所有脏数据和脏元数据刷新到磁盘中                   |

## 3. Page Cache的优势与劣势

### 优势

1. 加快对数据的访问 
2. 减少磁盘I/O的访问次数，提高系统磁盘寿命
3. 减少对磁盘I/O的访问，提高系统磁盘I/O吞吐量(Page Cache的预读机制)

### 劣势

1. 使用额外的物理内存空间，当物理内存比较紧俏的时候，可能会导致频繁的swap操作，最终会导致系统的磁盘I/O负载上升。
2. Page Cache没有给应用层提供一个很好的API。导致应用层想要优化Page Cache的使用策略很难。因此一些应用实现了自己的Page管理，比如MySQL的InnoDB存储引擎以16KB的页进行管理。
3. 在某些应用场景下，比如我们每次打开文件只需要读取或者写入几个字节的情况，会比Direct I/O多一些磁盘的读取于写入。

## 参考：

《Linux内核详解》

## 视频教程

[Linux I/O - Page Cache详解(一)](https://www.bilibili.com/video/BV1YP4y1Y7dS?share_source=copy_web)

[Linux I/O - Page Cache详解(二)](https://www.bilibili.com/video/BV1kg411F7ZP?spm_id_from=333.999.0.0)

[Linux I/O - Page Cache详解(三)](https://www.bilibili.com/video/BV1BM4y1G7qP?spm_id_from=333.999.0.0)

[Linux I/O - Page Cache详解(四)](https://www.bilibili.com/video/bv1CA411F7vW)







