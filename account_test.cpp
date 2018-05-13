#include <gtest/gtest.h>
#include "account.hpp"

TEST(TestAccount, order_lots)
{
    account acc("Test_order_lots",spdlog::stdout_color_mt("Test_order_lots"));
    acc.addMoney(10000);
    acc.order_lots(1,1000);
    ASSERT_FLOAT_EQ(acc.getMoney(),10000);
    acc.order_lots(1, 1);
    ASSERT_FLOAT_EQ(acc.getMoney(),9900);
    ASSERT_EQ(acc.getStockAmount(),1);
    acc.order_lots(10,1);
    ASSERT_FLOAT_EQ(acc.getMoney(),8900);
    ASSERT_EQ(acc.getStockAmount(), 2);
}

TEST(TestAccount, order_target_percent)
{
    account acc("Test_order_target_percent",spdlog::stdout_color_mt("Test_order_target_percent"));
    acc.addMoney(10000);

    acc.order_target_percent(1,0.1);
    ASSERT_EQ(acc.getStockAmount(),10);
    ASSERT_FLOAT_EQ(acc.getMoney(),9000);

    acc.order_target_percent(1,1.2);
    ASSERT_EQ(acc.getStockAmount(),10);
    ASSERT_FLOAT_EQ(acc.getMoney(),9000);

    acc.order_target_percent(1,-0.3);
    ASSERT_EQ(acc.getStockAmount(),10);
    ASSERT_FLOAT_EQ(acc.getMoney(),9000);

    acc.order_target_percent(10,0.5);
    ASSERT_EQ(acc.getStockAmount(),10);
    ASSERT_FLOAT_EQ(acc.getMoney(),9000);

    acc.order_target_percent(10,1);
    ASSERT_EQ(acc.getStockAmount(),19);
    ASSERT_FLOAT_EQ(acc.getMoney(),0);

    acc.order_target_percent(9,0);
    ASSERT_EQ(acc.getStockAmount(),0);
    ASSERT_FLOAT_EQ(acc.getMoney(),17100);

    acc.order_target_percent(1,0.01);
    ASSERT_EQ(acc.getStockAmount(),1);
    ASSERT_FLOAT_EQ(acc.getMoney(),17000);

    acc.order_target_percent(1,0);

    acc.order_target_percent(1,0.02);
    ASSERT_EQ(acc.getStockAmount(),3);
    ASSERT_FLOAT_EQ(acc.getMoney(),16800);
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

