#pragma once

class Orders
{
public:
    Orders(int orderNumberP)
    {
        for (int i = 0; i < 3; i++)
        {
            ingredients[i] = orderNumberP; // Initialize with the order number for demonstration
        }

        OrderNumber = orderNumberP;
    }

    int ingredients[3];
    int GetOrderNumber() { return OrderNumber; }

private:
    int OrderNumber{ 0 };
};