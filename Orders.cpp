#include "Map.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <fstream>

#include "Orders.h"
#include "Cards.h"
#include "Player.h"


using namespace std;



// Order class methods

std::string getRandomCardType() {
    // Random number generation setup
    std::random_device rd;  // Seed generator
    std::mt19937 gen(rd()); // Mersenne Twister engine
    std::uniform_int_distribution<> dist(1, 5);  // Range 1 to 5 for each card type

    // Select a card type based on the random number
    switch (dist(gen)) {
        case 1: return "BOMB";
        case 2: return "REINFORCEMENT";
        case 3: return "BLOCKADE";
        case 4: return "AIRLIFT";
        case 5: return "DIPLOMACY";
        default: return "";  // Should never happen
    }
}

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

deployOrder::~deployOrder(){}


bool deployOrder::validate() const {
    if(std::find(this->player->getOwnedTerritories().begin(),this->player->getOwnedTerritories().end(),this->target) != this->player->getOwnedTerritories().end()) {
        if(this->armies>this->player->getNumberOfReinforcement()) {
            std::cout<<"Not enough armies in the reinforcement pool";
            return false;
        }
        else {
            return true;
        }
    // return true;
    }
    else {
        return false;
    }
}

void deployOrder::execute() {
    if (validate()) {
        std::cout<<"\nNow running deploy order\n";
        std::cout<<"Origin armies in target territory:"<<this->target->getArmies()<<"\n";
       this->player->setNumberOfReinforcement(this->player->getNumberOfReinforcement() - this->armies);
        this->target->setArmies(this->target->getArmies() + this->armies);
        // this->player->setNumberOfReinforcement(this->player->getNumberOfReinforcement()-this->armies);
        std::cout<<"Have taken "<<this->armies<<" from reinforcement pool to the target territory\n";
        std::cout<<"After the operation:"<<this->target->getArmies()<<"\n";

    }
}

advanceOrder::advanceOrder(int armies, Territory* source,Territory* target, Player* player) {
    *name = "Advance Order";
    this->armies=armies;
    this->source=source;
    this->target=target;
    this->player=player;
}
advanceOrder::~advanceOrder() {

    // Destructor body (can be empty)
}

bool advanceOrder::validate() const {
    //check if source territories belong to the player and target territory is adjacent to source   or not
    if(std::find(this->player->getOwnedTerritories().begin(),this->player->getOwnedTerritories().end(),this->source) != this->player->getOwnedTerritories().end()&&std::find(this->source->getAdjacentTerritories().begin(),this->source->getAdjacentTerritories().end(),this->target)!=this->source->getAdjacentTerritories().end()) {



        if(this->player->isNegotiating()==true) {
            std::cout<<"Invalid order because negotiation\n";
            return false;
        }
        else {

            return true;
        }

    }
    else {
        std::cout<<"Invalid order advance\n";

        return false;
    }}



void advanceOrder::execute() {
    if (validate()) {
        std::cout<<"\nNow running advance order\n";

        *executed = true;

        //source and target belong to the same player
        if(this->source->getOwner()==this->target->getOwner()) {
            std::cout<<"Source Owner"<<this->source->getOwner()<<"\n";
            std::cout<<"Target Owner"<<this->target->getOwner()<<"\n";

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
                this->target->setPlayer(player);

                this->player->addTerritory(target);
                this->winOrNot=true;


            }
            else {
                std::cout<<"\nSource Army"<<this->source->getArmies();
                std::cout<<"\nTarget Army"<<this->target->getArmies();

                std::cout<<"The defender defends his/her territory";
                this->source->setArmies(0);
            }


        }
        if(winOrNot) {
            //todo: card need to be given
            //..........................
            //..........................

            Card* card=new Card(getRandomCardType());
            this->player->getHand().addCard(card);
        }
    }

}




bombOrder::bombOrder(Territory* target, Player* player) {
    *name = "Bomb Order";
    this->target=target;
    this->player=player;
}

bool bombOrder::validate() const {
    if(std::find(this->player->getOwnedTerritories().begin(),this->player->getOwnedTerritories().end(),this->target) != this->player->getOwnedTerritories().end()) {


        bool isValidOrder = false; // Flag to track if the order is valid

        for (Territory* t : this->player->getOwnedTerritories()) {


            // if (std::find(t->getAdjacentTerritoryNames().begin(), t->getAdjacentTerritoryNames().end(), this->target->getName()) == t->getAdjacentTerritoryNames().end() &&
            //     this->player->isNegotiating() == true &&
            //     this->target->getOwnerPlayer()->isNegotiating() == true) {
            //     std::cout << "Invalid order bomb1\n";
            //     isValidOrder = false;
            //     break;  // Exit loop if condition is met
            //     } else {
            //         isValidOrder = true;
            //     }
            for(Territory* i : t->getAdjacentTerritories()) {
                if(i->getName()==this->target->getName()) {
                    isValidOrder = true;
                }

            }
        }

        return isValidOrder;
    }
    else {
        return false;
    }
    return false;
}

void bombOrder::execute() {
    if (validate()) {
        std::cout<<"\nNow running bomb order\n";

        *executed = true;
        std::cout<<"\nTarget territory before bomb:"<<this->target->getArmies();

        this->target->setArmies((this->target->getArmies())/2);
        std::cout<<"\nTarget territory after bomb:"<<this->target->getArmies();
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
        if(this->player->getHand().hasCardType("BLOCKADE")) {
            return true;
        }
        else {

            return false;
        }    }
    else {
        std::cout<<"Invalid order blockade\n";
        return false;
    }
}

void blockadeOrder::execute() {
    if (validate()) {
        std::cout<<"\nNow running blockade order\n";

        *executed = true;
        std::cout<<"Running the blockade order,\n";
        std::cout<<"The armies number:"<<this->target->getArmies()<<"\n";

        this->target->setArmies((this->target->getArmies())*2);
        this->target->setOwner(this->neutral->getName());
        this->neutral->addTerritory(this->target);
        this->player->removeTerritory(this->target);
        std::cout<<"The armies have doubled:"<<this->target->getArmies()<<"\n";

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
        if(this->player->getHand().hasCardType("Airlift")) {
            std::cout<<"valid order airlift\n";

            return true;
        }
        else {
            std::cout<<"Invalid order airlift\n";

            return false;
        }

    }
    else {
        std::cout<<"Invalid order airlift\n";
        return false;
    }

}

void airliftOrder::execute() {

    if (validate()) {
        std::cout<<"\nNow running airlift order\n";

        *executed = true;
        std::cout<<"\nSource territory armies:"<<this->source->getArmies();
        std::cout<<"\nTarget territory armies:"<<this->target->getArmies();


        this->source->setArmies(this->source->getArmies() - this->armies);
        this->target->setArmies(this->target->getArmies() + this->armies);

        std::cout<<"\nSource territory armies(after airlift):"<<this->source->getArmies();
        std::cout<<"\nTarget territory armies(after airlift):"<<this->target->getArmies();
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
        std::cout<<"valid order negotiation\n";

        return true;
    }
    else {
        std::cout<<"Invalid order negotiation\n";
        return false;
    }

}

void negotiateOrder::execute() {
    if (validate()) {
        std::cout<<"\nNow running negotiate order\n";

        *executed = true;
        this->player->setNegotiate(true);
        this->enemy->setNegotiate(true);
    }
}

// orderList methods
orderList::orderList() {
    this->orders = vector<Order*>();
}

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
