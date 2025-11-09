#ifndef KPO2_CATEGORYFACADE_H
#define KPO2_CATEGORYFACADE_H

#include <iostream>
#include "../DomainFactory.h"
#include "../../repository/IRepository.h"
#include "../../domain/Operation.h"
#include "../../utils/Helpers.h"

// Паттерн "Фасад"
class CategoryFacade {
private:
    DomainFactory& domain_factory;
    IRepository<Category>& repository;
    IRepository<Operation>& operation_repo;
    IValidator& validator;
    std::string last_error;

public:
    CategoryFacade(DomainFactory& factory, IRepository<Category>& repo,
                   IRepository<Operation>& op_repo, IValidator& val)
            : domain_factory(factory), repository(repo),
              operation_repo(op_repo), validator(val) {}

    const std::string& getLastError() const { return last_error; }

    Status createCategory(OperationType type, std::string name) {
        try {
            repository.add(std::make_shared<Category>(
                    domain_factory.createCategory(type, std::move(name))));
            return Status::Success;
        } catch (std::exception& e) {
            last_error = e.what();
            return Status::Error;
        }
    }

    Status editCategory(int id, OperationType new_type, std::string new_name) {
        ValidationContext ctx;
        ctx.setStrategy(std::make_unique<CategoryValidationStrategy>(validator, new_name));

        if (ctx.execute() == Status::Error) {
            last_error = validator.getLastError();
            return Status::Error;
        }

        auto category = repository.findById(id);
        if (!category) {
            last_error = "Category not found";
            return Status::Error;
        }

        category->update(new_type, std::move(new_name));
        return Status::Success;
    }

    Status deleteCategory(int id) {
        auto category = repository.findById(id);
        if (!category) {
            last_error = "Category not found";
            return Status::Error;
        }

        for (auto& [op_id, op] : operation_repo.getAll()) {
            if (op && op->getCategoryId() == id) {
                op->update(op->getType(), op->getBankAccountId(),
                           op->getAmount(), UNCATEGORIZED_ID,
                           op->getDate(), op->getDescription());
            }
        }

        return repository.removeById(id);
    }

    void printAllCategories() const {
        std::cout << "=== Categories ===\n";
        for (const auto& [key, category] : repository.getAll()) {
            if (!category) continue;
            std::cout << "ID: " << category->getId()
                      << ", Type: " << (category->getType() == OperationType::Expense ? "Expense" : "Income")
                      << ", Name: " << category->getName() << "\n";
        }
    }
};

#endif //KPO2_CATEGORYFACADE_H
