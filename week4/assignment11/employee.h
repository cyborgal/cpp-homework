#pragma once
#include "user.h"

class Employee : public User {
public:
    void accessLevel() override;
};
