cmake_minimum_required(VERSION 3.12)
project(ece_30862)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ece_30862
    main.cpp
    tinyxml2.cpp
    tinyxml2.h
    Person.cpp
    Person.h
    Attack.cpp
    Attack.h
    Condition.cpp
    Condition.h Creature.cpp
        Creature.h
        Border.h
        Trigger.cpp
        Trigger.h
        Item.h
        Item.cpp
        TurnOn.h Room.cpp Room.h Container.h Container.cpp)
