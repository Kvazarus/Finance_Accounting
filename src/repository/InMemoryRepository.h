#ifndef KPO2_INMEMORYREPOSITORY_H
#define KPO2_INMEMORYREPOSITORY_H

#include "IRepository.h"

template<typename T>
class InMemoryRepository : public IRepository<T> {
private:
    std::unordered_map<int, std::shared_ptr<T>> entities;
public:
    void add(std::shared_ptr<T> entity) override { entities[entity->getId()] = std::move(entity); }
    std::shared_ptr<T> findById(int id) override {
        auto it = entities.find(id);
        return it == entities.end() ? nullptr : it->second;
    }
    Status removeById(int id) override {
        if (entities.erase(id)) return Status::Success;
        return Status::Error;
    }
    const std::unordered_map<int, std::shared_ptr<T>>& getAll() const override { return entities; }

    void clear() { entities.clear(); }
};

#endif //KPO2_INMEMORYREPOSITORY_H
