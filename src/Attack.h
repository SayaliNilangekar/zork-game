//
// Created by deepika on 10/19/18.
//

#ifndef ECE_30862_ATTACK_H
#define ECE_30862_ATTACK_H
#include "iostream"
#include "Condition.h"
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;


class Attack {
public:
    Condition* condition;
    string message;
    vector<string> action;
};


#endif //ECE_30862_ATTACK_H
