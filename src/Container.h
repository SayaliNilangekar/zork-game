//
// Created by Sayali on 19-Oct-18.
//

#ifndef ECE_30862_CONTAINER_H
#define ECE_30862_CONTAINER_H
#include <string>
#include <vector>
#include "Trigger.h"
#include "Item.h"

using namespace std;
class Container{
public:
    string name;
    vector<Item*> containerItem;  					//not everywhere
    string status; 							//not everywhere
    vector<string> accept;				//not everywhere
    Trigger *containerTrigger;
    //default constructor
    Container(string value);

};

#endif //ECE_30862_CONTAINER_H
