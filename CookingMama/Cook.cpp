#include "Cook.h"
#include <iostream>

Cook::Cook(std::queue<Orders>& ingredientsQueueP, std::queue<Orders>& ordersQueueP, std::mutex& ingredientsMutex, std::mutex& orderMutex, std::mutex& displayMutex, std::condition_variable& chefCondition, std::condition_variable& cookCondition) :
    ingredientsQueue(ingredientsQueueP),
    ordersQueue(ordersQueueP),
    ingredientsMut(ingredientsMutex),
    orderMut(orderMutex),
    displayMut(displayMutex),
    chefCond(chefCondition),
    cookCond(cookCondition)
{
    std::thread cookThread(&Cook::PrepareIngredients, this);
    cookThread.detach();
}

Cook::~Cook()
{
}

void Cook::PrepareIngredients()
{
    {
        std::unique_lock<std::mutex> lk(displayMut);
        std::cout << "The Cook is waiting for customers. \n";
    }

    while (true)
    {
        std::unique_lock<std::mutex> lk(orderMut);
        cookCond.wait(lk, [&] { return !ordersQueue.empty(); });

        /* Small delay to not instantly start the order when receiving it */
        std::this_thread::sleep_for(std::chrono::seconds(3));   // Sleep for 2 seconds

        {
            std::unique_lock<std::mutex> lk(displayMut);
            std::cout << "The cook is preparing the ingredients ( " 
                << ordersQueue.front().ingredients[0] << " "
                << ordersQueue.front().ingredients[1] << " "
                << ordersQueue.front().ingredients[2] << " "
                << ")\n";
        }

        std::this_thread::sleep_for(std::chrono::seconds(4));   // Sleep for 3 seconds

        {
            std::unique_lock<std::mutex> lk(displayMut);
            std::cout << "The cook finished preparing the ingredients ( " 
                << ordersQueue.front().ingredients[0] << " "
                << ordersQueue.front().ingredients[1] << " "
                << ordersQueue.front().ingredients[2] << " "
                << ")\n";
        }

        Orders data = ordersQueue.front();
        ordersQueue.pop();

        lk.unlock();

        /* Lock the ingredients */
        {
            std::lock_guard<std::mutex> lk(ingredientsMut);
            ingredientsQueue.push(data);
        }

        /* Give the ingredients to the chef here */
        chefCond.notify_one();
    }
}
