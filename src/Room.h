//
// Created by deepika on 10/19/18.
//

#ifndef ECE_30862_ROOM_H
#define ECE_30862_ROOM_H

#include "iostream"
#include "Trigger.h"
#include "Item.h"
#include "Border.h"
#include "Creature.h"
#include "Container.h"
#include <vector>
#include <string>
using namespace std;

class Room {
public:
    string name;
    string description;
    Trigger* roomTrigger;
    vector<Item*> roomItem;
    vector<Border*> roomBorder;
    vector<Container*> roomContainer;
    vector<Creature*> roomCreature;
    string roomType;

    //default constructors
    Room();
};


#endif //ECE_30862_ROOM_H
