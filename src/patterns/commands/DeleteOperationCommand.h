#ifndef KPO2_DELETEOPERATIONCOMMAND_H
#define KPO2_DELETEOPERATIONCOMMAND_H

#include "ICommand.h"
#include "../facades/OperationFacade.h"

class DeleteOperationCommand : public ICommand {
private:
    OperationFacade& facade;
    int operation_id;
    std::optional<Operation> old_state;

public:
    DeleteOperationCommand(OperationFacade& f, int id)
            : facade(f), operation_id(id) {}

    Status execute() override {
        return facade.deleteOperation(operation_id, &old_state);
    }

    Status undo() override {
        if (old_state == std::nullopt)
            return Status::Error;

        Status status = facade.addOperation(old_state.value());
        if (status == Status::Success) {
            old_state = std::nullopt;
        }
        return status;
    }
};

#endif //KPO2_DELETEOPERATIONCOMMAND_H
