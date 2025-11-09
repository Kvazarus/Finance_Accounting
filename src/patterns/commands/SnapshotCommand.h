#ifndef KPO2_SNAPSHOTCOMMAND_H
#define KPO2_SNAPSHOTCOMMAND_H

#include "ICommand.h"
#include "../RepositoryCaretaker.h"

// Для замеров времени с помощью Декоратора
class SnapshotCommand : public ICommand {
private:
    RepositoryCaretaker& caretaker;

public:
    explicit SnapshotCommand(RepositoryCaretaker& c) : caretaker(c) {}

    Status execute() override {
        caretaker.save();
        return Status::Success;
    }

    Status undo() override {
        if (!caretaker.hasSnapshot())
            return Status::Error;

        caretaker.restore();
        return Status::Success;
    }
};

#endif //KPO2_SNAPSHOTCOMMAND_H
