#ifndef KPO2_EDITOPERATIONCOMMAND_H
#define KPO2_EDITOPERATIONCOMMAND_H

#include "../facades/OperationFacade.h"
#include "ICommand.h"

class EditOperationCommand : public ICommand {
private:
    OperationFacade& facade;
    int operation_id;
    std::optional<Operation> old_state;
    OperationBuilder builder;

public:
    EditOperationCommand(OperationFacade& f, int id, OperationBuilder b)
            : facade(f), operation_id(id), builder(std::move(b)) {}

    Status execute() override {
        Operation new_op = builder.build(operation_id);
        return facade.editOperation(new_op, &old_state);
    }

    Status undo() override {
        if (old_state == std::nullopt)
            return Status::Error;

        Status status = facade.editOperation(old_state.value());
        if (status == Status::Success) {
            old_state = std::nullopt;
        }
        return status;
    }
};

#endif //KPO2_EDITOPERATIONCOMMAND_H
