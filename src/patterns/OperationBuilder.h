#ifndef KPO2_OPERATIONBUILDER_H
#define KPO2_OPERATIONBUILDER_H

#include "../domain/IValidator.h"
#include "../utils/Helpers.h"
#include "../domain/Operation.h"
#include "ValidationContext.h"
#include "OperationValidationStrategy.h"

// Паттерн "Строитель"
class OperationBuilder {
private:
    IValidator& validator;
    OperationType type;
    int bank_account_id = 0;
    double amount = 0;
    int category_id = UNCATEGORIZED_ID;
    std::string date;
    std::string description;

public:
    OperationBuilder(IValidator& val, OperationType type_) : validator(val), type(type_) {}

    OperationBuilder& setBankAccount(int id) { bank_account_id = id; return *this; }
    OperationBuilder& setAmount(double a) { amount = a; return *this; }
    OperationBuilder& setCategory(int id) { category_id = id; return *this; }
    OperationBuilder& setDate(const std::string& d) { date = d; return *this; }
    OperationBuilder& setDescription(const std::string& desc) { description = desc; return *this; }

    Operation build(int assignedId) {
        if (date.empty()) date = getCurrentDateTime();

        ValidationContext ctx;
        ctx.setStrategy(std::make_unique<OperationValidationStrategy>(
                validator, type, bank_account_id, amount, category_id, date));

        if (ctx.execute() == Status::Error)
            throw std::runtime_error(validator.getLastError());

        return {assignedId, type, bank_account_id, amount,
                std::move(date), std::move(description), category_id};
    }
};

#endif //KPO2_OPERATIONBUILDER_H
