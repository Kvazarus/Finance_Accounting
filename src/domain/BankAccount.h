#ifndef KPO2_BANKACCOUNT_H
#define KPO2_BANKACCOUNT_H

#include "IEntity.h"
#include <string>

class BankAccount : public IEntity {
private:
    int id;
    std::string name;
    double balance;

public:
    BankAccount(int id_, std::string name_, double balance_) : id(id_), name(std::move(name_)), balance(balance_) {}

    int getId() const override { return id; }
    std::string getName() const { return name; }
    double getBalance() const { return balance; }

    void setBalance(double new_balance) {
        balance = new_balance;
    }

    void update(std::string new_name, double new_balance) {
        name = std::move(new_name);
        balance = new_balance;
    }
};

#endif //KPO2_BANKACCOUNT_H
