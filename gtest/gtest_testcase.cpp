//
// Created by andrew on 2021/5/9.
//
#include <iostream>
#include <gtest/gtest.h>
#include <cstddef>
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
    // We disable the default assignment operator and copy c'tor.
    const QueueNode& operator = (const QueueNode&) = delete;
    QueueNode(const QueueNode&) = delete;
    // Gets the element in this node.
    const E& element() const { return element_; }

    // Gets the next node in the queue.
    QueueNode* next() { return next_; }
    const QueueNode* next() const { return next_; }

private:
    // Creates a node with a given element value.  The next pointer is
    // set to NULL.
    explicit QueueNode(const E& an_element)
            : element_(an_element), next_(nullptr) {}



    E element_;
    QueueNode* next_;
};

template <typename E>  // E is the element type.
class Queue {
public:
    // We disallow copying a queue.
    Queue(const Queue&) = delete;
    const Queue& operator = (const Queue&) = delete;
    // Creates an empty queue.
    Queue() : head_(nullptr), last_(nullptr), size_(0) {}

    // D'tor.  Clears the queue.
    ~Queue() { Clear(); }

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
};
//---------------开始测试的地方-------------------------

/***-------------------------事件 case -------------------------------***/
// TestCase事件： 每个案例执行前后
// 多次对类济南西给你初始化使用测试

namespace {
// To use a test fixture, derive a class from testing::Test.
    class QueueTestSmpl3 : public testing::Test {
    protected:  // You should make the members protected s.t. they can be
        // accessed from sub-classes.

        // virtual void SetUp() will be called before each test is run.  You
        // should define it if you need to initialize the variables.
        // Otherwise, this can be skipped.
        void SetUp() override {
            q1_.Enqueue(1);
            q2_.Enqueue(2);
            q2_.Enqueue(3);
        }

        // virtual void TearDown() will be called after each test is run.
        // You should define it if there is cleanup work to do.  Otherwise,
        // you don't have to provide it.
        //
        // virtual void TearDown() {
        // }

        // A helper function that some test uses.
        static int Double(int n) {
            return 2*n;
        }

        // A helper function for testing Queue::Map().
        static void MapTester(const Queue<int> * q) {
            // Creates a new queue, where each element is twice as big as the
            // corresponding one in q.
            const Queue<int> * const new_q = q->Map(Double);

            // Verifies that the new queue has the same size as q.
            ASSERT_EQ(q->Size(), new_q->Size());

            // Verifies the relationship between the elements of the two queues.
            for (const QueueNode<int>*n1 = q->Head(), *n2 = new_q->Head();
                 n1 != nullptr; n1 = n1->next(), n2 = n2->next()) {
                EXPECT_EQ(2 * n1->element(), n2->element());
            }

            delete new_q;
        }

        // Declares the variables your tests want to use.
        Queue<int> q0_;
        Queue<int> q1_;
        Queue<int> q2_;
    };

// When you have a test fixture, you define a test using TEST_F
// instead of TEST.

// Tests the default c'tor.
    TEST_F(QueueTestSmpl3, DefaultConstructor) {
        // You can access data in the test fixture here.
        EXPECT_EQ(0u, q0_.Size());
    }

// Tests Dequeue().
    TEST_F(QueueTestSmpl3, Dequeue) {
        int * n = q0_.Dequeue();
        EXPECT_TRUE(n == nullptr);

        n = q1_.Dequeue();
        ASSERT_TRUE(n != nullptr);
        EXPECT_EQ(1, *n);
        EXPECT_EQ(0u, q1_.Size());
        delete n;

        n = q2_.Dequeue();
        ASSERT_TRUE(n != nullptr);
        EXPECT_EQ(2, *n);
        EXPECT_EQ(1u, q2_.Size());
        delete n;
    }

// Tests the Queue::Map() function.
    TEST_F(QueueTestSmpl3, Map) {
        MapTester(&q0_);
        MapTester(&q1_);
        MapTester(&q2_);
    }
}  // namespace
