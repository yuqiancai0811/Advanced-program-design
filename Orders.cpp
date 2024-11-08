#include "Orders.h"
using namespace std;


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
deployOrder::deployOrder() {
    *name = "deployOrder";
}

bool deployOrder::validate() const {
    return true;
}

void deployOrder::execute() {
    if (validate()) {
        *effect = "Deploy troops";
        *executed = true;
    }
}

advanceOrder::advanceOrder() {
    *name = "Advance Order";
}

bool advanceOrder::validate() const {
    return true;
}

void advanceOrder::execute() {
    if (validate()) {
        *effect = "Advance troops";
        *executed = true;
    }
}

bombOrder::bombOrder() {
    *name = "Bomb Order";
}

bool bombOrder::validate() const {
    return true;
}

void bombOrder::execute() {
    if (validate()) {
        *effect = "Bomb troops";
        *executed = true;
    }
}

blockadeOrder::blockadeOrder() {
    *name = "Blockade Order";
}

bool blockadeOrder::validate() const {
    return true;
}

void blockadeOrder::execute() {
    if (validate()) {
        *effect = "Blockade troops";
        *executed = true;
    }
}

airliftOrder::airliftOrder() {
    *name = "Airlift Order";
}

bool airliftOrder::validate() const {
    return true;
}

void airliftOrder::execute() {
    if (validate()) {
        *effect = "Airlift troops";
        *executed = true;
    }
}

negotiateOrder::negotiateOrder() {
    *name = "Negotiate Order";
}

bool negotiateOrder::validate() const {
    return true;
}

void negotiateOrder::execute() {
    if (validate()) {
        *effect = "Negotiate troops";
        *executed = true;
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
