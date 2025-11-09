#ifndef KPO2_VALIDATOR_H
#define KPO2_VALIDATOR_H

#include "IValidator.h"
#include "BankAccount.h"
#include "Category.h"
#include "../repository/IRepository.h"
#include "../utils/Helpers.h"

class Validator : public IValidator {
private:
    IRepository<BankAccount>& bank_repo;
    IRepository<Category>& category_repo;
    std::string last_error;

    bool validateStringNotEmpty(const std::string& value, const std::string& field_name) {
        if (value.empty()) {
            last_error = field_name + " cannot be empty";
            return false;
        }
        return true;
    }

public:
    Validator(IRepository<BankAccount>& banks, IRepository<Category>& categories)
            : bank_repo(banks), category_repo(categories) {}

    const std::string& getLastError() const override { return last_error; }

    Status validateBankAccount(const std::string& name, double balance) override {
        if (!validateStringNotEmpty(name, "Account name")) return Status::Error;
        if (balance < 0) {
            last_error = "Balance must be >= 0";
            return Status::Error;
        }
        return Status::Success;
    }

    Status validateCategory(const std::string& name) override {
        if (!validateStringNotEmpty(name, "Category name")) return Status::Error;
        return Status::Success;
    }

    Status validateOperation(OperationType type, int bank_account_id, double amount,
                             int category_id, const std::string& date) override {
        if (amount <= 0) {
            last_error = "Operation amount must be positive";
            return Status::Error;
        }

        if (!validateStringNotEmpty(date, "Date")) return Status::Error;

        auto bank_account = bank_repo.findById(bank_account_id);
        if (!bank_account) {
            last_error = "Invalid bank account ID";
            return Status::Error;
        }

        if (category_id != UNCATEGORIZED_ID) {
            auto category = category_repo.findById(category_id);
            if (!category) {
                last_error = "Invalid category ID";
                return Status::Error;
            }

            if (category && category->getType() != type) {
                last_error = "Operation type does not match category type";
                return Status::Error;
            }
        }

        return Status::Success;
    }
};

#endif //KPO2_VALIDATOR_H
