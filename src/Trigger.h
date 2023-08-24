//
// Created by Sayali on 19-Oct-18.
//
#ifndef PROJECT_TRIGGER_H
#define PROJECT_TRIGGER_H

#include "Condition.h"
#include <string>
#include <vector>


class Trigger{
public:
    std::string type;
    int type_count;
    std::string command;
    Condition* trigCondition;
    std::string message;			        //print				//not everywhere
    std::vector<std::string> action;
    std::string location;

    //default contructor
    Trigger();
};

#endif //PROJECT_TRIGGER_H
