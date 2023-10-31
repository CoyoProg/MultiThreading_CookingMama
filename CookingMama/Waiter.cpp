#include "Waiter.h"
#include <iostream>

Waiter::Waiter(std::queue<Orders>& preparationQueueP, std::queue<Orders>& ordersQueueP, std::queue<Orders>& mealsQueueP, std::mutex& preparationMutex, std::mutex& orderMutex, std::mutex& mealMutex, std::mutex& eatMutex, std::mutex& displayMutex, std::condition_variable& customerCondition, std::condition_variable& customerCondition2, std::condition_variable& waiterCondition, std::condition_variable& cookCondition) :
    preparationQueue(preparationQueueP),
    mealsQueue(mealsQueueP),
    ordersQueue(ordersQueueP),
    preparationMut(preparationMutex),
    orderMut(orderMutex),
    mealMut(mealMutex),
    eatMut(eatMutex),
    displayMut(displayMutex),
    customerCond(customerCondition),
    customerCond2(customerCondition2),
    waiterCond(waiterCondition),
    cookCond(cookCondition)
{
    std::thread takeOrderThread(&Waiter::TakeOrder, this);
    takeOrderThread.detach();

    std::thread bringOrderThread(&Waiter::BringOrder, this);
    bringOrderThread.detach();
}

Waiter::~Waiter()
{
}

void Waiter::TakeOrder()
{
    {
        std::unique_lock<std::mutex> lk(displayMut);
        std::cout << "The Waiter is waiting for customers. \n";
    }

    while (true)
    {
        std::unique_lock<std::mutex> lk(preparationMut);
        customerCond.wait(lk, [&] { return !preparationQueue.empty() && !isBusy; });

        isBusy = true;

        std::this_thread::sleep_for(std::chrono::seconds(1)); // Sleep for 1 second
        {
            std::unique_lock<std::mutex> lk(displayMut);
            std::cout << "Waiter is running to customer " << preparationQueue.front().GetOrderNumber() << " !\n";
        }


        std::this_thread::sleep_for(std::chrono::seconds(2)); // Sleep for 2 second
        {
            std::unique_lock<std::mutex> lk(displayMut);
            std::cout << "Waiter took the order of customer " << preparationQueue.front().GetOrderNumber() << "\n";
        }

        Orders data = preparationQueue.front();
        preparationQueue.pop();

        isBusy = false;
        lk.unlock();

        /* Lock the order */
        {
            std::lock_guard<std::mutex> lk(orderMut);
            ordersQueue.push(data);
        }

        /* Send the order to the kitchen here */
        cookCond.notify_one();
    }
}

void Waiter::BringOrder()
{
    while (true)
    {
        std::unique_lock<std::mutex> lk(mealMut);
        waiterCond.wait(lk, [&] { return !mealsQueue.empty() && !isBusy; });

        isBusy = true;

        std::this_thread::sleep_for(std::chrono::seconds(1)); // Sleep for 1 second
        {
            std::unique_lock<std::mutex> lk(displayMut);
            std::cout << "Waiter is running to customer " << mealsQueue.front().GetOrderNumber() << " !\n";
        }


        std::this_thread::sleep_for(std::chrono::seconds(2)); // Sleep for 2 second
        {
            std::unique_lock<std::mutex> lk(displayMut);
            std::cout << "Waiter give the order of customer " << mealsQueue.front().GetOrderNumber() << "\n";
        }

        //Orders data = mealsQueue.front();
        mealsQueue.pop();

        isBusy = false;
        lk.unlock();

        /* Send the order to the kitchen here */
        customerCond2.notify_one();
    }
}
