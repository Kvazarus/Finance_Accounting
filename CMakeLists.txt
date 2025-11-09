cmake_minimum_required(VERSION 3.28)
project(KPO2)

set(CMAKE_CXX_STANDARD 17)

add_executable(KPO2 src/main.cpp
        src/domain/IEntity.h
        src/domain/BankAccount.h
        src/utils/OperationType.h
        src/domain/Category.h
        src/domain/Operation.h
        src/utils/Helpers.h
        src/utils/Status.h
        src/repository/IRepository.h
        src/repository/InMemoryRepository.h
        src/domain/IValidator.h
        src/domain/Validator.h
        src/patterns/IValidationStrategy.h
        src/patterns/CategoryValidationStrategy.h
        src/patterns/OperationValidationStrategy.h
        src/patterns/ValidationContext.h
        src/patterns/BankAccountValidationStrategy.h
        src/patterns/IOperationObserver.h
        src/patterns/OperationNotifier.h
        src/patterns/AccountBalanceObserver.h
        src/patterns/DomainFactory.h
        src/patterns/OperationBuilder.h
        src/patterns/facades/BankAccountFacade.h
        src/patterns/facades/CategoryFacade.h
        src/patterns/facades/OperationFacade.h
        src/patterns/commands/ICommand.h
        src/patterns/commands/AddOperationCommand.h
        src/patterns/commands/DeleteOperationCommand.h
        src/patterns/commands/EditOperationCommand.h
        src/patterns/commands/TimedCommand.h
        src/repository/CommonInMemoryRepository.h
        src/patterns/commands/SnapshotCommand.h
        src/patterns/RepositoryCaretaker.h
        src/patterns/CommandManager.h
        src/UI/ConsoleUI.h)
