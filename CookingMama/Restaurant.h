#pragma once

#include <iostream>
#include <vector>
#include <thread>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "Orders.h"

class Restaurant 
{
public:
    Restaurant() {};
    ~Restaurant() {};
    void startSimulation();

private:
    std::queue<Orders> preparationQueue;
    std::queue<Orders> ordersQueue;
    std::queue<Orders> mealsQueue;
    std::queue<Orders> readyMeals;
    std::queue<Orders> ingredientsQueue;

    std::mutex preparationMut;
    std::mutex orderMut;
    std::mutex mealMut;
    std::mutex eatMut;
    std::mutex ingredientsMut;
    std::mutex displayMut;

    std::condition_variable customerCond;
    std::condition_variable customerCond2;
    std::condition_variable cookCond;
    std::condition_variable waiterCond;
    std::condition_variable chefCond;

    void AddClient();
    inline bool isRestaurantFull() { return customerCount >= 5; }
    int customerCount{0};
};

