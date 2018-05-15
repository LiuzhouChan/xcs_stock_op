
#ifndef XCS_ACCOUNT_HPP
#define XCS_ACCOUNT_HPP
#include <vector>
#include <cstdint>
#include <string>
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/logger.h>

class account
{
public: 
    account(std::string stock_name, std::shared_ptr<spdlog::logger> logger=spdlog::stdout_color_mt("account"));
    void addMoney(double money){ money_+=money; }
    void order_lots(double price, int64_t amount);
    void order_target_percent(double price, double percent);
    void order_percent(double price, double percent);
    double getMoney() { return money_; }
    int64_t getStockAmount() { return stock_number_; }
    double getValue(double price) { return money_+stock_number_*price; }
private: 
    std::string stock_name_;
    std::shared_ptr<spdlog::logger> logger_;
    double money_;
    int64_t stock_number_;   //lot number of stock
};

#endif // !XCS_ACCOUNT_HPP
