#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

#include "Orders.h"

class Customer
{
public:
	Customer(int customerIdP, 
		std::queue<Orders>& preparationQueueP,
		std::queue<Orders>& readyMealsP,
		std::mutex& preparationMutex,
		std::mutex& eatMutex,
		std::mutex& displayMutex,
		std::condition_variable& customerCondition,
		std::condition_variable& customerCondition2);
	~Customer();

private:
	int CustomerID{ 0 };
	bool IsOrderReceived{ false };
	Orders myMeal;

	std::queue<Orders>& preparationQueue;
	std::queue<Orders>& readyMeals;

	std::mutex& preparationMut;
	std::mutex& eatMut;
	std::mutex& displayMut;
	std::condition_variable& customerCond;
	std::condition_variable& customerCond2;

	void WalkIn();
	void Eat();
	Orders RandomMeal();

};

