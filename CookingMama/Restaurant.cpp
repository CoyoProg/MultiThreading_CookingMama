#include "Restaurant.h"
#include <random>

#include "Customer.h"
#include "Waiter.h"
#include "Cook.h"
#include "Chef.h"

void Restaurant::startSimulation()
{
    /** Add customers */
    std::thread clientThread(&Restaurant::AddClient, this);
    clientThread.detach();

    /** Add a Waiter*/
    Waiter waiter(
        preparationQueue,
        ordersQueue,
        mealsQueue,
        preparationMut,
        orderMut,
        mealMut,
        eatMut,
        displayMut,
        customerCond,
        customerCond2,
        waiterCond,
        cookCond);

    /** Add A Cook */
    Cook cooker(
        ingredientsQueue,
        ordersQueue,
        ingredientsMut,
        orderMut,
        displayMut,
        chefCond,
        cookCond);

    /** Add A Chef */
    Chef chef(
        ingredientsQueue,
        mealsQueue,
        ingredientsMut,
        displayMut,
        mealMut,
        chefCond,
        waiterCond);
}

void Restaurant::AddClient()
{
    /* Time for the first customers to walk in */
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Sleep for 1 seconds

    while (!isRestaurantFull())
    {
        customerCount++;
        Customer* customer = new Customer(customerCount, preparationQueue, preparationMut, eatMut, displayMut, customerCond, customerCond2);

        /* Time between two customers */
        std::this_thread::sleep_for(std::chrono::seconds(20)); // Sleep for 15 seconds
    }
}