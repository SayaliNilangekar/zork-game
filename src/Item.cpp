#include "Item.h"
#include <string>
using namespace std;
Item::Item(string value) {
    this->name=value;
    itemTurnOn=NULL;
}