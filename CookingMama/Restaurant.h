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
    Restaurant();
    ~Restaurant();
    void startSimulation();

private:
    std::queue<Orders> preparationQueue;
    std::queue<Orders> ordersQueue;
    std::queue<Orders> ingredientsQueue;
    inline bool isRestaurantFull() { return customerCount >= 5; }

    void AddClient();

    void CustomerWalkIn();
    Orders RandomMeal();
    int customerCount{0};

    void TakeOrder();
    void PrepareIngredients();
    void PrepareMeal();
    void BringOrder();
    void EatMeal();

    std::mutex preparationMut;
    std::mutex orderMut;
    std::mutex ingredientsMut;
    std::mutex displayMut;
    std::condition_variable customerCond;
    std::condition_variable cookCond;
    std::condition_variable chefCond;


    std::condition_variable chefCV;
    std::condition_variable chiefCV;
};

