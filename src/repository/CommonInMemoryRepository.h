#ifndef KPO2_COMMONINMEMORYREPOSITORY_H
#define KPO2_COMMONINMEMORYREPOSITORY_H

#include "../domain/Operation.h"
#include "../domain/BankAccount.h"
#include "../domain/Category.h"
#include "InMemoryRepository.h"

// Паттерн "Снимок (Memento)"
class CommonInMemoryRepository {
private:
    InMemoryRepository<Operation> operationRepo;
    InMemoryRepository<BankAccount> accountRepo;
    InMemoryRepository<Category> categoryRepo;

public:
    IRepository<Operation>& getOperations() { return operationRepo; }
    IRepository<BankAccount>& getBankAccounts() { return accountRepo; }
    IRepository<Category>& getCategories() { return categoryRepo; }

    // --- Снимок ---
    class Snapshot {
    private:
        CommonInMemoryRepository& repo;
        std::unordered_map<int, std::shared_ptr<Operation>> saved_operations;
        std::unordered_map<int, std::shared_ptr<BankAccount>> saved_accounts;
        std::unordered_map<int, std::shared_ptr<Category>> saved_categories;

    public:
        explicit Snapshot(CommonInMemoryRepository& originator)
                : repo(originator),
                  saved_operations(cloneMap(originator.operationRepo.getAll())),
                  saved_accounts(cloneMap(originator.accountRepo.getAll())),
                  saved_categories(cloneMap(originator.categoryRepo.getAll())) {}

        void restore() {
            restoreRepo(repo.operationRepo, saved_operations);
            restoreRepo(repo.accountRepo, saved_accounts);
            restoreRepo(repo.categoryRepo, saved_categories);
        }

    private:
        template<typename T>
        static std::unordered_map<int, std::shared_ptr<T>> cloneMap(const std::unordered_map<int, std::shared_ptr<T>>& source) {
            std::unordered_map<int, std::shared_ptr<T>> result;
            for (const auto& [id, ptr] : source)
                result[id] = std::make_shared<T>(*ptr);
            return result;
        }

        template<typename T>
        static void restoreRepo(InMemoryRepository<T>& target,
                                const std::unordered_map<int, std::shared_ptr<T>>& backup) {
            target.clear();
            for (const auto& [id, ptr] : backup)
                target.add(std::make_shared<T>(*ptr));
        }
    };

    Snapshot createSnapshot() { return Snapshot(*this); }
};

#endif //KPO2_COMMONINMEMORYREPOSITORY_H
