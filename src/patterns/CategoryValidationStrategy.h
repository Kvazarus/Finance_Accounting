#ifndef KPO2_CATEGORYVALIDATIONSTRATEGY_H
#define KPO2_CATEGORYVALIDATIONSTRATEGY_H

#include "../domain/IValidator.h"
#include "IValidationStrategy.h"

class CategoryValidationStrategy : public IValidationStrategy {
private:
    IValidator& validator;
    std::string name;
public:
    CategoryValidationStrategy(IValidator& v, std::string n)
            : validator(v), name(std::move(n)) {}

    Status validate() override {
        return validator.validateCategory(name);
    }
};

#endif //KPO2_CATEGORYVALIDATIONSTRATEGY_H
