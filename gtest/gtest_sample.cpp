//
// Created by andrew on 2021/5/9.
//
#include <iostream>
#include <climits>
#include <gtest/gtest.h>

/*
 * 简答函数测试框架
 * */


// Returns n! (the factorial of n).  For negative n, n! is defined to be 1.
int Factorial(int n) {
    int result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }

    return result;
}

// Returns true if and only if n is a prime number.
bool IsPrime(int n) {
    // Trivial case 1: small numbers
    if (n <= 1) return false;

    // Trivial case 2: even numbers
    if (n % 2 == 0) return n == 2;

    // Now, we have that n is odd and n >= 3.

    // Try to divide n by every odd number i, starting from 3
    for (int i = 3; ; i += 2) {
        // We only have to try i up to the square root of n
        if (i > n/i) break;

        // Now, we have i <= n/i < n.
        // If n is divisible by i, n is not prime.
        if (n % i == 0) return false;
    }

    // n has no integer factor in the range (1, n), and thus is prime.
    return true;
}

// namespace 相当于使用了static  整体static，就算其他文件中使用了同样的命名也不会进行报错
namespace {

// Step 2. Use the TEST macro to define your tests.
//
// TEST has two parameters: the test case name and the test name.
// After using the macro, you should define your test logic between a
// pair of braces.  You can use a bunch of macros to indicate the
// success or failure of a test.  EXPECT_TRUE and EXPECT_EQ are
// examples of such macros.  For a complete list, see gtest.h.
//
// <TechnicalDetails>
//
// In Google Test, tests are grouped into test cases.  This is how we
// keep test code organized.  You should put logically related tests
// into the same test case.
//
// The test case name and the test name should both be valid C++
// identifiers.  And you should not use underscore (_) in the names.
//
// Google Test guarantees that each test you define is run exactly
// once, but it makes no guarantee on the order the tests are
// executed.  Therefore, you should write your tests in such a way
// that their results don't depend on their order.
//
// </TechnicalDetails>


// Tests Factorial().

// Tests factorial of negative numbers.
    TEST(FactorialTest, Negative) {
        // This test is named "Negative", and belongs to the "FactorialTest"
        // test case.
        EXPECT_EQ(1, Factorial(-5));
        EXPECT_EQ(1, Factorial(-1));
        EXPECT_GT(Factorial(-10), 0);

        // <TechnicalDetails>
        //
        // EXPECT_EQ(expected, actual) is the same as
        //
        //   EXPECT_TRUE((expected) == (actual))
        //
        // except that it will print both the expected value and the actual
        // value when the assertion fails.  This is very helpful for
        // debugging.  Therefore in this case EXPECT_EQ is preferred.
        //
        // On the other hand, EXPECT_TRUE accepts any Boolean expression,
        // and is thus more general.
        //
        // </TechnicalDetails>
    }

// Tests factorial of 0.
    TEST(FactorialTest, Zero) {
        EXPECT_EQ(1, Factorial(0));
    }

// Tests factorial of positive numbers.
    TEST(FactorialTest, Positive) {
        // 后面加上对应打印信息，当测试失败的时候就会打印出来
        EXPECT_EQ(4, Factorial(1)) << "expect 4 failed Factorial(1) should equal 1";
        EXPECT_EQ(2, Factorial(2));

        EXPECT_EQ(6, Factorial(3));
        EXPECT_EQ(40320, Factorial(8));
    }


// Tests IsPrime()

// Tests negative input.
    TEST(IsPrimeTest, Negative) {
        // This test belongs to the IsPrimeTest test case.

        EXPECT_FALSE(IsPrime(-1));
        EXPECT_FALSE(IsPrime(-2));
        EXPECT_FALSE(IsPrime(INT_MIN));
    }

// Tests some trivial cases.
    TEST(IsPrimeTest, Trivial) {
        EXPECT_FALSE(IsPrime(0));
        EXPECT_FALSE(IsPrime(1));
        EXPECT_TRUE(IsPrime(2));
        EXPECT_TRUE(IsPrime(3));
    }

// Tests positive input.
    TEST(IsPrimeTest, Positive) {
        EXPECT_FALSE(IsPrime(4));
        EXPECT_TRUE(IsPrime(5));
        EXPECT_FALSE(IsPrime(6));
        EXPECT_TRUE(IsPrime(23));
    }
    // 参数化
    // 添加类没继承自 public::testing::TestWithParam<int>
    class IsPrimeParamTest : public testing::TestWithParam<int>{

    };
    // 多参数测试方法
    TEST_P(IsPrimeParamTest, multiArgs){
        int n = GetParam();
        EXPECT_TRUE(IsPrime(n));
    }
    // 开始传入参数
    // 第三个参数还可以是
    /*
     * Range(Begin, end[ , step])    testing::Range(3, 10, 2)  testing::Values(3, 5, 7, 9)
     * ValuesIn(容器和C数组)
     * Bool() 分别获取true和false
     * combine(g1, g2, g3.....)排列组合
     * */

    INSTANTIATE_TEST_CASE_P(TrueReturn, IsPrimeParamTest, testing::Values(3, 5, 11, 17));
}  // namespace
