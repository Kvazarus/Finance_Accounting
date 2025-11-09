#ifndef KPO2_IVALIDATIONSTRATEGY_H
#define KPO2_IVALIDATIONSTRATEGY_H

#include "../utils/Status.h"
#include "../domain/IValidator.h"

// Паттерн "Стратегия"
class IValidationStrategy {
public:
    virtual Status validate() = 0;
    virtual ~IValidationStrategy() = default;
};

#endif //KPO2_IVALIDATIONSTRATEGY_H
