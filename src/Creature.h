//
// Created by deepika on 10/19/18.
//

#ifndef ECE_30862_CREATURE_H
#define ECE_30862_CREATURE_H
#include "iostream"
#include <string>
#include "Attack.h"
#include "Trigger.h"
#include "Item.h"
#include <vector>

using namespace std;

class Creature {
public:
    string name;
    vector<Item*> vulnerability;
    Attack* creatureAttack;
    Trigger* creatureTrigger;

    //default constructor
    Creature(string value);
};


#endif //ECE_30862_CREATURE_H
