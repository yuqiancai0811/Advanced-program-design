#include "Orders.h"
using namespace std;


#include "Player.h"

// Order class methods
Order::Order() : effect(new string), executed(new bool(false)), name(new string) {}


Order::~Order() {
    delete effect;
    delete executed;
    delete name;
}

Order::Order(const Order& other) {
    effect = new string(*other.effect);
    executed = new bool(*other.executed);
    name = new string(*other.name);
}

Order& Order::operator=(const Order& other) {
    if (this == &other) return *this;

    delete effect;
    delete executed;
    delete name;

    effect = new string(*other.effect);
    executed = new bool(*other.executed);
    name = new string(*other.name);

    return *this;
}

bool Order::validate() const {
    return true;
}

void Order::execute() {
    if (validate()) {
        *effect = "Base order executed";
        *executed = true;
    }
     Notify(this); // Part 5: trigger the writing of the entry in the log file 
}

string Order::toString() const {
    if (*executed && !name->empty()) {
        return "Order: " + *name + " is executed\n";
    } else if (!*executed && !name->empty()) {
        return "Order: " + *name + " is not executed\n";
    } else {
        return "Order: Null\n";
    }
}

ostream& operator<<(ostream& os, const Order& order) {
    os << "Order: " << *order.name << (*order.executed ? " (Executed)" : " (Not executed)") << "\n";
    os << "Effect: " << *order.effect << "\n";
    return os;
}
// Part5: Implementing the stringToLog() function from ILoggable
string Order::stringToLog() const {
      if (*executed && !name->empty()) {
        return "Order: " + *name + " is executed\n";
    } else if (!*executed && !name->empty()) {
        return "Order: " + *name + " is not executed\n";
    } else {
        return "Order: Null\n";
    }
}
// Subclass implementations
deployOrder::deployOrder(int armies, Territory* target, Player* player) {
    *name = "deployOrder";
    this->armies=armies;
    this->target=target;
    this->player=player;
}
deployOrder::deployOrder() {
    *name = "deployOrder";

}


bool deployOrder::validate() const {
    if(std::find(this->player->getOwnedTerritories().begin(),this->player->getOwnedTerritories().end(),this->target) != this->player->getOwnedTerritories().end()) {
        if(this->armies<this->player->getNumberOfReinforcement()) {
            std::cout<<"Not enough armies in the reinforcement pool";
            return true;
        }

    }
    else {
        return false;
    }
}

void deployOrder::execute() {
    if (validate()) {
       this->player->setNumberOfReinforcement(this->player->getNumberOfReinforcement() - this->armies);
        this->target->setArmies(this->target->getArmies() + this->armies);
        this->player->setNumberOfReinforcement(this->player->getNumberOfReinforcement()-this->armies);
    }
}

advanceOrder::advanceOrder(int armies, Territory* source,Territory* target, Player* player) {
    *name = "Advance Order";
    this->armies=armies;
    this->source=source;
    this->target=target;
    this->player=player;
}

bool advanceOrder::validate() const {
    //check if source territories belong to the play and target territory is adjacent to source   or not
    if(std::find(this->player->getOwnedTerritories().begin(),this->player->getOwnedTerritories().end(),this->source) != this->player->getOwnedTerritories().end()&&std::find(this->source->getAdjacentTerritories().begin(),this->source->getAdjacentTerritories().end(),this->target)!=this->source->getAdjacentTerritories().end()) {
       if(this->player->isNegotiating()==true&&this->target->getOwnerPlayer()->isNegotiating()==true) {
           return false;
       }
        else {
            return true;
        }
    }
    else {
        return false;
    }}



void advanceOrder::execute() {
    if (validate()) {
        *executed = true;

        //source and target belong to the same player
        if(this->source->getOwner()==this->target->getOwner()) {
            this->source->setArmies(this->source->getArmies() - this->armies);
            this->target->setArmies(this->target->getArmies() + this->armies);
        }
        else {


            while(this->source->getArmies()>0&&this->target->getArmies()>0) {
                int sourceKill=this->source->getArmies()*0.6;

                int targetKill=this->target->getArmies()*0.7;

                int sourceLeft=source->getArmies()-targetKill;

                int targetLeft=target->getArmies()-sourceKill;

                this->source->setArmies(sourceLeft);

                this->target->setArmies(targetLeft);
            }

            if(source->getArmies()>target->getArmies()) {
                std::cout<<"The attacker captures the territory.";
                this->target->setArmies(source->getArmies());
                this->target->getOwnerPlayer()->removeTerritory(target);
                this->player->addTerritory(target);
                this->winOrNot=true;


            }
            else {
                std::cout<<"The defender defends his/her territory";
            }


        }
        if(winOrNot) {
            //todo: card need to be given
            //..........................
            //..........................
            string orderType[5]={"BOMB","REINFORCEMENT","BLOCKADE","AIRLIFT","DIPLOMACY"};
            static std::mt19937 rng(static_cast<unsigned>(std::time(nullptr))); // Seed with current time
            std::uniform_int_distribution<int> dist(0, 4); // Define range from 0 to 4
            //randomly add a card to hand
            this->player->getHand().addCard(orderType[rng]);
        }
    }

}




bombOrder::bombOrder(Territory* target, Player* player) {
    *name = "Bomb Order";
    this->target=target;
    this->player=player;
}

bool bombOrder::validate() const {
    if(std::find(this->player->getOwnedTerritories().begin(),this->player->getOwnedTerritories().end(),this->target) == this->player->getOwnedTerritories().end()) {
        for(Territory* t:this->player->getOwnedTerritories()) {
            if(std::find(t->getAdjacentTerritories().begin(),t->getAdjacentTerritories().end(),this->target) == t->getAdjacentTerritories().end()&&this->player->isNegotiating()==true&&this->target->getOwnerPlayer()->isNegotiating()==true) {
                return false;
            }
            else {
                return true;
            }
        }
    }
    else {
        return false;
    }
}

void bombOrder::execute() {
    if (validate()) {
        *executed = true;
        this->target->setArmies((this->target->getArmies())/2);
    }
}

blockadeOrder::blockadeOrder(int armies,Player* player, Player* neutral,Territory* target) {
    *name = "Blockade Order";
    this->armies=armies;
    this->player=player;
    this->neutral=neutral;
    this->target=target;

}
blockadeOrder::blockadeOrder(int armies,Player* player, Territory* target) {
    *name = "Blockade Order";
    this->armies=armies;
    this->player=player;
    this->target=target;
    Player neutral=Player();

}

bool blockadeOrder::validate() const {
    if(std::find(this->player->getOwnedTerritories().begin(),this->player->getOwnedTerritories().end(),this->target) != this->player->getOwnedTerritories().end()) {
        return true;
    }
    else {
        return false;
    }
}

void blockadeOrder::execute() {
    if (validate()) {
        *executed = true;
        this->target->setArmies((this->target->getArmies())*2);
        this->target->setOwner(this->neutral->getName());
        this->neutral->addTerritory(this->target);
        this->player->removeTerritory(this->target);
    }
}

airliftOrder::airliftOrder(int armies,Territory* source,Territory* target,Player* player) {
    *name = "Airlift Order";
    this->armies=armies;
    this->source=source;
    this->target=target;
    this->player=player;
}

bool airliftOrder::validate() const {
    if(std::find(this->player->getOwnedTerritories().begin(),this->player->getOwnedTerritories().end(),this->target) != this->player->getOwnedTerritories().end()&&std::find(this->player->getOwnedTerritories().begin(),this->player->getOwnedTerritories().end(),this->source) != this->player->getOwnedTerritories().end()) {
        return true;
    }
    else {
        return false;
    }

}

void airliftOrder::execute() {
    if (validate()) {
        *executed = true;
        this->source->setArmies(this->source->getArmies() - this->armies);
        this->target->setArmies(this->target->getArmies() + this->armies);
    }
}

//player is the player who issues this order
negotiateOrder::negotiateOrder(Player* player,Player* enemy) {
    *name = "Negotiate Order";
    this->player=player;
    this->enemy=enemy;

}

bool negotiateOrder::validate() const {
    if(player!=enemy) {
        return true;
    }
    else {
        return false;
    }

}

void negotiateOrder::execute() {
    if (validate()) {
        *executed = true;
        this->player->setNegotiate(true);
        this->enemy->setNegotiate(true);
    }
}

// orderList methods
orderList::~orderList() {
    for (Order* order : orders) {
        delete order;
    }
}

void orderList::addOrder(Order* order) {
    orders.push_back(order);
    Notify(this); // Part 5: trigger the writing of the entry in the log file 

}

void orderList::removeOrder(int index) {
    if (index >= 0 && index < orders.size()) {
        delete orders[index];
        orders.erase(orders.begin() + index);
    }
}

void orderList::moveOrder(int oldIndex, int newIndex) {
    if (oldIndex >= 0 && oldIndex < orders.size() &&
        newIndex >= 0 && newIndex < orders.size()) {
        swap(orders[oldIndex], orders[newIndex]);
    }
}

void orderList::showAllOrders() const {
    for (const auto& order : orders) {
        cout << order->toString();
    }
}

/*--------------- Helper function for executeOrdersPhase() in Part3 -----------------*/
bool orderList::hasMoreOrders() const {
    return !orders.empty();
}

Order* orderList::getNextOrder() {
    if (!orders.empty()) {
        Order* nextOrder = orders.front();
        orders.erase(orders.begin());
        return nextOrder;
    }
    return nullptr;
}
/*-------------------------------------------------------------------------------*/

ostream& operator<<(ostream& os, const orderList& ordersList) {
    for (const auto& order : ordersList.orders) {
        os << *order;
    }
    return os;
}

// Part5: IImplementing the stringToLog() function from ILoggable
string orderList::stringToLog() const {
    string log = "OrderList: Total orders = " + to_string(orders.size()) + "\n";

    for (size_t i = 0; i < orders.size(); ++i) {
        log += " - Order " + to_string(i + 1) + ": " + orders[i]->toString() + "\n";
    }

    return log;
}
