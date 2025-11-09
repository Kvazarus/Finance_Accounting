#ifndef KPO2_REPOSITORYCARETAKER_H
#define KPO2_REPOSITORYCARETAKER_H

#include "../repository/CommonInMemoryRepository.h"

class RepositoryCaretaker {
private:
    CommonInMemoryRepository& repo;
    std::shared_ptr<CommonInMemoryRepository::Snapshot> snapshot;

public:
    explicit RepositoryCaretaker(CommonInMemoryRepository& repository)
            : repo(repository) {}

    void save() {
        snapshot = std::make_shared<CommonInMemoryRepository::Snapshot>(repo);
    }

    void restore() {
        if (snapshot)
            snapshot->restore();
    }

    bool hasSnapshot() const {
        return snapshot != nullptr;
    }
};

#endif //KPO2_REPOSITORYCARETAKER_H
