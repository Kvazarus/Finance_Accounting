#ifndef KPO2_COMMANDMANAGER_H
#define KPO2_COMMANDMANAGER_H

#include <memory>
#include <stack>
#include "commands/ICommand.h"

class CommandManager {
private:
    std::stack<std::unique_ptr<ICommand>> undoStack;
    std::stack<std::unique_ptr<ICommand>> redoStack;

public:
    Status executeCommand(std::unique_ptr<ICommand> command) {
        Status result = command->execute();
        if (result == Status::Success) {
            undoStack.push(std::move(command));
            while (!redoStack.empty()) redoStack.pop();
        }
        return result;
    }

    Status undo() {
        if (undoStack.empty()) return Status::Error;
        auto command = std::move(undoStack.top());
        undoStack.pop();
        Status result = command->undo();
        if (result == Status::Success) redoStack.push(std::move(command));
        return result;
    }

    Status redo() {
        if (redoStack.empty()) return Status::Error;
        auto command = std::move(redoStack.top());
        redoStack.pop();
        Status result = command->execute();
        if (result == Status::Success) undoStack.push(std::move(command));
        return result;
    }

    bool canUndo() const { return !undoStack.empty(); }
    bool canRedo() const { return !redoStack.empty(); }
};

#endif //KPO2_COMMANDMANAGER_H
