#include "Customer.h"
#include <iostream>
#include <random>
#include <functional>

Customer::Customer(int customerIdP, std::queue<Orders>& preparationQueueP, std::mutex& preparationMutex, std::mutex& displayMutex, std::condition_variable& customerCondition) :
    preparationQueue(preparationQueueP),
    CustomerID(customerIdP),
    preparationMut(preparationMutex),
    displayMut(displayMutex),
    customerCond(customerCondition)
{
    std::thread waiterThread(&Customer::WalkIn, this);
    waiterThread.detach();
}

Customer::~Customer()
{
}

void Customer::WalkIn()
{
    /* Customer come into the restaurant and choose what meal to order */
    Orders data = RandomMeal();

    /* Lock the display */
    {
        std::lock_guard<std::mutex> lk(displayMut);
        std::cout << "Customer " << CustomerID << " walks in.\n";
    }

    std::this_thread::sleep_for(std::chrono::seconds(2)); // Sleep for 2 seconds

    {
        std::lock_guard<std::mutex> lk(displayMut);
        std::cout << "Customer " << CustomerID << " choosed a meal ("
            << data.ingredients[0]
            << " " << data.ingredients[1]
            << " " << data.ingredients[2]
            << " )\n";
    }

    /* Lock the order */
    {
        std::lock_guard<std::mutex> lk(preparationMut);
        preparationQueue.push(data);
    }

    customerCond.notify_one();

    /* Time between two customers */
    std::this_thread::sleep_for(std::chrono::seconds(15)); // Sleep for 10 seconds
}

Orders Customer::RandomMeal()
{
    Orders order(CustomerID);

    /* Generate 3 random ingredient numbers out of 10
       ChatGPT random style to change from rand */
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 10); // Assuming ingredient numbers are from 1 to 10

    for (int i = 0; i < 3; i++) {
        order.ingredients[i] = dist(gen);
    }

    return order;
}
