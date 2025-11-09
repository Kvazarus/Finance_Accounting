#ifndef KPO2_IREPOSITORY_H
#define KPO2_IREPOSITORY_H

#include <memory>
#include <unordered_map>
#include "../utils/Status.h"

template<typename T>
class IRepository {
public:
    virtual void add(std::shared_ptr<T> entity) = 0;
    virtual std::shared_ptr<T> findById(int id) = 0;
    virtual Status removeById(int id) = 0;
    virtual const std::unordered_map<int, std::shared_ptr<T>>& getAll() const = 0;
    virtual ~IRepository() = default;
};

#endif //KPO2_IREPOSITORY_H
