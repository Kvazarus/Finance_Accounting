#ifndef KPO2_CATEGORY_H
#define KPO2_CATEGORY_H

#include "IEntity.h"
#include "../utils/OperationType.h"
#include <string>

class Category : public IEntity {
private:
    int id;
    OperationType type;
    std::string name;

public:
    Category(int id_, OperationType type_, std::string name_) : id(id_), type(type_), name(std::move(name_)) {}

    int getId() const override { return id; }
    OperationType getType() const { return type; }
    std::string getName() const { return name; }

    void update(OperationType new_type, std::string new_name) {
        type = new_type;
        name = std::move(new_name);
    }
};

#endif //KPO2_CATEGORY_H
