#ifndef KPO2_OPERATIONFACADE_H
#define KPO2_OPERATIONFACADE_H

#include <optional>
#include <iostream>
#include "../OperationNotifier.h"
#include "../DomainFactory.h"
#include "../../repository/IRepository.h"
#include "../../utils/Helpers.h"
#include "../OperationValidationStrategy.h"
#include "../OperationBuilder.h"

// Паттерн "Фасад"
class OperationFacade {
private:
    IRepository<Operation>& repository;
    OperationNotifier& notifier;
    DomainFactory& factory;
    IValidator& validator;
    std::string last_error;

public:
    OperationFacade(IRepository<Operation>& repo, OperationNotifier& notif,
                    DomainFactory& fact, IValidator& val)
            :  repository(repo), notifier(notif), factory(fact), validator(val) {}

    const std::string& getLastError() const { return last_error; }

    Status createOperation(OperationBuilder& builder, int* operation_id = nullptr) {
        try {
            int tempId = factory.peekOperationId();
            auto operation = builder.build(tempId);
            factory.commitOperationId();
            repository.add(std::make_shared<Operation>(operation));
            if (operation_id)
                *operation_id = operation.getId();
            notifier.notifyAdded(operation);
            return Status::Success;
        } catch (std::exception& e) {
            last_error = e.what();
            return Status::Error;
        }
    }

    Status addOperation(const Operation& operation) {
        try {
            repository.add(std::make_shared<Operation>(operation));
            notifier.notifyAdded(operation);
            return Status::Success;
        } catch (std::exception& e) {
            last_error = e.what();
            return Status::Error;
        }
    }

    Status editOperation(int id, OperationType new_type, int new_bank_account_id,
                         double new_amount, int new_category_id = UNCATEGORIZED_ID,
                         std::string new_date = "", std::string new_description = "") {
        if (new_date.empty()) new_date = getCurrentDateTime();

        ValidationContext ctx;
        ctx.setStrategy(std::make_unique<OperationValidationStrategy>(
                validator, new_type, new_bank_account_id, new_amount,
                new_category_id, new_date));

        if (ctx.execute() == Status::Error) {
            last_error = validator.getLastError();
            return Status::Error;
        }

        auto operation = repository.findById(id);
        if (!operation) {
            last_error = "Operation not found";
            return Status::Error;
        }

        Operation old_operation = *operation;

        operation->update(new_type, new_bank_account_id, new_amount,
                          new_category_id, std::move(new_date),
                          std::move(new_description));

        notifier.notifyEdited(old_operation, *operation);

        return Status::Success;
    }

    Status editOperation(const Operation& new_operation, std::optional<Operation>* old_state = nullptr) {
        auto operation = repository.findById(new_operation.getId());
        if (!operation) {
            last_error = "Operation not found";
            return Status::Error;
        }

        ValidationContext ctx;
        ctx.setStrategy(std::make_unique<OperationValidationStrategy>(
                validator, new_operation.getType(),
                new_operation.getBankAccountId(),
                new_operation.getAmount(),
                new_operation.getCategoryId(),
                new_operation.getDate()));

        if (ctx.execute() == Status::Error) {
            last_error = validator.getLastError();
            return Status::Error;
        }

        Operation old_operation = *operation;
        operation->update(
                new_operation.getType(),
                new_operation.getBankAccountId(),
                new_operation.getAmount(),
                new_operation.getCategoryId(),
                new_operation.getDate(),
                new_operation.getDescription()
        );

        if (old_state)
            *old_state = old_operation;
        notifier.notifyEdited(old_operation, *operation);
        return Status::Success;
    }


    Status deleteOperation(int id, std::optional<Operation>* old_state = nullptr) {
        auto operation = repository.findById(id);
        if (!operation) {
            last_error = "Operation not found";
            return Status::Error;
        }

        Operation old_operation = *operation;
        repository.removeById(id);
        if (old_state)
            *old_state = old_operation;
        notifier.notifyDeleted(old_operation);
        return Status::Success;
    }

    void printAllOperations() const {
        std::cout << "=== Operations ===\n";
        for (const auto& [key, operation] : repository.getAll()) {
            std::cout << "ID: " << operation->getId()
                      << ", Type: " << (operation->getType() == OperationType::Expense ? "Expense" : "Income")
                      << ", Amount: " << operation->getAmount()
                      << ", Account ID: " << operation->getBankAccountId()
                      << ", Category ID: " << operation->getCategoryId()
                      << ", Date: " << operation->getDate()
                      << ", Desc: " << operation->getDescription() << "\n";
        }
    }
};

#endif //KPO2_OPERATIONFACADE_H
