#ifndef KPO2_IENTITY_H
#define KPO2_IENTITY_H

class IEntity {
public:
    virtual int getId() const = 0;
    virtual ~IEntity() = default;
};

#endif //KPO2_IENTITY_H
