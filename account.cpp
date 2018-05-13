
#include "account.hpp"
#include <cmath>
account::account(std::string stock_name, std::shared_ptr<spdlog::logger> logger):
        stock_name_(stock_name),
        logger_(logger),
        money_(0),
        stock_number_(0)
        {}

void account::order_lots(double price, int64_t amount)
{
    if(amount == 0)
    {
        return;
    }
    else if(amount > 0)
    {
        // buy the stock
        double total_price = price * amount * 100;
        if(total_price > money_)
        {
            logger_->warn("The money is not enough for the order buy {}:{} and will be discarded", stock_name_, amount);
        }
        else
        {
            // the money is enough
            money_ -= total_price;
            stock_number_ += amount;
            logger_->info("Buy {} amount:{} the remain money {} the remain stock lot:{}", stock_name_, amount, money_, stock_number_);
        }
    }
    else
    {
        // sell the stock
        amount = -amount;
        if(amount>stock_number_)
        {
            logger_->warn("The stock is not enough for the order sell {}:{} and will be discarded", stock_name_, amount);
        }
        else
        {
            // the stock is enough
            stock_number_ -= amount;
            money_ += price * amount * 100;
            logger_->info("Sell {} amount:{} the remain money{} the remain stock lot:{}",stock_name_, amount, money_, stock_number_);
        }
    }
}

void account::order_target_percent(double price, double percent)
{
    if(percent < 0 || percent > 1)
    {
        logger_->warn("The percent is not in 0 to 1, the order_target_percent will be discarded");
        return;
    }
    double stock_value = price * stock_number_ * 100;
    double total_value = money_ + stock_value;
    double current_position = stock_value / total_value;

    double position_to_adjust = percent - current_position;

    int64_t amount_to_adjust = (int64_t)std::floor(total_value*std::abs(position_to_adjust)/(price *100));
    if(amount_to_adjust == 0)
    {
        logger_->info("The diff is less then a log and this order will be discarded");
        return;
    }

    if(position_to_adjust > 0)  
    {
        // buy more stock
        order_lots(price, amount_to_adjust);
    }
    else
    {
        order_lots(price, -amount_to_adjust);
    }
}

