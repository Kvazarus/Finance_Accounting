#ifndef KPO2_DOMAINFACTORY_H
#define KPO2_DOMAINFACTORY_H

#include "../domain/IValidator.h"
#include "../domain/BankAccount.h"
#include "ValidationContext.h"
#include "BankAccountValidationStrategy.h"
#include "CategoryValidationStrategy.h"
#include "../domain/Category.h"

// Паттерн "Фабрика"
class DomainFactory {
private:
    IValidator& validator;
    int bank_account_counter;
    int category_counter;
    int operation_counter;

public:
    explicit DomainFactory(IValidator& v) : validator(v) {
        bank_account_counter = 1;
        category_counter = 1;
        operation_counter = 1;
    }

    BankAccount createBankAccount(std::string name, double balance) {
        ValidationContext ctx;
        ctx.setStrategy(std::make_unique<BankAccountValidationStrategy>(validator, name, balance));

        if (ctx.execute() == Status::Error)
            throw std::runtime_error(validator.getLastError());

        return {bank_account_counter++, std::move(name), balance};
    }

    Category createCategory(OperationType type, std::string name) {
        ValidationContext ctx;
        ctx.setStrategy(std::make_unique<CategoryValidationStrategy>(validator, name));

        if (ctx.execute() == Status::Error)
            throw std::runtime_error(validator.getLastError());

        return {category_counter++, type, std::move(name)};
    }

    int peekOperationId() const {
        return operation_counter;
    }

    void commitOperationId() {
        operation_counter++;
    }
};

#endif //KPO2_DOMAINFACTORY_H
