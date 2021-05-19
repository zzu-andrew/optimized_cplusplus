## `gtest`



## 断言

### `ASSERT_*`系列的断言

当检查失败时，退出当前函数

### `EXPECT_*`系列的断言

当检查失败时，继续向下执行



布尔值的检查

| **Fatal assertion**            | **Nonfatal assertion**         | **Verifies**         |
| ------------------------------ | ------------------------------ | -------------------- |
| `ASSERT_TRUE(`*condition*`)`;  | `EXPECT_TRUE(`*condition*`)`;  | *condition* is true  |
| `ASSERT_FALSE(`*condition*`)`; | `EXPECT_FALSE(`*condition*`)`; | *condition* is false |

数值型数据检查

| **Fatal assertion**                    | **Nonfatal assertion**                 | **Verifies**             |
| -------------------------------------- | -------------------------------------- | ------------------------ |
| `ASSERT_EQ(`*expected*`, `*actual*`);` | `EXPECT_EQ(`*expected*`, `*actual*`);` | *expected* `==` *actual* |
| `ASSERT_NE(`*val1*`, `*val2*);`        | `EXPECT_NE(`*val1*`, `*val2*`);`       | *val1* `!=` *val2*       |
| `ASSERT_LT(`*val1*`, `*val2*`);`       | `EXPECT_LT(`*val1*`, `*val2*`);`       | *val1* `<` *val2*        |
| `ASSERT_LE(`*val1*`, `*val2*`);`       | `EXPECT_LE(`*val1*`, `*val2*`);`       | *val1* `<=` *val2*       |
| `ASSERT_GT(`*val1*`, `*val2*`);`       | `EXPECT_GT(`*val1*`, `*val2*`);`       | *val1* `>` *val2*        |
| `ASSERT_GE(`*val1*`, `*val2*`);`       | `EXPECT_GE(`*val1*`, `*val2*`);`       | *val1* `>=` *val2*       |

字符串检查

| **Fatal assertion**                                   | **Nonfatal assertion**                                | **Verifies**                                            |
| ----------------------------------------------------- | ----------------------------------------------------- | ------------------------------------------------------- |
| `ASSERT_STREQ(`*expected_str*`, `*actual_str*`);`     | `EXPECT_STREQ(`*expected_str*`, `*actual_str*`);`     | the two C strings have the same content                 |
| `ASSERT_STRNE(`*str1*`, `*str2*`);`                   | `EXPECT_STRNE(`*str1*`, `*str2*`);`                   | the two C strings have different content                |
| `ASSERT_STRCASEEQ(`*expected_str*`, `*actual_str*`);` | `EXPECT_STRCASEEQ(`*expected_str*`, `*actual_str*`);` | the two C strings have the same content, ignoring case  |
| `ASSERT_STRCASENE(`*str1*`, `*str2*`);`               | `EXPECT_STRCASENE(`*str1*`, `*str2*`);`               | the two C strings have different content, ignoring case |

返回成功和失败

直接返回成功： SUCCEED()

返回失败：

| Fatal assertion | Nonfatal assertion |
| --------------- | ------------------ |
| FAIL()          | ADD_FAILURE()      |

 异常检查

| **Fatal assertion**                              | **Nonfatal assertion**                           | **Verifies**                                      |
| ------------------------------------------------ | ------------------------------------------------ | ------------------------------------------------- |
| `ASSERT_THROW(`*statement*, *exception_type*`);` | `EXPECT_THROW(`*statement*, *exception_type*`);` | *statement* throws an exception of the given type |
| `ASSERT_ANY_THROW(`*statement*`);`               | `EXPECT_ANY_THROW(`*statement*`);`               | *statement* throws an exception of any type       |
| `ASSERT_NO_THROW(`*statement*`);`                | `EXPECT_NO_THROW(`*statement*`);`                | *statement* doesn't throw any exception           |

输入参数的检查

失败的时候回返回详细的信息，包括哪个参数的错误

| **Fatal assertion**                    | **Nonfatal assertion**                 | **Verifies**                     |
| -------------------------------------- | -------------------------------------- | -------------------------------- |
| `ASSERT_PRED1(`*pred1, val1*`);`       | `EXPECT_PRED1(`*pred1, val1*`);`       | *pred1(val1)* returns true       |
| `ASSERT_PRED2(`*pred2, val1, val2*`);` | `EXPECT_PRED2(`*pred2, val1, val2*`);` | *pred2(val1, val2)* returns true |
| ...                                    | ...                                    | ...                              |

```cpp
TEST(MultiPrime, Demo)
{
    int m = 5, n = 6;
    EXPECT_PRED2(MultiPrime, m, n);
}
```

如果还是嫌输出的信息不够详细可以使用如下自定义格式的数据

| **Fatal assertion**                                  | **Nonfatal assertion**                             | **Verifies**                             |
| ---------------------------------------------------- | -------------------------------------------------- | ---------------------------------------- |
| `ASSERT_PRED_FORMAT1(`*pred_format1, val1*);`        | `EXPECT_PRED_FORMAT1(`*pred_format1, val1*);       | *pred_format1(val1)* is successful       |
| `ASSERT_PRED_FORMAT2(`*pred_format2, val1, val2*`);` | `EXPECT_PRED_FORMAT2(`*pred_format2, val1, val2*); | *pred_format2(val1, val2)* is successful |

```cpp
testing::AssertionResult AssertFoo(const char* m_expr, const char* n_expr, const char* k_expr, int m, int n, int k) {
    if (Foo(m, n) == k)
        return testing::AssertionSuccess();
    testing::Message msg;
    msg << m_expr << " 和 " << n_expr << " 的最大公约数应该是：" << Foo(m, n) << " 而不是：" << k_expr;
    return testing::AssertionFailure(msg);
}

TEST(AssertFooTest, HandleFail)
{
    EXPECT_PRED_FORMAT3(AssertFoo, 3, 6, 2);
}
// error: 3 和 6 的最大公约数应该是：3 而不是：2
```

浮点数检查

| **Fatal assertion**                       | **Nonfatal assertion**                    | **Verifies**                             |
| ----------------------------------------- | ----------------------------------------- | ---------------------------------------- |
| `ASSERT_FLOAT_EQ(`*expected, actual*`);`  | `EXPECT_FLOAT_EQ(`*expected, actual*`);`  | the two `float` values are almost equal  |
| `ASSERT_DOUBLE_EQ(`*expected, actual*`);` | `EXPECT_DOUBLE_EQ(`*expected, actual*`);` | the two `double` values are almost equal |

对两个非常相近的数检查

| **Fatal assertion**                       | **Nonfatal assertion**                    | **Verifies**                                                 |
| ----------------------------------------- | ----------------------------------------- | ------------------------------------------------------------ |
| `ASSERT_NEAR(`*val1, val2, abs_error*`);` | `EXPECT_NEAR`*(val1, val2, abs_error*`);` | the difference between *val1* and *val2* doesn't exceed the given absolute error |

类型检查，类型检查失败之后会导致编译失败

```cpp
template <typename T> class FooType {
public:
    void Bar() { testing::StaticAssertTypeEq<int, T>(); }
};

TEST(TypeAssertionTest, Demo)
{
    FooType<bool> fooType;
    fooType.Bar();
}
```



