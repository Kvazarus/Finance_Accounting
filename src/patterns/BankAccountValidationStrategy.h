#ifndef KPO2_BANKACCOUNTVALIDATIONSTRATEGY_H
#define KPO2_BANKACCOUNTVALIDATIONSTRATEGY_H

#include "IValidationStrategy.h"

class BankAccountValidationStrategy : public IValidationStrategy {
private:
    IValidator& validator;
    std::string name;
    double balance;
public:
    BankAccountValidationStrategy(IValidator& v, std::string n, double b)
            : validator(v), name(std::move(n)), balance(b) {}

    Status validate() override {
        return validator.validateBankAccount(name, balance);
    }
};

#endif //KPO2_BANKACCOUNTVALIDATIONSTRATEGY_H
