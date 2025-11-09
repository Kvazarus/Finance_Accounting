#ifndef KPO2_OPERATIONNOTIFIER_H
#define KPO2_OPERATIONNOTIFIER_H

#include <memory>
#include <vector>
#include "IOperationObserver.h"

class OperationNotifier {
private:
    std::vector<std::weak_ptr<IOperationObserver>> observers;

public:
    void attach(const std::shared_ptr<IOperationObserver>& observer) {
        observers.push_back(observer);
    }

    void detach(const std::shared_ptr<IOperationObserver>& observer) {
        observers.erase(
                std::__remove_if(observers.begin(), observers.end(),
                          [&](__gnu_cxx::__normal_iterator<std::weak_ptr<IOperationObserver> *, std::vector<std::weak_ptr<IOperationObserver>>> w) {
                              auto s = w->lock();
                              return !s || s == observer;
                          }),
                observers.end());
    }

    void notifyAdded(const Operation& op) {
        for (auto it = observers.begin(); it != observers.end();) {
            if (auto obs = it->lock()) {
                try {
                    obs->onOperationAdded(op);
                } catch (...) {}
                ++it;
            } else {
                it = observers.erase(it);
            }
        }
    }

    void notifyDeleted(const Operation& op) {
        for (auto it = observers.begin(); it != observers.end();) {
            if (auto obs = it->lock()) {
                obs->onOperationDeleted(op);
                ++it;
            } else {
                it = observers.erase(it);
            }
        }
    }

    void notifyEdited(const Operation& old_op, const Operation& new_op) {
        for (auto it = observers.begin(); it != observers.end();) {
            if (auto obs = it->lock()) {
                obs->onOperationEdited(old_op, new_op);
                ++it;
            } else {
                it = observers.erase(it);
            }
        }
    }
};

#endif //KPO2_OPERATIONNOTIFIER_H
