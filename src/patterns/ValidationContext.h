#ifndef KPO2_VALIDATIONCONTEXT_H
#define KPO2_VALIDATIONCONTEXT_H

#include <memory>
#include <stdexcept>
#include "IValidationStrategy.h"

class ValidationContext {
private:
    std::unique_ptr<IValidationStrategy> strategy;
public:
    void setStrategy(std::unique_ptr<IValidationStrategy> s) { strategy = std::move(s); }

    Status execute() {
        if (!strategy) throw std::runtime_error("Validation strategy not set");
        return strategy->validate();
    }
};

#endif //KPO2_VALIDATIONCONTEXT_H
