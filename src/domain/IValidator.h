#ifndef KPO2_IVALIDATOR_H
#define KPO2_IVALIDATOR_H

#include <string>
#include "../utils/Status.h"
#include "../utils/OperationType.h"

class IValidator {
public:
    virtual Status validateBankAccount(const std::string& name, double balance) = 0;
    virtual Status validateCategory(const std::string& name) = 0;
    virtual Status validateOperation(OperationType type, int bank_account_id, double amount,
                                     int category_id, const std::string& date) = 0;
    virtual const std::string& getLastError() const = 0;
    virtual ~IValidator() = default;
};

#endif //KPO2_IVALIDATOR_H
