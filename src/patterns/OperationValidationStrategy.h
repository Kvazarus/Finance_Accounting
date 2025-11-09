#ifndef KPO2_OPERATIONVALIDATIONSTRATEGY_H
#define KPO2_OPERATIONVALIDATIONSTRATEGY_H

#include "IValidationStrategy.h"

class OperationValidationStrategy : public IValidationStrategy {
private:
    IValidator& validator;
    OperationType type;
    int bank_account_id;
    double amount;
    int category_id;
    std::string date;
public:
    OperationValidationStrategy(IValidator& v, OperationType t, int bank_id, double amt, int cat_id, std::string d)
            : validator(v), type(t), bank_account_id(bank_id), amount(amt), category_id(cat_id), date(std::move(d)) {}

    Status validate() override {
        return validator.validateOperation(type, bank_account_id, amount, category_id, date);
    }
};

#endif //KPO2_OPERATIONVALIDATIONSTRATEGY_H
