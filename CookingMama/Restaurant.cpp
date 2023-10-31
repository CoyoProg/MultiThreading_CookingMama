#include "Restaurant.h"
#include <random>

#include "Customer.h"

Restaurant::Restaurant()
{
}

Restaurant::~Restaurant()
{
}

void Restaurant::startSimulation()
{
    std::thread clientThread(&Restaurant::AddClient, this);
    clientThread.detach();



    // Create threads for different components and detach them
    std::thread waiterThread(&Restaurant::TakeOrder, this);
    waiterThread.detach();

    //std::thread customerThread(&Restaurant::CustomerWalkIn, this);
    //customerThread.detach();

    std::thread cookThread(&Restaurant::PrepareIngredients, this);
    cookThread.detach();

    std::thread chefThread(&Restaurant::PrepareMeal, this);
    chefThread.detach();
}

void Restaurant::AddClient()
{
    /* Time for the first customers to walk in */
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Sleep for 1 seconds

    while (!isRestaurantFull())
    {
        customerCount++;
        Customer* customer = new Customer(customerCount, preparationQueue, preparationMut, displayMut, customerCond);

        /* Time between two customers */
        std::this_thread::sleep_for(std::chrono::seconds(15)); // Sleep for 15 seconds
    }
}

void Restaurant::CustomerWalkIn()
{
    /* Time for the first customers to walk in */
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Sleep for 1 seconds

    /* Customer come into the restaurant and choose what meal to order */
    while (!isRestaurantFull()) 
    {
        customerCount++;

        Orders data = RandomMeal();

        /* Lock the display */
        {
            std::lock_guard<std::mutex> lk(displayMut);
            std::cout << "Customer " << customerCount << " walks in.\n";
        }

        std::this_thread::sleep_for(std::chrono::seconds(2)); // Sleep for 2 seconds

        {
            std::lock_guard<std::mutex> lk(displayMut);
            std::cout << "Customer " << customerCount << " choosed a meal ("
                << data.ingredients[0]
                << " " << data.ingredients[1]
                << " " << data.ingredients[2]
                << " )\n";
        }

        /* Lock the order */
        {
            std::lock_guard<std::mutex> lk(preparationMut);
            preparationQueue.push(data);
        }

        customerCond.notify_one();

        /* Time between two customers */
        std::this_thread::sleep_for(std::chrono::seconds(15)); // Sleep for 10 seconds
    }
}

Orders Restaurant::RandomMeal() 
{
    Orders order(customerCount);

    /* Generate 3 random ingredient numbers out of 10
       ChatGPT random style to change from rand */
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 10); // Assuming ingredient numbers are from 1 to 10

    for (int i = 0; i < 3; i++) {
        order.ingredients[i] = dist(gen);
    }

    return order;
}

void Restaurant::TakeOrder()
{
    {
        std::unique_lock<std::mutex> lk(displayMut);
        std::cout << "The Waiter is waiting for customers. \n";
    }

    while (true) 
    {
        std::unique_lock<std::mutex> lk(preparationMut);
        customerCond.wait(lk, [&] { return !preparationQueue.empty(); });

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

void Restaurant::PrepareIngredients()
{
    {
        std::unique_lock<std::mutex> lk(displayMut);
        std::cout << "The Cook is waiting for orders. \n";
    }

    while (true)
    {
        std::unique_lock<std::mutex> lk(orderMut);
        cookCond.wait(lk, [&] { return !ordersQueue.empty(); });

        /* Small delay to not instantly start the order when receiving it */
        std::this_thread::sleep_for(std::chrono::seconds(1));   // Sleep for 1 seconds

        {
            std::unique_lock<std::mutex> lk(displayMut);
            std::cout << "The cook is preparing the meal of customer " << ordersQueue.front().GetOrderNumber() << " \n";
        }

        std::this_thread::sleep_for(std::chrono::seconds(3));   // Sleep for 3 seconds

        {
            std::unique_lock<std::mutex> lk(displayMut);
            std::cout << "The cook finished preparing the meal of customer " << ordersQueue.front().GetOrderNumber() << " \n";
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

void Restaurant::PrepareMeal()
{
    {
        std::unique_lock<std::mutex> lk(displayMut);
        std::cout << "The Chef is waiting for orders. \n";
    }

    while (true)
    {
        std::unique_lock<std::mutex> lk(ingredientsMut);
        chefCond.wait(lk, [&] { return !ingredientsQueue.empty(); });

        /* Small delay to not instantly cook the meal when receiving the ingredients */
        std::this_thread::sleep_for(std::chrono::seconds(1));   // Sleep for 1 seconds

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
    }
}

void Restaurant::BringOrder()
{
}

void Restaurant::EatMeal()
{
}
