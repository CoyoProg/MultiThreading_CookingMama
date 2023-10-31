#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

#include "Orders.h"

class Waiter
{
public:
	Waiter(
		std::queue<Orders>& preparationQueueP,
		std::queue<Orders>& ordersQueueP,
		std::queue<Orders>& mealsQueue,
		std::mutex& preparationMutex,
		std::mutex& orderMutex,
		std::mutex& mealMutex,
		std::mutex& eatMutex,
		std::mutex& displayMutex,
		std::condition_variable& customerCondition,
		std::condition_variable& customerCondition2,
		std::condition_variable& waiterCondition,
		std::condition_variable& cookCondition);

	~Waiter();

private:
	std::queue<Orders>& preparationQueue;
	std::queue<Orders>& ordersQueue;
	std::queue<Orders>& mealsQueue;

	std::mutex& preparationMut;
	std::mutex& orderMut;
	std::mutex& mealMut;
	std::mutex& eatMut;
	std::mutex& displayMut;
	std::condition_variable& customerCond;
	std::condition_variable& customerCond2;
	std::condition_variable& waiterCond;
	std::condition_variable& cookCond;

	std::mutex busyMut;
	bool isBusy{ false };
	void TakeOrder();
	void BringOrder();
};

