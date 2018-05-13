#include <gtest/gtest.h>
#include "account.hpp"

class TestAccount:public testing::Test
{
public: 
    TestAccount():acc(account("gtest_stock"))
    {
    }
    virtual void SetUp()
    {
        acc.addMoney(10000);
    }

    account acc;
};

TEST_F(TestAccount, order_lots)
{
    acc.order_lots(1,1000);
    ASSERT_FLOAT_EQ(acc.getMoney(),10000);
    acc.order_lots(1, 1);
    ASSERT_FLOAT_EQ(acc.getMoney(),9900);
    ASSERT_EQ(acc.getStockAmount(),1);
    acc.order_lots(10,1);
    ASSERT_FLOAT_EQ(acc.getMoney(),8900);
    ASSERT_EQ(acc.getStockAmount(), 2);
}

TEST_F(TestAccount, order_target_percent)
{
    
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

