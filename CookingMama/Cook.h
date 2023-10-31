#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

#include "Orders.h"


class Cook
{
public:
	Cook(
		std::queue<Orders>& ingredientsQueueP,
		std::queue<Orders>& ordersQueueP,
		std::mutex& ingredientsMutex,
		std::mutex& orderMutex,
		std::mutex& displayMutex,
		std::condition_variable& chefCondition,
		std::condition_variable& cookCondition);

	~Cook();

private:
	std::queue<Orders>& ingredientsQueue;
	std::queue<Orders>& ordersQueue;

	std::mutex& ingredientsMut;
	std::mutex& orderMut;
	std::mutex& displayMut;
	std::condition_variable& cookCond;
	std::condition_variable& chefCond;

	void PrepareIngredients();
};

