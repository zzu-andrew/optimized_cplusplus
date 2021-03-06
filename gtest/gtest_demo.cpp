//
// Created by andrew on 2021/5/9.
//
#include <iostream>
#include <exception>
#include <gtest/gtest.h>

struct BankAccount {
    //    余额
    int balance{};

    BankAccount() = default;
    explicit BankAccount(const int balance) : balance(balance) {

    }
    void Deposit(int &&amount);
    bool Withdraw(int &amount);
};

void BankAccount::Deposit(int &&amount) {
    // 因为amount只有int类型大小使用  move没有任何的作用
    balance += amount;
}

bool BankAccount::Withdraw(int &amount) {
    if (amount < balance) {
        balance -= amount;
        return true;
    }

    return false;
}

// 为一组测试指定测试名称
TEST(AccountTest, BankAccountStartsEmpty) {
    BankAccount bankAccount;
    // ASSERT_EQ 和 EXPECT_EQ 的区别是 ASSERT_EQ判断失败会终止程序
    EXPECT_EQ(0, bankAccount.balance) << "bankAccount should be zero";
    ASSERT_EQ(1, 1);
}

namespace {
// 带夹具的测试
// 1. 使用TEST_F
// 2. 要传入测试夹具的类名
// 测试复杂的数据结构，可以使用测试夹具
/*
 * 比如测试性能，需要提前对一些数据进行初始化等
 * */
    class BankAccountTest : public testing::Test {
    public:
        BankAccount *pBankAccount;

        BankAccountTest() {
            pBankAccount = new BankAccount;
        }

        ~BankAccountTest() override {
            delete pBankAccount;
        }
    };
//The first parameter is the name of the test fixture class
    TEST_F(BankAccountTest, BankAccountTestEmpty) {
        EXPECT_EQ(0, pBankAccount->balance);
    }

    TEST_F(BankAccountTest, CanDepositMoney) {
        pBankAccount->Deposit(100);
        EXPECT_EQ(100, pBankAccount->balance);
    }
}

///////////////////////////////////////////////////////////////
namespace {
    class AccountBalance{
    public:
        int initialBalance;
        int withdrawAmount;
        int finalBalance;
        bool success;

        friend std::ostream &operator<<(std::ostream &os, const AccountBalance &balance) {
            os << "initialBalance: " << balance.initialBalance << " withdrawAmount: " << balance.withdrawAmount
               << " finalBalance: " << balance.finalBalance << " success: " << balance.success;
            return os;
        }
    };

    // 继承之后变量也会跟着继承
    class WithdrawAccountTest : public BankAccountTest, public testing::WithParamInterface<AccountBalance> {
    public:
        WithdrawAccountTest() {
            pBankAccount->balance = GetParam().initialBalance;
        }

    };
//     public testing::WithParamInterface<AccountBalance> 自动申请比变量内存并进行
//  11:42 时间戳·
    TEST_P(WithdrawAccountTest, BankAccountStartsEmpty) {
        auto as =GetParam();
        auto success = pBankAccount->Withdraw(as.withdrawAmount);
        EXPECT_EQ(as.finalBalance, pBankAccount->balance);
        EXPECT_EQ(as.success, success);
    }

    // 初始化测试夹具 TEST_P
    INSTANTIATE_TEST_SUITE_P(Default, WithdrawAccountTest, testing::Values(
            AccountBalance{100, 50, 50, true},
            AccountBalance{100, 200, 100, false}
            ));
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

