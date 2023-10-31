#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

#include "Orders.h"

class Chef
{
public:
	Chef(
		std::queue<Orders>& ingredientsQueueP,
		std::queue<Orders>& mealsQueueP,
		std::mutex& ingredientsMutex,
		std::mutex& displayMutex,
		std::mutex& mealMutex,
		std::condition_variable& chefCondition,
		std::condition_variable& waiterCondition);

	~Chef();

private:
	std::queue<Orders>& ingredientsQueue;
	std::queue<Orders>& mealsQueue;

	std::mutex& ingredientsMut;
	std::mutex& displayMut;
	std::mutex& mealMut;
	std::condition_variable& chefCond;
	std::condition_variable& waiterCond;

	void PrepareMeal();
};

