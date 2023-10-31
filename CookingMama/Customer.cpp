#include "Customer.h"
#include <iostream>
#include <random>
#include <functional>

Customer::Customer(int customerIdP, std::queue<Orders>& preparationQueueP, std::queue<Orders>& readyMealsP, std::mutex& preparationMutex, std::mutex& eatMutex, std::mutex& displayMutex, std::condition_variable& customerCondition, std::condition_variable& customerCondition2) :
    preparationQueue(preparationQueueP),
    readyMeals(readyMealsP),
    CustomerID(customerIdP),
    preparationMut(preparationMutex),
    eatMut(eatMutex),
    displayMut(displayMutex),
    customerCond(customerCondition),
    customerCond2(customerCondition2)
{
    std::thread walkinThread(&Customer::WalkIn, this);
    walkinThread.detach();

    std::thread eatThread(&Customer::Eat, this);
    eatThread.detach();
}

Customer::~Customer()
{
}

void Customer::WalkIn()
{
    /* Customer come into the restaurant and choose what meal to order */
    myMeal = RandomMeal();

    /* Lock the display */
    {
        std::lock_guard<std::mutex> lk(displayMut);
        std::cout << "Customer " << CustomerID << " walks in.\n";
    }

    std::this_thread::sleep_for(std::chrono::seconds(2)); // Sleep for 2 seconds

    {
        std::lock_guard<std::mutex> lk(displayMut);
        std::cout << "Customer " << CustomerID << " choosed a meal ("
            << myMeal.ingredients[0]
            << " " << myMeal.ingredients[1]
            << " " << myMeal.ingredients[2]
            << " )\n";
    }

    /* Lock the order */
    {
        std::lock_guard<std::mutex> lk(preparationMut);
        preparationQueue.push(myMeal);
    }

    customerCond.notify_one();
}

void Customer::Eat()
{
    while (true)
    {
        std::unique_lock<std::mutex> lk(eatMut);
        customerCond2.wait(lk, [&] 
            { 
                //std::cout << "NUMBER MEAL: " << readyMeals.front().isReady << "\n";
                //std::cout << "NUMBER CUSTOMER: " << CustomerID << "\n";
                return (!readyMeals.empty() && readyMeals.front().GetOrderNumber() == CustomerID);
            });

        std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate eating for 1 seconds

        {
            std::unique_lock<std::mutex> lk(displayMut);
            std::cout << "Customer " << CustomerID << " is eating."
                << " ( " << readyMeals.front().ingredients[0]
                << " " << readyMeals.front().ingredients[1]
                << " " << readyMeals.front().ingredients[2]
                << " )\n";
        }

        readyMeals.pop();

        std::this_thread::sleep_for(std::chrono::seconds(3)); // Simulate eating for 3 seconds

        {
            std::unique_lock<std::mutex> lk(displayMut);
            std::cout << "Customer " << CustomerID << " finished eating.\n";
        }

        lk.unlock();
    }
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
