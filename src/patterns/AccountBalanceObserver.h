#ifndef KPO2_ACCOUNTBALANCEOBSERVER_H
#define KPO2_ACCOUNTBALANCEOBSERVER_H

#include "IOperationObserver.h"
#include "../domain/BankAccount.h"
#include "../repository/IRepository.h"

class AccountBalanceObserver : public IOperationObserver {
private:
    IRepository<BankAccount>& repository;

public:
    explicit AccountBalanceObserver(IRepository<BankAccount>& repo) : repository(repo) {}

    Status onOperationAdded(const Operation& operation) override {
        auto bank_account = repository.findById(operation.getBankAccountId());
        if (!bank_account) return Status::Error;

        double delta = (operation.getType() == OperationType::Income)
                       ? operation.getAmount()
                       : -operation.getAmount();

        bank_account->setBalance(bank_account->getBalance() + delta);
        return Status::Success;
    }

    Status onOperationDeleted(const Operation& operation) override {
        auto bank_account = repository.findById(operation.getBankAccountId());
        if (!bank_account) return Status::Error;

        double delta = (operation.getType() == OperationType::Income)
                       ? -operation.getAmount()
                       : operation.getAmount();

        bank_account->setBalance(bank_account->getBalance() + delta);
        return Status::Success;
    }

    Status onOperationEdited(const Operation& old_op, const Operation& new_op) override {
        auto old_bank_account = repository.findById(old_op.getBankAccountId());
        if (!old_bank_account) return Status::Error;

        double revert_delta = (old_op.getType() == OperationType::Income)
                              ? -old_op.getAmount()
                              : old_op.getAmount();
        old_bank_account->setBalance(old_bank_account->getBalance() + revert_delta);


        auto new_bank_account = repository.findById(new_op.getBankAccountId());
        if (!new_bank_account) return Status::Error;

        double apply_delta = (new_op.getType() == OperationType::Income)
                             ? new_op.getAmount()
                             : -new_op.getAmount();
        new_bank_account->setBalance(new_bank_account->getBalance() + apply_delta);

        return Status::Success;
    }
};

#endif //KPO2_ACCOUNTBALANCEOBSERVER_H
