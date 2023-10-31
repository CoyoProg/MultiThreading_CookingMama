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
		std::mutex& preparationMutex,
		std::mutex& displayMutex,
		std::condition_variable& customerCondition);
	~Customer();

private:
	int CustomerID{ 0 };

	std::queue<Orders>& preparationQueue;

	std::mutex& preparationMut;
	std::mutex& displayMut;
	std::condition_variable& customerCond;

	void WalkIn();
	Orders RandomMeal();
};

