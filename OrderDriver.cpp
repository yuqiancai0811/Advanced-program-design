#include "OrderDriver.h"
#include "Orders.h"

void testOrdersLists() {

    orderList ordersList;

    // Creating orders
    ordersList.addOrder(new deployOrder());
    ordersList.addOrder(new advanceOrder());
    ordersList.addOrder(new bombOrder());
    ordersList.addOrder(new blockadeOrder());
    ordersList.addOrder(new airliftOrder());
    ordersList.addOrder(new negotiateOrder());

    std::cout << "Orders before execution:\n";
    ordersList.showAllOrders();

    // Execute some orders
    ordersList.removeOrder(0); // Remove first order (DeployOrder)
    ordersList.moveOrder(2, 0); // Move Blockade order to the first position

    std::cout << "\n\nOrders after modifications (move and remove):\n";
    ordersList.showAllOrders();

}

// int main() {
//     testOrdersLists();
//     return 0;
// }
