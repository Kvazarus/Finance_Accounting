#ifndef KPO2_BANKACCOUNTFACADE_H
#define KPO2_BANKACCOUNTFACADE_H

#include <vector>
#include <iostream>
#include "../DomainFactory.h"
#include "../../domain/Operation.h"
#include "../../repository/IRepository.h"

// Паттерн "Фасад"
class BankAccountFacade {
private:
    DomainFactory& domain_factory;
    IRepository<BankAccount>& repository;
    IRepository<Operation>& operation_repo;
    IValidator& validator;
    std::string last_error;

public:
    explicit BankAccountFacade(DomainFactory& factory, IRepository<BankAccount>& repo,
                               IRepository<Operation>& op_repo, IValidator& val)
            : domain_factory(factory), repository(repo),
              operation_repo(op_repo), validator(val) {}

    const std::string& getLastError() const { return last_error; }

    Status createBankAccount(std::string name, double balance) {
        try {
            repository.add(std::make_shared<BankAccount>(
                    domain_factory.createBankAccount(std::move(name), balance)));
            return Status::Success;
        } catch (std::exception& e) {
            last_error = e.what();
            return Status::Error;
        }
    }

    Status editBankAccount(int id, std::string new_name, double new_balance) {
        ValidationContext ctx;
        ctx.setStrategy(std::make_unique<BankAccountValidationStrategy>(validator, new_name, new_balance));

        if (ctx.execute() == Status::Error) {
            last_error = validator.getLastError();
            return Status::Error;
        }

        auto bank_account = repository.findById(id);
        if (!bank_account) {
            last_error = "Account not found";
            return Status::Error;
        }

        bank_account->update(std::move(new_name), new_balance);
        return Status::Success;
    }

    Status deleteBankAccount(int id) {
        if (!repository.findById(id)) {
            last_error = "Account not found";
            return Status::Error;
        }

        std::vector<int> to_delete;
        for (auto& [op_id, op] : operation_repo.getAll()) {
            if (op && op->getBankAccountId() == id)
                to_delete.push_back(op_id);
        }
        for (int op_id : to_delete)
            operation_repo.removeById(op_id);

        Status status = repository.removeById(id);

        return status;
    }

    void printAllAccounts() const {
        std::cout << "=== Accounts ===\n";
        for (const auto& [key, bank_account] : repository.getAll()) {
            std::cout << "ID: " << bank_account->getId()
                      << ", Name: " << bank_account->getName()
                      << ", Balance: " << bank_account->getBalance() << "\n";
        }
    }
};

#endif //KPO2_BANKACCOUNTFACADE_H
