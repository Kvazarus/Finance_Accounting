#ifndef KPO2_ADDOPERATIONCOMMAND_H
#define KPO2_ADDOPERATIONCOMMAND_H

#include "ICommand.h"
#include "../facades/OperationFacade.h"

class AddOperationCommand : public ICommand {
private:
    OperationFacade& facade;
    OperationBuilder builder;
    int operation_id = -1;

public:
    AddOperationCommand(OperationFacade &f, OperationBuilder b)
            : facade(f), builder(std::move(b)) {}

    Status execute() override {
        Status status = facade.createOperation(builder, &operation_id);
        return status;
    }

    Status undo() override {
        if (operation_id != -1) {
            Status status = facade.deleteOperation(operation_id);
            if (status == Status::Success) {
                operation_id = -1;
            }
            return status;
        }
        return Status::Error;
    }
};

#endif //KPO2_ADDOPERATIONCOMMAND_H
