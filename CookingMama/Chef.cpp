#include "Chef.h"
#include <iostream>

Chef::Chef(std::queue<Orders>& ingredientsQueueP, std::queue<Orders>& mealsQueueP, std::mutex& ingredientsMutex, std::mutex& displayMutex, std::mutex& mealMutex, std::condition_variable& chefCondition, std::condition_variable& waiterCondition) :
    ingredientsQueue(ingredientsQueueP),
    mealsQueue(mealsQueueP),
    ingredientsMut(ingredientsMutex),
    displayMut(displayMutex),
    mealMut(mealMutex),
    chefCond(chefCondition),
    waiterCond(waiterCondition)
{
    std::thread cheefThread(&Chef::PrepareMeal, this);
    cheefThread.detach();
}

Chef::~Chef()
{
}

void Chef::PrepareMeal()
{
    {
        std::unique_lock<std::mutex> lk(displayMut);
        std::cout << "The Chef is waiting for customers. \n";
    }

    while (true)
    {
        std::unique_lock<std::mutex> lk(ingredientsMut);
        chefCond.wait(lk, [&] { return !ingredientsQueue.empty(); });

        /* Small delay to not instantly cook the meal when receiving the ingredients */
        std::this_thread::sleep_for(std::chrono::seconds(2));   // Sleep for 1 seconds

        {
            std::unique_lock<std::mutex> lk(displayMut);
            std::cout << "The chef is preparing the meal of customer " << ingredientsQueue.front().GetOrderNumber() << " \n";
        }

        std::this_thread::sleep_for(std::chrono::seconds(4));   // Sleep for 4 seconds

        {
            std::unique_lock<std::mutex> lk(displayMut);
            std::cout << "The meal of customer " << ingredientsQueue.front().GetOrderNumber() << " is ready. \n";
        }

        Orders data = ingredientsQueue.front();
        ingredientsQueue.pop();

        lk.unlock();

        /* Lock the order */
        {
            std::lock_guard<std::mutex> lk(mealMut);
            mealsQueue.push(data);
        }

        waiterCond.notify_one();
    }
}
