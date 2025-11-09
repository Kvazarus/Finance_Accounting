#ifndef KPO2_OPERATION_H
#define KPO2_OPERATION_H

#include "IEntity.h"
#include "../utils/OperationType.h"
#include <string>

class Operation : public IEntity {
private:
    int id;
    OperationType type;
    int bank_account_id;
    double amount;
    std::string date;
    std::string description;
    int category_id;

public:
    Operation(int id_, OperationType type_, int bank_account_id_,
              double amount_, std::string date_, std::string description_, int category_id_)
            : id(id_), type(type_), bank_account_id(bank_account_id_),
              amount(amount_), date(std::move(date_)), description(std::move(description_)),
              category_id(category_id_) {
    }

    int getId() const override { return id; }
    OperationType getType() const { return type; }
    int getBankAccountId() const { return bank_account_id; }
    double getAmount() const { return amount; }
    const std::string& getDate() const { return date; }
    const std::string& getDescription() const { return description; }
    int getCategoryId() const { return category_id; }

    void update(OperationType new_type, int new_bank_account_id, double new_amount,
                int new_category_id, std::string new_date, std::string new_description) {
        type = new_type;
        bank_account_id = new_bank_account_id;
        amount = new_amount;
        category_id = new_category_id;
        date = std::move(new_date);
        description = std::move(new_description);
    }
};

#endif //KPO2_OPERATION_H
