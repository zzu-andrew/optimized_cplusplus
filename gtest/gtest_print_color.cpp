//
// Created by andrew on 2021/5/17.
//
#include <iostream>
#include <exception>
#include <gtest/gtest.h>
#include <cstring>


// A simple string class.
class MyString {
private:
    const char* c_string_;


public:
    const MyString& operator=(const MyString& rhs) = delete;
    // Clones a 0-terminated C string, allocating memory using new.
    static const char* CloneCString(const char* a_c_string);

    ////////////////////////////////////////////////////////////
    //
    // C'tors

    // The default c'tor constructs a NULL string.
    MyString() : c_string_(nullptr) {}

    // Constructs a MyString by cloning a 0-terminated C string.
    explicit MyString(const char* a_c_string) : c_string_(nullptr) {
        Set(a_c_string);
    }

    // Copy c'tor
    MyString(const MyString& string) : c_string_(nullptr) {
        Set(string.c_string_);
    }

    ////////////////////////////////////////////////////////////
    //
    // D'tor.  MyString is intended to be a final class, so the d'tor
    // doesn't need to be virtual.
    ~MyString() { delete[] c_string_; }

    // Gets the 0-terminated C string this MyString object represents.
    const char* c_string() const { return c_string_; }

    size_t Length() const { return c_string_ == nullptr ? 0 : strlen(c_string_); }

    // Sets the 0-terminated C string this MyString object represents.
    void Set(const char* c_string);
};

const char *MyString::CloneCString(const char *a_c_string) {
    return nullptr;
}

void MyString::Set(const char *c_string) {

}


TEST(MyString, DefaultConstruct) {
    const MyString s;

    EXPECT_EQ(nullptr, s.c_string());

}

// Queue is a simple queue implemented as a singled-linked list.
//
// The element type must support copy constructor.
template <typename E>  // E is the element type
class Queue;

// QueueNode is a node in a Queue, which consists of an element of
// type E and a pointer to the next node.
template <typename E>  // E is the element type
class QueueNode {
    friend class Queue<E>;

public:
    // Gets the element in this node.
    const E& element() const { return element_; }

    // Gets the next node in the queue.
    QueueNode* next() { return next_; }
    const QueueNode* next() const { return next_; }
    QueueNode(const QueueNode&) = delete;

private:
    // Creates a node with a given element value.  The next pointer is
    // set to NULL.
    explicit QueueNode(const E& an_element)
            : element_(an_element), next_(nullptr) {}

    // We disable the default assignment operator and copy c'tor.
    const QueueNode& operator = (const QueueNode&);

    E element_;
    QueueNode* next_;
};

template <typename E>  // E is the element type.
class Queue {
public:
    // Creates an empty queue.
    Queue() : head_(nullptr), last_(nullptr), size_(0) {}

    // D'tor.  Clears the queue.
    ~Queue() { Clear(); }
    Queue(const Queue&) = delete;

    // Clears the queue.
    void Clear() {
        if (size_ > 0) {
            // 1. Deletes every node.
            QueueNode<E>* node = head_;
            QueueNode<E>* next = node->next();
            for (; ;) {
                delete node;
                node = next;
                if (node == nullptr) break;
                next = node->next();
            }

            // 2. Resets the member variables.
            head_ = last_ = nullptr;
            size_ = 0;
        }
    }

    // Gets the number of elements.
    size_t Size() const { return size_; }

    // Gets the first element of the queue, or NULL if the queue is empty.
    QueueNode<E>* Head() { return head_; }
    const QueueNode<E>* Head() const { return head_; }

    // Gets the last element of the queue, or NULL if the queue is empty.
    QueueNode<E>* Last() { return last_; }
    const QueueNode<E>* Last() const { return last_; }

    // Adds an element to the end of the queue.  A copy of the element is
    // created using the copy constructor, and then stored in the queue.
    // Changes made to the element in the queue doesn't affect the source
    // object, and vice versa.
    void Enqueue(const E& element) {
        auto* new_node = new QueueNode<E>(element);

        if (size_ == 0) {
            head_ = last_ = new_node;
            size_ = 1;
        } else {
            last_->next_ = new_node;
            last_ = new_node;
            size_++;
        }
    }

    // Removes the head of the queue and returns it.  Returns NULL if
    // the queue is empty.
    E* Dequeue() {
        if (size_ == 0) {
            return nullptr;
        }

        const QueueNode<E>* const old_head = head_;
        head_ = head_->next_;
        size_--;
        if (size_ == 0) {
            last_ = nullptr;
        }

        E* element = new E(old_head->element());
        delete old_head;

        return element;
    }

    // Applies a function/functor on each element of the queue, and
    // returns the result in a new queue.  The original queue is not
    // affected.
    // 将老的queue 的element的2倍形成一个新的queue
    template <typename F>
    Queue* Map(F function) const {
        auto* new_queue = new Queue();
        for (const QueueNode<E>* node = head_; node != nullptr;
             node = node->next_) {
            new_queue->Enqueue(function(node->element()));
        }

        return new_queue;
    }

private:
    QueueNode<E>* head_;  // The first node of the queue.
    QueueNode<E>* last_;  // The last node of the queue.
    size_t size_;  // The number of elements in the queue.

    // We disallow copying a queue.

    const Queue& operator = (const Queue&);
};


namespace {
class QueueTestSmpl : public testing::Test{
protected:
    // 定义保护类型的变量和函数，这样子类中也能访问
    void SetUp() override {
        q1_.Enqueue(1);
        q2_.Enqueue(2);
        q2_.Enqueue(3);
    }

    // virtual void TearDown() will be called after each test is run.

    static int Double(int n) {
        return 2 * n;
    }

    // A helper function for testing Queue::Map
    static void MapTester(const Queue<int> *q) {
        const Queue<int> * const new_q = q->Map(Double);

        // two queue has the same size as q
        ASSERT_EQ(q->Size(), new_q->Size());

        for (const QueueNode<int> *n1 = q->Head(), *n2 = new_q->Head();
            n1 != nullptr; n1 = n1->next(), n2 = n2->next()) {
            EXPECT_EQ(2 * n1->element(), n2->element());
        }
        delete new_q;
    }

    Queue<int> q0_;
    Queue<int> q1_;
    Queue<int> q2_;
};

// When you have a test fixture, you define a test using TEST_F
// instead of TEST.
// 特定就是能在 测试范围内使用类中的变量
TEST_F(QueueTestSmpl, Dequeue) {
    EXPECT_EQ(0, q0_.Size());
}

TEST_F(QueueTestSmpl, Map) {
    MapTester(&q0_);
    MapTester(&q1_);
    MapTester(&q2_);
}

}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

