#ifndef KPO2_ICOMMAND_H
#define KPO2_ICOMMAND_H

#include "../../utils/Status.h"

// Паттерн "Команда"
class ICommand {
public:
    virtual Status execute() = 0;
    virtual Status undo() = 0;
    virtual ~ICommand() = default;
};

#endif //KPO2_ICOMMAND_H
