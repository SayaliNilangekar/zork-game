#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "tinyxml2.h"
#include "iostream"
#include "Room.h"
#include "Person.h"

using namespace tinyxml2;
using namespace std;

//statements need
//turn item availability "on" oly after you open the container
//give error it take (item) and item already in inventory
//write action function which are within triggers

//variable declaration
vector<Room*> roomVect;
vector<Item*> itemVect;
vector<Container*> containerVect;
vector<Creature*> creatureVect;
vector<Trigger*> triggerVect;
Person *person = new Person();
string command;
int container_items_availability=0; //hardcoded
bool exit_flag = true;

//function declaration

void read_item(string item){
    for(int invent_item=0; invent_item < person->inventory.size(); invent_item++){
        if(person->inventory[invent_item]->name == item){
            if(person->inventory[invent_item]->writing != "\0"){
                cout<<person->inventory[invent_item]->writing<<endl;
            }else{
                cout<<"Nothing written"<<endl;
            }
        }
    }

}

void open_exit(){
    if(person->currentRoom->roomType == "exit"){
        cout<<"Game Over"<<endl;
        exit_flag=false;
    }
}

void open_container(string container){
    vector<Container*> currContainer = person->currentRoom->roomContainer;
    for(int i =0; i < currContainer.size(); i++){
        if(currContainer[i]->name == container){
            if(!currContainer[i]->containerItem.empty()){
                cout<<currContainer[i]->name<<" contains ";
                cout<<currContainer[i]->containerItem[0]->name;
                for(int j=1; j<currContainer[i]->containerItem.size(); j++){
                    cout<<" "<<currContainer[i]->containerItem[j]->name;
                }
                container_items_availability=1;
            }else{
                cout<<currContainer[i]->name<<" is empty.";
            }
        }
    }
    cout<<endl;
}

void take_item(string item){
    //if item is in room
     vector<Item*> item_list = person->currentRoom->roomItem;
     for(int i=0; i < item_list.size(); i++){
         if(item_list[i]->name == item){
             person->inventory.push_back(item_list[i]);
             cout<<"Item "<<item_list[i]->name<<" added to inventory."<<endl;
             item_list.erase(find(item_list.begin(),item_list.end(),item_list[i]));
             person->currentRoom->roomItem=item_list;
         }
     }

     //if item is in room's container
     for( int container=0; container<person->currentRoom->roomContainer.size(); container++){
         vector<Item*> item_list = person->currentRoom->roomContainer[container]->containerItem;
         for(int i=0; i < item_list.size(); i++){
             if(item_list[i]->name == item) {
                 if (container_items_availability == 1) {
                     person->inventory.push_back(item_list[i]);
                     cout << "Item " << item_list[i]->name << " added to inventory." << endl;
                     item_list.erase(find(item_list.begin(), item_list.end(), item_list[i]));
                     person->currentRoom->roomContainer[container]->containerItem=item_list;
                     container_items_availability=0;
                 }else{
                     cout<<"Error"<<endl;
                 }
             }

         }
     }

}

void checkInventory(){
    vector<Item*> inventory = person->inventory;
    if(!inventory.empty()){
        cout<<"Inventory: ";
        cout<<inventory[0]->name;
        for(int item=1; item<inventory.size(); item++){
            cout<<", "<<inventory[item]->name;
        }
    }else{
        cout<<"Inventory: empty";
    }
    cout<<endl;
}

void changeRoom(string roomCommand){
    Room* currentRoom=person->currentRoom;
    if (roomCommand == "n"){
        roomCommand="north";
    }else if(roomCommand=="s"){
        roomCommand="south";
    }else if(roomCommand=="w"){
        roomCommand="west";
    }else if(roomCommand=="e"){
        roomCommand="east";
    }
    for(int border=0; border < (person->currentRoom->roomBorder).size(); border++){
        if(person->currentRoom->roomBorder[border]->direction == roomCommand){
            string room_name = person->currentRoom->roomBorder[border]->name;
            for(int i=0; i<roomVect.size(); i++){
                if(roomVect[i]->name == room_name){
                    currentRoom=roomVect[i];
                }
            }
        }
    }
    if(person->currentRoom==currentRoom){
        cout<<"Can't go that way."<<endl;
    }else{
        cout<<currentRoom->description<<endl;
    }
    person->currentRoom=currentRoom;
}

string checkRoomTrigger(string command, Room* currRoom, vector<Item*> inventory){
    vector<string> trigger_match;
    trigger_match.push_back(currRoom->name);
    //container
    for(int i=0; i<currRoom->roomContainer.size();i++){
        trigger_match.push_back(currRoom->roomContainer[i]->name);
    }
    //creature
    for(int c=0; c<currRoom->roomCreature.size();c++){
        trigger_match.push_back(currRoom->roomCreature[c]->name);
    }

    //match trigger location
    for (int i = 0; i<triggerVect.size(); i++){
        if (std::find(trigger_match.begin(), trigger_match.end(), triggerVect[i]->location) != trigger_match.end()){
            if (triggerVect[i]->command == command){
                if (triggerVect[i]->type == "single"){
                    if (triggerVect[i]->type_count == 0){
                        //++(triggerVect[i]->type_count);

                        if(triggerVect[i]->trigCondition->has != "\0" ){

                            //Condition has object, owner and has
                            string has = triggerVect[i]->trigCondition->has;
                            string object = triggerVect[i]->trigCondition->object;
                            string owner = triggerVect[i]->trigCondition->owner;
                            if (has == "yes") {
                                //check if owner is "ROOM"
                                for (int room=0; room < roomVect.size(); room++) {
                                    if (roomVect[room]->name == owner) {
                                        vector<Item *> checkItem=roomVect[room]->roomItem;
                                        for (int k=0; k < checkItem.size(); k++) {
                                            if (checkItem[k]->name == object) {
                                                cout << triggerVect[i]->message << endl;
                                                ++(triggerVect[i]->type_count);
                                                return "nochange";
                                            }//there should be an else
                                        }
                                    }
                                }//check if owner room ends here

                                for (int container=0; container < containerVect.size(); container++) {
                                    if (containerVect[container]->name == owner) {
                                        vector<Item *> checkItem=containerVect[container]->containerItem;
                                        for (int j=0; j < checkItem.size(); j++) {
                                            if (checkItem[j]->name == object) {
                                                cout << triggerVect[i]->message << endl;
                                                ++(triggerVect[i]->type_count);
                                                return "nochange";
                                            }//there should be an else
                                        }
                                    }
                                }//check if owner == container ends here

                                if (owner == "inventory") {
                                    if(!inventory.empty()) {
                                        for (int invent=0; invent < inventory.size(); invent++) {
                                            if (inventory[invent]->name == object) {
                                                cout << triggerVect[i]->message << endl;
                                                ++(triggerVect[i]->type_count);
                                                return "nochange";
                                            }
                                        }
                                    }
                                }//check inventory ends here

                            }//has==yes
                            else if (has == "no"){
                                //check if owner is "ROOM"-------->HAS === NO<--------
                                for(int room=0; room<roomVect.size(); room++){
                                    if ( roomVect[room]->name == owner){
                                        int not_found=1;
                                        vector <Item*> checkItem = roomVect[room]->roomItem;
                                        for(int k=0; k < checkItem.size(); k++){
                                            if(checkItem[k]->name == object){

                                                not_found=0;
                                            }
                                        }
                                        if(not_found==1){
                                            cout<<triggerVect[i]->message<<endl;
                                            ++(triggerVect[i]->type_count);
                                            return "nochange";
                                        }
                                    }
                                }//check if owner room ends here

                                for(int container=0; container<containerVect.size(); container++){
                                    if ( containerVect[container]->name == owner){
                                        int not_found=1;
                                        vector <Item*> checkItem = containerVect[container]->containerItem;
                                        for(int j=0; j < checkItem.size(); j++){
                                            if(checkItem[j]->name == object){

                                                not_found=0;
                                            }
                                        }
                                        if(not_found==1){
                                            cout<<triggerVect[i]->message<<endl;
                                            ++(triggerVect[i]->type_count);
                                            return "nochange";
                                        }
                                    }
                                }//check if owner == container ends here
                                if (owner == "inventory") {
                                    int not_found=1;
                                    if(!inventory.empty()) {
                                        for (int invent=0; invent < inventory.size(); invent++) {
                                            if (inventory[invent]->name == object) {
                                                not_found=0;
                                            }
                                        }//check inventory ends here
                                        if(not_found==1){
                                            cout << triggerVect[i]->message<<endl;
                                            ++(triggerVect[i]->type_count);
                                            return "nochange";
                                        }
                                    }else{
                                        cout << triggerVect[i]->message << endl;
                                        ++(triggerVect[i]->type_count);
                                        return "nochange";
                                    }
                                }
                            }//has==no
                        }else{
                            string object = triggerVect[i]->trigCondition->object;
                            string status = triggerVect[i]->trigCondition->status;
                            int object_is_item=0;

                            //if object==item
                            for(int item=0; item<itemVect.size(); item++){
                                if(itemVect[item]->name == object){
                                    object_is_item=1;
                                    if(itemVect[item]->status == status){
                                        cout<<triggerVect[i]->message<<endl;
                                        ++(triggerVect[i]->type_count);
                                        return "nochange";
                                    }
                                }
                            }//object==item

                            //if object==container
                            if(object_is_item==0){
                                for(int container=0; container<containerVect.size(); container++){
                                    if((containerVect[container]->name == object) && (containerVect[container]->status == status)){
                                        cout<<triggerVect[i]->message<<endl;
                                        ++(triggerVect[i]->type_count);
                                        return "nochange";
                                    }
                                }
                            }//object==container

                        }  //else of has == "\0"


                    }//type_count ends here

                }else{ //--------------------------------------------->PERMANENT----------------------->
                    if(triggerVect[i]->trigCondition->has != "\0" ){

                        //Condition has object, owner and has
                        string has = triggerVect[i]->trigCondition->has;
                        string object = triggerVect[i]->trigCondition->object;
                        string owner = triggerVect[i]->trigCondition->owner;
                        if (has == "yes") {
                            //check if owner is "ROOM"
                            for (int room=0; room < roomVect.size(); room++) {
                                if (roomVect[room]->name == owner) {
                                    vector<Item *> checkItem=roomVect[room]->roomItem;
                                    for (int k=0; k < checkItem.size(); k++) {
                                        if (checkItem[k]->name == object) {
                                            cout << triggerVect[i]->message << endl;
                                            return "nochange";
                                        }//there should be an else
                                    }
                                }
                            }//check if owner room ends here

                            for (int container=0; container < containerVect.size(); container++) {
                                if (containerVect[container]->name == owner) {
                                    vector<Item *> checkItem=containerVect[container]->containerItem;
                                    for (int j=0; j < checkItem.size(); j++) {
                                        if (checkItem[j]->name == object) {
                                            cout << triggerVect[i]->message << endl;
                                            return "nochange";
                                        }//there should be an else
                                    }
                                }
                            }//check if owner == container ends here

                            if (owner == "inventory") {
                                if(!inventory.empty()) {
                                    for (int invent=0; invent < inventory.size(); invent++) {
                                        if (inventory[invent]->name == object) {
                                            cout << triggerVect[i]->message << endl;
                                            return "nochange";
                                        }
                                    }
                                }
                            }//check inventory ends here

                        }//has==yes
                        else if (has == "no"){
                            //check if owner is "ROOM"-------->HAS === NO<--------
                            for(int room=0; room<roomVect.size(); room++){
                                if ( roomVect[room]->name == owner){
                                    int not_found=1;
                                    vector <Item*> checkItem = roomVect[room]->roomItem;
                                    for(int k=0; k < checkItem.size(); k++){
                                        if(checkItem[k]->name == object){

                                            not_found=0;
                                        }
                                    }
                                    if(not_found==1){
                                        cout<<triggerVect[i]->message<<endl;
                                        return "nochange";
                                    }
                                }
                            }//check if owner room ends here

                            for(int container=0; container<containerVect.size(); container++){
                                if ( containerVect[container]->name == owner){
                                    int not_found=1;
                                    vector <Item*> checkItem = containerVect[container]->containerItem;
                                    for(int j=0; j < checkItem.size(); j++){
                                        if(checkItem[j]->name == object){

                                            not_found=0;
                                        }
                                    }
                                    if(not_found==1){
                                        cout<<triggerVect[i]->message<<endl;
                                        return "nochange";
                                    }
                                }
                            }//check if owner == container ends here
                            if (owner == "inventory") {
                                int not_found=1;
                                if(!inventory.empty()) {
                                    for (int invent=0; invent < inventory.size(); invent++) {
                                        if (inventory[invent]->name == object) {

                                            not_found=0;
                                        }
                                    }//check inventory ends here
                                    if(not_found==1){
                                        cout << triggerVect[i]->message<<endl;
                                        return "nochange";
                                    }
                                }else{
                                    cout << triggerVect[i]->message << endl;
                                    return "nochange";
                                }
                            }
                        }//permanent---->has==no
                    }else{
                        string object = triggerVect[i]->trigCondition->object;
                        string status = triggerVect[i]->trigCondition->status;
                        int object_is_item=0;

                        //if object==item (permanenet)
                        for(int item=0; item<itemVect.size(); item++){
                            if(itemVect[item]->name == object){
                                object_is_item=1;
                                if(itemVect[item]->status == status){
                                    cout<<triggerVect[i]->message<<endl;
                                    return "nochange";
                                }
                            }
                        }//object==item  (permanenet)

                        //if object==container  (permanenet)
                        if(object_is_item==0){
                            for(int container=0; container<containerVect.size(); container++){
                                if((containerVect[container]->name == object) && (containerVect[container]->status == status)){
                                    cout<<triggerVect[i]->message<<endl;
                                    return "nochange";
                                }
                            }
                        }//object==container  (permanenet)

                    }  //else of has == "\0"  (permanenet)

                }//else of single
            }//command ends here //write condition if it is non command trigger

        }//trigger match ends here

    }
    return "change";

}
void zork(vector<Room*>room,vector<Item*>item,vector<Container*>contaier,vector<Creature*>creature,vector<Trigger*>trigger){
    person->currentRoom = roomVect[0];
    cout<<person->currentRoom->description<<endl;
    string change_room;
    //input loop
    while (exit_flag){
        //cout<<"Enter now"<<endl;
        getline(cin,command);
        //cin >> command;
        //direction
        if ((command =="n")||(command =="s")||(command =="e")||(command =="w")) {
            change_room = checkRoomTrigger(command,person->currentRoom,person->inventory);
            if(change_room=="change"){
                changeRoom(command);
            }
        }
        //list inventory items
        if(command == "i"){
            checkInventory();
        }
        //take item in inventory
        if(command.find("take") != string::npos){
            command = command.erase(0,5);
            if(command == "\0"){
                cout<<"Error"<<endl;
            }else{
                take_item(command);
            }
        }
        //open(container)
        if(command.find("open") != string::npos){
            command = command.erase(0,5);
            if(command == "\0"){
                cout<<"Error"<<endl;
            }if(command == "exit"){
                open_exit();
            }
            else{
                open_container(command);
            }
        }
        //read (item)
        if(command.find("read") != string::npos){
            command = command.erase(0,5);
            if(command == "\0"){
                cout<<"Error"<<endl;
            }else{
                read_item(command);
            }
        }

    }

}
int main()
{
    //

    XMLDocument doc;

    tinyxml2::XMLError eResult = doc.LoadFile("/home/deepika/Desktop/C++/ZORK/ece-30862/sample.xml");
    if(eResult == tinyxml2::XML_SUCCESS)
    {
            XMLElement *pRoot, *pParm, *pProcess, *pApp, *pLineFormat;
            pRoot = doc.FirstChildElement("map");  //this is getting null try this??
            if ( pRoot )
            {
                // Parse parameters
                pParm=pRoot->FirstChildElement("room");

                while (pParm)
                {
                   Room *roomobj = new Room();
                   roomobj->name = pParm->FirstChildElement("name")->GetText();
                   roomobj->description = pParm->FirstChildElement("description")->GetText();
                   //for room type
                   if(pParm->FirstChildElement("type")){
                       roomobj->roomType=pParm->FirstChildElement("type")->GetText();
                   }
                   //border of room
                   pProcess=pParm->FirstChildElement("border");
                   while(pProcess) {
                       Border *roomborder = new Border();
                       roomborder->name=pProcess->FirstChildElement("name")->GetText();
                       roomborder->direction=pProcess->FirstChildElement("direction")->GetText();
                       roomobj->roomBorder.push_back(roomborder);
                       pProcess=pProcess->NextSiblingElement("border");
                   }
                   //items in room
                    pProcess=pParm->FirstChildElement("item");
                    while(pProcess) {
                        Item *roomItem = new Item(pProcess->GetText());
                        itemVect.push_back(roomItem);
                        roomobj->roomItem.push_back(roomItem);
                        pProcess=pProcess->NextSiblingElement("item");
                    }
                    //containers in room
                    pProcess=pParm->FirstChildElement("container");
                    while(pProcess) {
                        Container *roomContainer = new Container(pProcess->GetText());
                        containerVect.push_back(roomContainer);
                        roomobj->roomContainer.push_back(roomContainer);
                        pProcess=pProcess->NextSiblingElement("container");
                    }

                    //creatures in room
                    pProcess=pParm->FirstChildElement("creature");
                    while(pProcess) {
                        Creature *roomCreature = new Creature(pProcess->GetText());
                        creatureVect.push_back(roomCreature);
                        roomobj->roomCreature.push_back(roomCreature);
                        pProcess=pProcess->NextSiblingElement("creature");
                    }

                    //triggers in room
                    pProcess=pParm->FirstChildElement("trigger");
                    while(pProcess) {
                        Trigger *roomTrigger = new Trigger();
                        roomTrigger->location=roomobj->name;
                        if(pProcess->FirstChildElement("command")){
                            roomTrigger->command = pProcess->FirstChildElement("command")->GetText();
                        }
                        if(pProcess->FirstChildElement("type")){
                            roomTrigger->type = pProcess->FirstChildElement("type")->GetText();
                        }
                        if(pProcess->FirstChildElement("print")){
                            roomTrigger->message = pProcess->FirstChildElement("print")->GetText();
                        }

                        //deeper for actions
                        pApp=pProcess->FirstChildElement("action");
                        while(pApp) {
                            roomTrigger->action.push_back(pApp->GetText());
                            pApp=pApp->NextSiblingElement("action");
                        }

                        //deeper for condition
                        pApp=pProcess->FirstChildElement("condition");
                        Condition *roomCondition = new Condition();
                        if(pApp->FirstChildElement("object")){
                            roomCondition->object = pApp->FirstChildElement("object")->GetText();
                        }
                        if(pApp->FirstChildElement("status")){
                            roomCondition->status = pApp->FirstChildElement("status")->GetText();
                        }
                        if(pApp->FirstChildElement("owner")){
                            roomCondition->owner = pApp->FirstChildElement("owner")->GetText();
                        }
                        if(pApp->FirstChildElement("has")){
                            roomCondition->has = pApp->FirstChildElement("has")->GetText();
                        }
                        roomTrigger->trigCondition = roomCondition;

                        roomobj->roomTrigger = roomTrigger;
                        triggerVect.push_back(roomTrigger);

                        pProcess=pProcess->NextSiblingElement("trigger");
                    }
                    roomVect.push_back(roomobj);


//                    cout<<"Room name:----->"<<roomobj->name<<endl;
//                    if(roomobj->roomTrigger != NULL){
//                        cout<<"Trigger.print--->"<<roomobj->roomTrigger->message<<endl;
//                        cout<<"Trigger.type----->"<<roomobj->roomTrigger->type<<endl;
//                        //cout<<"Trigger condition"
//                    }
//                   for ( int i=0;i<roomobj->roomCreature.size();i++){
//                       cout<<roomobj->name<<"-------->"<<roomobj->roomCreature[i]->name<<endl;
//                   }
//
//                 cout<<"Description:"<<pParm->FirstChildElement("description")->GetText()<<endl;
//                 cout<<"Item:"<<pParm->FirstChildElement("item")->GetText()<<endl;
                   pParm=pParm->NextSiblingElement("room");
                }


                //Look for items in map (first level)
                pParm=pRoot->FirstChildElement("item");
                while (pParm) {
                    string item_name=pParm->FirstChildElement("name")->GetText();
                    //unnecessary for-loop to search for item
                    int item_exist=0;
                    for(int item=0; item<itemVect.size(); item++) {
                        if (item_name == itemVect[item]->name) {
                            item_exist=1;
                        }
                    }
                    for(int item=0; item<itemVect.size(); item++) {
                        if (item_name == itemVect[item]->name) {
                            //writing
                            if (pParm->FirstChildElement("writing")) {
                                itemVect[item]->writing=pParm->FirstChildElement("writing")->GetText();
                            }
                            //status
                            if (pParm->FirstChildElement("status")) {
                                itemVect[item]->status=pParm->FirstChildElement("status")->GetText();
                            }
                            //turnon
                            if (pParm->FirstChildElement("turnon")) {
                                pProcess=pParm->FirstChildElement("turnon");
                                TurnOn *itemTurnon=new TurnOn();
                                if (pProcess->FirstChildElement("print")) {
                                    itemTurnon->print=pProcess->FirstChildElement("print")->GetText();
                                }
                                if (pProcess->FirstChildElement("action")) {
                                    itemTurnon->action=pProcess->FirstChildElement("action")->GetText();
                                }
                                itemVect[item]->itemTurnOn=itemTurnon;
                            }
                        }//end of if
                    }//end of for
                    if(item_exist==0){
                            Item *item = new Item(item_name);
                            //writing
                            if(pParm->FirstChildElement("writing")){
                                item->writing=pParm->FirstChildElement("writing")->GetText();
                            }
                            //status
                            if(pParm->FirstChildElement("status")){
                                item->status=pParm->FirstChildElement("status")->GetText();
                            }
                            //turnon
                            if(pParm->FirstChildElement("turnon")){
                                pProcess=pParm->FirstChildElement("turnon");
                                TurnOn *itemTurnon = new TurnOn();
                                if(pProcess->FirstChildElement("print")){
                                    itemTurnon->print=pProcess->FirstChildElement("print")->GetText();
                                }
                                if(pProcess->FirstChildElement("action")){
                                    itemTurnon->action=pProcess->FirstChildElement("action")->GetText();
                                }
                                item->itemTurnOn=itemTurnon;
                            }
                            itemVect.push_back(item);

                        }//end of if(item_exist=0)


                    pParm=pParm->NextSiblingElement("item");
                }//end of item parse

//                for (int i =0; i<itemVect.size(); i++){
//                    cout<<itemVect[i]->name<<"------->"<<itemVect[i]->writing<<endl;
//                }
//                cout << endl;

                //Look for containers in map (first level)
                pParm=pRoot->FirstChildElement("container");
                while (pParm) {
                    string container_name=pParm->FirstChildElement("name")->GetText();

                    int container_exist=0;
                    //check if container exists in main container vect
                    for(int c=0; c<containerVect.size(); c++) {
                        if (container_name == containerVect[c]->name) {
                            container_exist=1;
                        }
                    }
                    for(int c=0; c<containerVect.size(); c++) {
                        //it exists
                        if (container_name == containerVect[c]->name) {
                            //status
                            if (pParm->FirstChildElement("status")) {
                                containerVect[c]->status=pParm->FirstChildElement("status")->GetText();
                            }
                            //accept
                            pProcess = pParm->FirstChildElement("accept");
                            while(pProcess) {
                               containerVect[c]->accept.push_back(pProcess->GetText());
                                pProcess=pProcess->NextSiblingElement("accept");
                            }
                            //triggers in container
                            pProcess=pParm->FirstChildElement("trigger");
                            while(pProcess) {
                                Trigger *containerTrigger = new Trigger();
                                containerTrigger->location=containerVect[c]->name;
                                if(pProcess->FirstChildElement("command")){
                                    containerTrigger->command = pProcess->FirstChildElement("command")->GetText();
                                }
                                if(pProcess->FirstChildElement("type")){
                                    containerTrigger->type = pProcess->FirstChildElement("type")->GetText();
                                }
                                if(pProcess->FirstChildElement("print")){
                                    containerTrigger->message = pProcess->FirstChildElement("print")->GetText();
                                }

                                //deeper for actions
                                pApp=pProcess->FirstChildElement("action");
                                while(pApp) {
                                    containerTrigger->action.push_back(pApp->GetText());
                                    pApp=pApp->NextSiblingElement("action");
                                }

                                //deeper for condition
                                pApp=pProcess->FirstChildElement("condition");
                                Condition *containerCondition = new Condition();
                                if(pApp->FirstChildElement("object")){
                                    containerCondition->object = pApp->FirstChildElement("object")->GetText();
                                }
                                if(pApp->FirstChildElement("status")){
                                    containerCondition->status = pApp->FirstChildElement("status")->GetText();
                                }
                                if(pApp->FirstChildElement("owner")){
                                    containerCondition->owner = pApp->FirstChildElement("owner")->GetText();
                                }
                                if(pApp->FirstChildElement("has")){
                                    containerCondition->has = pApp->FirstChildElement("has")->GetText();
                                }
                                containerTrigger->trigCondition = containerCondition;

                                containerVect[c]->containerTrigger = containerTrigger;
                                triggerVect.push_back(containerTrigger);

                                pProcess=pProcess->NextSiblingElement("trigger");
                            }//end of trigger parse

                            //container's items
                            pProcess=pParm->FirstChildElement("item");
                            while (pProcess){
                                string container_item_name = pProcess->GetText();
                                //ignored condition where item is not in itemVect but it is in Container ====>seems impossible
                                for(int item=0; item < itemVect.size(); item++){
                                    if(container_item_name==itemVect[item]->name){
                                        containerVect[c]->containerItem.push_back(itemVect[item]);
                                    }
                                }
                                pProcess=pProcess->NextSiblingElement("item");
                            }



                        }//end of if
                    }//end of for
                    //does not exist
                    if(container_exist==0){
                        Container *container = new Container(container_name);
                        if (pParm->FirstChildElement("status")) {
                            container->status=pParm->FirstChildElement("status")->GetText();
                        }
                        //accept
                        pProcess = pParm->FirstChildElement("accept");
                        while(pProcess) {
                            container->accept.push_back(pProcess->GetText());
                            pProcess=pProcess->NextSiblingElement("accept");
                        }
                        //triggers in container
                        pProcess=pParm->FirstChildElement("trigger");
                        while(pProcess) {
                            Trigger *containerTrigger = new Trigger();
                            containerTrigger->location=container->name;
                            if(pProcess->FirstChildElement("command")){
                                containerTrigger->command = pProcess->FirstChildElement("command")->GetText();
                            }
                            if(pProcess->FirstChildElement("type")){
                                containerTrigger->type = pProcess->FirstChildElement("type")->GetText();
                            }
                            if(pProcess->FirstChildElement("print")){
                                containerTrigger->message = pProcess->FirstChildElement("print")->GetText();
                            }

                            //deeper for actions
                            pApp=pProcess->FirstChildElement("action");
                            while(pApp) {
                                containerTrigger->action.push_back(pApp->GetText());
                                pApp=pApp->NextSiblingElement("action");
                            }

                            //deeper for condition
                            pApp=pProcess->FirstChildElement("condition");
                            Condition *containerCondition = new Condition();
                            if(pApp->FirstChildElement("object")){
                                containerCondition->object = pApp->FirstChildElement("object")->GetText();
                            }
                            if(pApp->FirstChildElement("status")){
                                containerCondition->status = pApp->FirstChildElement("status")->GetText();
                            }
                            if(pApp->FirstChildElement("owner")){
                                containerCondition->owner = pApp->FirstChildElement("owner")->GetText();
                            }
                            if(pApp->FirstChildElement("has")){
                                containerCondition->has = pApp->FirstChildElement("has")->GetText();
                            }
                            containerTrigger->trigCondition = containerCondition;

                            container->containerTrigger = containerTrigger;
                            triggerVect.push_back(containerTrigger);

                            pProcess=pProcess->NextSiblingElement("trigger");
                        }//end of trigger parse

                        //container's items
                        pProcess=pParm->FirstChildElement("item");
                        while (pProcess){
                            string container_item_name = pProcess->GetText();
                            //ignored condition where item is not in itemVect but it is in Container ====>seems impossible
                            for(int item=0; item < itemVect.size(); item++){
                                if(container_item_name==itemVect[item]->name){
                                    container->containerItem.push_back(itemVect[item]);
                                }
                            }
                            pProcess=pProcess->NextSiblingElement("item");
                        }
                        containerVect.push_back(container);

                    }


                    pParm=pParm->NextSiblingElement("container");

                }//end of container parse
//                cout<<"Container"<<endl;


                //Look for creatures in map (first level)
                pParm=pRoot->FirstChildElement("creature");
                while (pParm) {
                    string creature_name=pParm->FirstChildElement("name")->GetText();

                    int creature_exist=0;
                    //check if creature exists in main creature vect
                    for(int c=0; c<creatureVect.size(); c++) {
                        if (creature_name == creatureVect[c]->name) {
                            creature_exist=1;
                        }
                    }
                    for(int c=0; c<creatureVect.size(); c++) {
                        //it exists
                        if (creature_name == creatureVect[c]->name) {

                            //triggers in creature
                            pProcess=pParm->FirstChildElement("trigger");
                            while (pProcess) {
                                Trigger *creatureTrigger=new Trigger();
                                creatureTrigger->location=creatureVect[c]->name;
                                if (pProcess->FirstChildElement("command")) {
                                    creatureTrigger->command=pProcess->FirstChildElement("command")->GetText();
                                }
                                if (pProcess->FirstChildElement("type")) {
                                    creatureTrigger->type=pProcess->FirstChildElement("type")->GetText();
                                }
                                if (pProcess->FirstChildElement("print")) {
                                    creatureTrigger->message=pProcess->FirstChildElement("print")->GetText();
                                }

                                //deeper for actions
                                pApp=pProcess->FirstChildElement("action");
                                while (pApp) {
                                    creatureTrigger->action.push_back(pApp->GetText());
                                    pApp=pApp->NextSiblingElement("action");
                                }

                                //deeper for condition
                                pApp=pProcess->FirstChildElement("condition");
                                Condition *creatureCondition=new Condition();
                                if (pApp->FirstChildElement("object")) {
                                    creatureCondition->object=pApp->FirstChildElement("object")->GetText();
                                }
                                if (pApp->FirstChildElement("status")) {
                                    creatureCondition->status=pApp->FirstChildElement("status")->GetText();
                                }
                                if (pApp->FirstChildElement("owner")) {
                                    creatureCondition->owner=pApp->FirstChildElement("owner")->GetText();
                                }
                                if (pApp->FirstChildElement("has")) {
                                    creatureCondition->has=pApp->FirstChildElement("has")->GetText();
                                }
                                creatureTrigger->trigCondition=creatureCondition;

                                creatureVect[c]->creatureTrigger=creatureTrigger;
                                triggerVect.push_back(creatureTrigger);

                                pProcess=pProcess->NextSiblingElement("trigger");
                            }//end of trigger parse

                            //creature's items
                            pProcess=pParm->FirstChildElement("vulnerability");
                            while (pProcess) {
                                string creature_item_name=pProcess->GetText();
                                //ignored condition where item is not in itemVect but it is in creature ====>seems impossible
                                for (int item=0; item < itemVect.size(); item++) {
                                    if (creature_item_name == itemVect[item]->name) {
                                        creatureVect[c]->vulnerability.push_back(itemVect[item]);
                                    }
                                }
                                pProcess=pProcess->NextSiblingElement("vulnerability");
                            }

                            //creature's attack
                            pProcess=pParm->FirstChildElement("attack");
                            if(pProcess) {
                                Attack *creatureAttack=new Attack();
                                pApp=pProcess->FirstChildElement("print");
                                //attack's print
                                if (pApp) {
                                    creatureAttack->message=pApp->GetText();
                                }

                                //attack's action
                                pApp=pProcess->FirstChildElement("action");
                                while (pApp) {
                                    creatureAttack->action.push_back(pApp->GetText());
                                    pApp=pApp->NextSiblingElement("action");
                                }

                                //attack's condition
                                if (pProcess->FirstChildElement("condition")) {
                                    pApp=pProcess->FirstChildElement("condition");
                                    Condition *attackCondition=new Condition();
                                    if (pApp->FirstChildElement("object")) {
                                        attackCondition->object=pApp->FirstChildElement("object")->GetText();
                                    }
                                    if (pApp->FirstChildElement("status")) {
                                        attackCondition->status=pApp->FirstChildElement("status")->GetText();
                                    }
                                    if (pApp->FirstChildElement("owner")) {
                                        attackCondition->owner=pApp->FirstChildElement("owner")->GetText();
                                    }
                                    if (pApp->FirstChildElement("has")) {
                                        attackCondition->has=pApp->FirstChildElement("has")->GetText();
                                    }
                                    creatureAttack->condition=attackCondition;
                                }
                                creatureVect[c]->creatureAttack=creatureAttack;
                            }
                        }//end of if
                    }//end of for
                    //does not exist
                    if(creature_exist==0){
                        Creature *creature = new Creature(creature_name);
                        //triggers in creature
                        pProcess=pParm->FirstChildElement("trigger");
                        while(pProcess) {
                            Trigger *creatureTrigger = new Trigger();
                            creatureTrigger->location=creature->name;
                            if(pProcess->FirstChildElement("command")){
                                creatureTrigger->command = pProcess->FirstChildElement("command")->GetText();
                            }
                            if(pProcess->FirstChildElement("type")){
                                creatureTrigger->type = pProcess->FirstChildElement("type")->GetText();
                            }
                            if(pProcess->FirstChildElement("print")){
                                creatureTrigger->message = pProcess->FirstChildElement("print")->GetText();
                            }

                            //deeper for actions
                            pApp=pProcess->FirstChildElement("action");
                            while(pApp) {
                                creatureTrigger->action.push_back(pApp->GetText());
                                pApp=pApp->NextSiblingElement("action");
                            }

                            //deeper for condition
                            pApp=pProcess->FirstChildElement("condition");
                            Condition *creatureCondition = new Condition();
                            if(pApp->FirstChildElement("object")){
                                creatureCondition->object = pApp->FirstChildElement("object")->GetText();
                            }
                            if(pApp->FirstChildElement("status")){
                                creatureCondition->status = pApp->FirstChildElement("status")->GetText();
                            }
                            if(pApp->FirstChildElement("owner")){
                                creatureCondition->owner = pApp->FirstChildElement("owner")->GetText();
                            }
                            if(pApp->FirstChildElement("has")){
                                creatureCondition->has = pApp->FirstChildElement("has")->GetText();
                            }
                            creatureTrigger->trigCondition = creatureCondition;

                            creature->creatureTrigger = creatureTrigger;
                            triggerVect.push_back(creatureTrigger);

                            pProcess=pProcess->NextSiblingElement("trigger");
                        }//end of trigger parse

                        //creature's items
                        pProcess=pParm->FirstChildElement("vulnerability");
                        while (pProcess){
                            string creature_item_name = pProcess->GetText();
                            //ignored condition where item is not in itemVect but it is in creature ====>seems impossible
                            for(int item=0; item < itemVect.size(); item++){
                                if(creature_item_name==itemVect[item]->name){
                                    creature->vulnerability.push_back(itemVect[item]);
                                }
                            }
                            pProcess=pProcess->NextSiblingElement("vulnerability");
                        }

                        //creature's attack
                        pProcess=pParm->FirstChildElement("attack");
                        if (pProcess) {
                            Attack *creatureAttack=new Attack();
                            pApp=pProcess->FirstChildElement("print");
                            //attack's print
                            if (pApp) {
                                creatureAttack->message=pApp->GetText();
                            }

                            //attack's action
                            pApp=pProcess->FirstChildElement("action");
                            while (pApp) {
                                creatureAttack->action.push_back(pApp->GetText());
                                pApp=pApp->NextSiblingElement("action");
                            }

                            //attack's condition
                            if (pProcess->FirstChildElement("condition")) {
                                pApp=pProcess->FirstChildElement("condition");
                                Condition *attackCondition=new Condition();
                                if (pApp->FirstChildElement("object")) {
                                    attackCondition->object=pApp->FirstChildElement("object")->GetText();
                                }
                                if (pApp->FirstChildElement("status")) {
                                    attackCondition->status=pApp->FirstChildElement("status")->GetText();
                                }
                                if (pApp->FirstChildElement("owner")) {
                                    attackCondition->owner=pApp->FirstChildElement("owner")->GetText();
                                }
                                if (pApp->FirstChildElement("has")) {
                                    attackCondition->has=pApp->FirstChildElement("has")->GetText();
                                }
                                creatureAttack->condition=attackCondition;
                            }
                            creature->creatureAttack=creatureAttack;
                        }

                        creatureVect.push_back(creature);

                    }


                    pParm=pParm->NextSiblingElement("creature");

                }//end of creature parse

                //cout<<"Creature"<<endl;


            }//end of map parse
            //<----------------------write the main code------------------------------------->
            zork(roomVect,itemVect,containerVect,creatureVect,triggerVect);
    }//end of lead XML success
    else
    {
        std::cout<<("Could not load XML File.");
        return false;
    }
}
