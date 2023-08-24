//
// Created by Sayali on 19-Oct-18.
//

#ifndef PROJECT_ITEM_H
#define PROJECT_ITEM_H
#include <string>
#include "TurnOn.h"

class Item{
public:
    std::string name;
    std::string writing;							//not everywhere
    std::string status; 							//not everywhere
    TurnOn* itemTurnOn;

    //default constructor
    Item(std::string value);
};

#endif //PROJECT_ITEM_H
