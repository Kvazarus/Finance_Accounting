#ifndef KPO2_IOPERATIONOBSERVER_H
#define KPO2_IOPERATIONOBSERVER_H

#include "../utils/Status.h"
#include "../domain/Operation.h"

// Паттерн "Наблюдатель"
class IOperationObserver {
public:
    virtual ~IOperationObserver() = default;
    virtual Status onOperationAdded(const Operation& operation) = 0;
    virtual Status onOperationDeleted(const Operation& operation) = 0;
    virtual Status onOperationEdited(const Operation& old_op, const Operation& new_op) = 0;
};

#endif //KPO2_IOPERATIONOBSERVER_H
