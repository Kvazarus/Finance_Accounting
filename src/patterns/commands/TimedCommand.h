#ifndef KPO2_TIMEDCOMMAND_H
#define KPO2_TIMEDCOMMAND_H

#include <memory>
#include <string>
#include <chrono>
#include <iostream>
#include "ICommand.h"

// Паттерн "Декоратор"
class TimedCommand : public ICommand {
private:
    std::unique_ptr<ICommand> wrapped;
    std::string name;

    template<typename Func>
    Status measure(Func&& func, const std::string& action) {
        using namespace std::chrono;
        auto start = std::chrono::high_resolution_clock::now();

        Status result = func();

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        std::cout << "[TimedCommand] " << name << "::" << action
                  << " took " << duration << " mcs\n";

        return result;
    }

public:
    explicit TimedCommand(std::unique_ptr<ICommand> cmd, std::string n = "Command")
            : wrapped(std::move(cmd)), name(std::move(n)) {}

    Status execute() override {
        return measure([&]() { return wrapped->execute(); }, "execute");
    }

    Status undo() override {
        return measure([&]() { return wrapped->undo(); }, "undo");
    }
};

#endif //KPO2_TIMEDCOMMAND_H
