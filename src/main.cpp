#include <iostream>
#include <string>
#include <memory>
#include <utility>

#include "boost/di.hpp"
#include "repository/CommonInMemoryRepository.h"
#include "domain/IValidator.h"
#include "patterns/facades/OperationFacade.h"
#include "patterns/facades/CategoryFacade.h"
#include "patterns/facades/BankAccountFacade.h"
#include "domain/Validator.h"
#include "patterns/AccountBalanceObserver.h"
#include "patterns/RepositoryCaretaker.h"
#include "patterns/CommandManager.h"
#include "UI/ConsoleUI.h"
#include "patterns/commands/AddOperationCommand.h"
#include "patterns/commands/TimedCommand.h"
#include "patterns/commands/EditOperationCommand.h"
#include "patterns/commands/DeleteOperationCommand.h"
#include "patterns/commands/SnapshotCommand.h"

namespace di = boost::di;

int main() {
    auto injector = di::make_injector(
        di::bind<CommonInMemoryRepository>.to<CommonInMemoryRepository>().in(di::singleton),

        di::bind<IRepository<BankAccount>>.to(
                [](const auto& injector) -> IRepository<BankAccount>& {
                    auto& repo = injector.template create<CommonInMemoryRepository&>();
                    return repo.getBankAccounts();
                }
        ),
                di::bind<IRepository<Category>>.to(
                [](const auto& injector) -> IRepository<Category>& {
                    auto& repo = injector.template create<CommonInMemoryRepository&>();
                    return repo.getCategories();
                }
        ),
                di::bind<IRepository<Operation>>.to(
                [](const auto& injector) -> IRepository<Operation>& {
                    auto& repo = injector.template create<CommonInMemoryRepository&>();
                    return repo.getOperations();
                }
        ),

        di::bind<IValidator>.to<Validator>(),

        di::bind<DomainFactory>.to<DomainFactory>(),

        di::bind<OperationNotifier>.to<OperationNotifier>().in(di::singleton),
        di::bind<AccountBalanceObserver>.to<AccountBalanceObserver>().in(di::singleton),

        di::bind<BankAccountFacade>.to<BankAccountFacade>(),
        di::bind<CategoryFacade>.to<CategoryFacade>(),
        di::bind<OperationFacade>.to<OperationFacade>(),

        di::bind<RepositoryCaretaker>.to<RepositoryCaretaker>(),
        di::bind<CommandManager>.to<CommandManager>(),

        di::bind<OperationBuilder>.to(
                [](IValidator& validator, OperationType type) {
                    return OperationBuilder(validator, type);
                }
        )
    );

    auto& repo = injector.create<CommonInMemoryRepository&>();
    auto& validator = injector.create<IValidator&>();
    auto& operationNotifier = injector.create<OperationNotifier&>();
    auto& bankFacade = injector.create<BankAccountFacade&>();
    auto& categoryFacade = injector.create<CategoryFacade&>();
    auto& operationFacade = injector.create<OperationFacade&>();
    auto& commandManager = injector.create<CommandManager&>();
    auto& repositoryCaretaker = injector.create<RepositoryCaretaker&>();

    operationNotifier.attach(std::make_shared<AccountBalanceObserver>(repo.getBankAccounts()));

    bool hasSnapshot = false;

    bool useTimedCommands = true;
    bool running = true;

    while (running) {
        ConsoleUI::printHeader("BANKING OPERATIONS MANAGEMENT SYSTEM");
        std::cout << "1. Bank Accounts Management\n";
        std::cout << "2. Categories Management\n";
        std::cout << "3. Operations Management\n";
        std::cout << "4. Snapshots (Memento)\n";
        std::cout << "5. Toggle Timed Commands: " << (useTimedCommands ? "ON" : "OFF") << "\n";
        std::cout << "6. Display All Data\n";
        std::cout << "7. Exit\n";

        int choice = ConsoleUI::getIntInput("Select option: ", 1, 7);

        switch (choice) {
            case 1: {
                while (true) {
                    ConsoleUI::printHeader("BANK ACCOUNTS MANAGEMENT");
                    std::cout << "1. Create New Account\n";
                    std::cout << "2. View All Accounts\n";
                    std::cout << "3. Edit Account\n";
                    std::cout << "4. Delete Account\n";
                    std::cout << "5. Back\n";

                    int accountChoice = ConsoleUI::getIntInput("Select action: ", 1, 5);

                    if (accountChoice == 5) break;

                    switch (accountChoice) {
                        case 1: {
                            std::string name = ConsoleUI::getStringInput("Enter account name: ");
                            double balance = ConsoleUI::getDoubleInput("Enter initial balance: ");
                            Status result = bankFacade.createBankAccount(name, balance);
                            if (result == Status::Success) ConsoleUI::showSuccess("Account created successfully!");
                            else ConsoleUI::showError(bankFacade.getLastError());
                            break;
                        }
                        case 2: {
                            bankFacade.printAllAccounts();
                            break;
                        }
                        case 3: {
                            bankFacade.printAllAccounts();
                            int id = ConsoleUI::getIntInput("Enter account ID to edit: ", 1);
                            std::string newName = ConsoleUI::getStringInput("Enter new account name: ");
                            double newBalance = ConsoleUI::getDoubleInput("Enter new balance: ");
                            Status result = bankFacade.editBankAccount(id, newName, newBalance);
                            if (result == Status::Success) ConsoleUI::showSuccess("Account updated successfully!");
                            else ConsoleUI::showError(bankFacade.getLastError());
                            break;
                        }
                        case 4: {
                            bankFacade.printAllAccounts();
                            int id = ConsoleUI::getIntInput("Enter account ID to delete: ", 1);
                            Status result = bankFacade.deleteBankAccount(id);
                            if (result == Status::Success) ConsoleUI::showSuccess("Account deleted successfully!");
                            else ConsoleUI::showError(bankFacade.getLastError());
                            break;
                        }
                        default: {
                            ConsoleUI::showError("Invalid option");
                            break;
                        }
                    }
                    ConsoleUI::pressAnyKey();
                }
                break;
            }

            case 2: {
                while (true) {
                    ConsoleUI::printHeader("CATEGORIES MANAGEMENT");
                    std::cout << "1. Create New Category\n";
                    std::cout << "2. View All Categories\n";
                    std::cout << "3. Edit Category\n";
                    std::cout << "4. Delete Category\n";
                    std::cout << "5. Back\n";

                    int categoryChoice = ConsoleUI::getIntInput("Select action: ", 1, 5);

                    if (categoryChoice == 5) break;

                    switch (categoryChoice) {
                        case 1: {
                            std::cout << "Select category type:\n";
                            std::cout << "1. Income\n";
                            std::cout << "2. Expense\n";
                            int typeChoice = ConsoleUI::getIntInput("Your choice: ", 1, 2);
                            OperationType type = (typeChoice == 1) ? OperationType::Income : OperationType::Expense;
                            std::string name = ConsoleUI::getStringInput("Enter category name: ");
                            Status result = categoryFacade.createCategory(type, name);
                            if (result == Status::Success) ConsoleUI::showSuccess("Category created successfully!");
                            else ConsoleUI::showError(categoryFacade.getLastError());
                            break;
                        }
                        case 2: {
                            categoryFacade.printAllCategories();
                            break;
                        }
                        case 3: {
                            categoryFacade.printAllCategories();
                            int id = ConsoleUI::getIntInput("Enter category ID to edit: ", 1);
                            std::cout << "Select new category type:\n";
                            std::cout << "1. Income\n";
                            std::cout << "2. Expense\n";
                            int typeChoice = ConsoleUI::getIntInput("Your choice: ", 1, 2);
                            OperationType newType = (typeChoice == 1) ? OperationType::Income : OperationType::Expense;
                            std::string newName = ConsoleUI::getStringInput("Enter new category name: ");
                            Status result = categoryFacade.editCategory(id, newType, newName);
                            if (result == Status::Success) ConsoleUI::showSuccess("Category updated successfully!");
                            else ConsoleUI::showError(categoryFacade.getLastError());
                            break;
                        }
                        case 4: {
                            categoryFacade.printAllCategories();
                            int id = ConsoleUI::getIntInput("Enter category ID to delete: ", 1);
                            Status result = categoryFacade.deleteCategory(id);
                            if (result == Status::Success) ConsoleUI::showSuccess("Category deleted successfully!");
                            else ConsoleUI::showError(categoryFacade.getLastError());
                            break;
                        }
                        default: {
                            ConsoleUI::showError("Invalid option");
                            break;
                        }
                    }
                    ConsoleUI::pressAnyKey();
                }
                break;
            }

            case 3: {
                while (true) {
                    ConsoleUI::printHeader("OPERATIONS MANAGEMENT");
                    std::cout << "1. Create Income Operation\n";
                    std::cout << "2. Create Expense Operation\n";
                    std::cout << "3. View All Operations\n";
                    std::cout << "4. Edit Operation\n";
                    std::cout << "5. Delete Operation\n";
                    std::cout << "6. Undo Last Operation\n";
                    std::cout << "7. Redo Last Operation\n";
                    std::cout << "8. Back\n";

                    int operationChoice = ConsoleUI::getIntInput("Select action: ", 1, 8);

                    if (operationChoice == 8) break;

                    switch (operationChoice) {
                        case 1:
                        case 2: {
                            OperationType type = (operationChoice == 1) ? OperationType::Income : OperationType::Expense;
                            bankFacade.printAllAccounts();
                            int accountId = ConsoleUI::getIntInput("Enter bank account ID: ", 1);
                            double amount = ConsoleUI::getDoubleInput("Enter amount: ", 0.01);
                            categoryFacade.printAllCategories();
                            int categoryId = ConsoleUI::getIntInput("Enter category ID (0 - no category): ", 0);
                            std::string description = ConsoleUI::getStringInput("Enter description: ", true);

                            OperationBuilder builder(validator, type);

                            builder.setBankAccount(accountId)
                                    .setAmount(amount)
                                    .setCategory(categoryId)
                                    .setDescription(description);

                            std::unique_ptr<ICommand> command = std::make_unique<AddOperationCommand>(operationFacade, builder);
                            if (useTimedCommands) {
                                command = std::make_unique<TimedCommand>(std::move(command), "AddOperation");
                            }

                            Status result = commandManager.executeCommand(std::move(command));
                            if (result == Status::Success) ConsoleUI::showSuccess("Operation created successfully!");
                            else ConsoleUI::showError(operationFacade.getLastError());
                            break;
                        }
                        case 3: {
                            operationFacade.printAllOperations();
                            break;
                        }
                        case 4: {
                            operationFacade.printAllOperations();
                            int id = ConsoleUI::getIntInput("Enter operation ID to edit: ", 1);
                            std::cout << "Select new operation type:\n";
                            std::cout << "1. Income\n";
                            std::cout << "2. Expense\n";
                            int typeChoice = ConsoleUI::getIntInput("Your choice: ", 1, 2);
                            OperationType newType = (typeChoice == 1) ? OperationType::Income : OperationType::Expense;
                            bankFacade.printAllAccounts();
                            int newAccountId = ConsoleUI::getIntInput("Enter new bank account ID: ", 1);
                            double newAmount = ConsoleUI::getDoubleInput("Enter new amount: ", 0.01);
                            categoryFacade.printAllCategories();
                            int newCategoryId = ConsoleUI::getIntInput("Enter new category ID (0 - no category): ", 0);
                            std::string newDescription = ConsoleUI::getStringInput("Enter new description: ", true);

                            OperationBuilder builder(validator, newType);
                            builder.setBankAccount(newAccountId)
                                    .setAmount(newAmount)
                                    .setCategory(newCategoryId)
                                    .setDescription(newDescription);

                            std::unique_ptr<ICommand> command = std::make_unique<EditOperationCommand>(operationFacade, id, builder);
                            if (useTimedCommands) {
                                command = std::make_unique<TimedCommand>(std::move(command), "EditOperation");
                            }

                            Status result = commandManager.executeCommand(std::move(command));
                            if (result == Status::Success) ConsoleUI::showSuccess("Operation updated successfully!");
                            else ConsoleUI::showError(operationFacade.getLastError());
                            break;
                        }
                        case 5: {
                            operationFacade.printAllOperations();
                            int id = ConsoleUI::getIntInput("Enter operation ID to delete: ", 1);

                            std::unique_ptr<ICommand> command = std::make_unique<DeleteOperationCommand>(operationFacade, id);
                            if (useTimedCommands) {
                                command = std::make_unique<TimedCommand>(std::move(command), "DeleteOperation");
                            }

                            Status result = commandManager.executeCommand(std::move(command));
                            if (result == Status::Success) ConsoleUI::showSuccess("Operation deleted successfully!");
                            else ConsoleUI::showError(operationFacade.getLastError());
                            break;
                        }
                        case 6: {
                            Status result = commandManager.undo();
                            if (result == Status::Success) ConsoleUI::showSuccess("Undo successful!");
                            else ConsoleUI::showError("Nothing to undo");
                            break;
                        }
                        case 7: {
                            Status result = commandManager.redo();
                            if (result == Status::Success) ConsoleUI::showSuccess("Redo successful!");
                            else ConsoleUI::showError("Nothing to redo");
                            break;
                        }
                        default: {
                            ConsoleUI::showError("Invalid option");
                            break;
                        }
                    }
                    ConsoleUI::pressAnyKey();
                }
                break;
            }

            case 4: {
                ConsoleUI::printHeader("SNAPSHOTS MANAGEMENT");
                std::cout << "1. Create Snapshot\n";
                std::cout << "2. Restore Snapshot\n";
                std::cout << "3. Back\n";

                int snapshotChoice = ConsoleUI::getIntInput("Select action: ", 1, 3);

                switch (snapshotChoice) {
                    case 1: {
                        std::unique_ptr<ICommand> snapshotCmd = std::make_unique<SnapshotCommand>(repositoryCaretaker);

                        if (useTimedCommands) {
                            snapshotCmd = std::make_unique<TimedCommand>(
                                    std::move(snapshotCmd),
                                    "CreateSnapshot"
                            );
                        }

                        Status result = snapshotCmd->execute();
                        if (result == Status::Success) {
                            ConsoleUI::showSuccess("Snapshot created successfully!");
                            hasSnapshot = true;
                        } else {
                            ConsoleUI::showError("Failed to create snapshot");
                        }
                        break;
                    }

                    case 2: {
                        if (!hasSnapshot) {
                            ConsoleUI::showError("No snapshot available to restore!");
                            break;
                        }

                        std::unique_ptr<ICommand> restoreCmd = std::make_unique<SnapshotCommand>(repositoryCaretaker);

                        if (useTimedCommands) {
                            restoreCmd = std::make_unique<TimedCommand>(
                                    std::move(restoreCmd),
                                    "RestoreSnapshot"
                            );
                        }

                        Status result = restoreCmd->undo();
                        if (result == Status::Success) {
                            ConsoleUI::showSuccess("Snapshot restored successfully!");
                        } else {
                            ConsoleUI::showError("Failed to restore snapshot");
                        }
                        break;
                    }
                    case 3: {
                        break;
                    }
                    default: {
                        ConsoleUI::showError("Invalid option");
                        break;
                    }
                }
                ConsoleUI::pressAnyKey();
                break;
            }

            case 5: {
                useTimedCommands = !useTimedCommands;
                ConsoleUI::showSuccess("Timed commands " + std::string(useTimedCommands ? "enabled" : "disabled"));
                ConsoleUI::pressAnyKey();
                break;
            }

            case 6: {
                ConsoleUI::printHeader("ALL DATA");
                bankFacade.printAllAccounts();
                categoryFacade.printAllCategories();
                operationFacade.printAllOperations();
                ConsoleUI::pressAnyKey();
                break;
            }

            case 7: {
                running = false;
                ConsoleUI::showSuccess("Goodbye!");
                break;
            }
            default: {
                ConsoleUI::showError("Invalid option");
                break;
            }
        }
    }

    return 0;
}
