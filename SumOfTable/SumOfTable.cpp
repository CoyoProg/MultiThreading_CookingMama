// SumOfTable.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <Windows.h>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

int numberOfThreads = 3;
int randomVectorSize{ 0 };
int sumGlobal{0};

mutex myMutex;

void InitializeList(std::vector<int>& table)
{
    std::cout << "Table: ";

    int randomVectorSize = (rand() % 10) + 3;
    //int randomVectorSize = 50000;
    table.resize(randomVectorSize);

    for (int i = 0; i < randomVectorSize; i++)
    {
        table[i] = rand() % 10;
        std::cout << table[i] << " ";
    }

    std::cout << "\n\n";
}

int CalculateSequentialSum(const std::vector<int>& table)
{
    int sum = 0;
    for (auto value : table)
    {
        sum += value;
    }

    return sum;
}

void CalculateSubTableSum(const std::vector<int>& table, int& sum, int startP, int endP)
{
    for (int i = startP; i < endP; i++)
    {
        sum += table[i];
    }
}

void CalculateGlobalSum(const std::vector<int>& table, int startP, int endP)
{
    // Lock the mutex and update the global sum
    lock_guard<mutex> lock(myMutex);

    int localSum = 0;
    for (int i = startP; i < endP; i++)
    {
        localSum += table[i];
    }

    sumGlobal += localSum;
}

int MultiThreading(std::vector<int>& table)
{
    // Create an array of threads
    vector<thread> threads;
    vector<int> subSum(numberOfThreads, 0);

    int subValue = table.size() / numberOfThreads;
    int rest = table.size() % numberOfThreads;

    for (int i = 0; i < numberOfThreads; i++) 
    {
        int start = i * subValue;
        int end = start + subValue;

        if (i == numberOfThreads - 1) 
        {
            end += rest; // Add the remainder to the last thread
        }

        threads.emplace_back(CalculateSubTableSum, table, std::ref(subSum[i]), start, end);
    }

    // Join all the threads
    for (int i = 0; i < threads.size(); i++) 
    {
        threads[i].join();
    }

    int finalSum = CalculateSequentialSum(subSum);
    return finalSum;
}

void Mutex(std::vector<int>& table)
{
    // Create an array of threads
    vector<thread> threads;

    int subValue = table.size() / numberOfThreads;
    int rest = table.size() % numberOfThreads;

    for (int i = 0; i < numberOfThreads; i++)
    {
        int start = i * subValue;
        int end = start + subValue;

        if (i == numberOfThreads - 1)
        {
            end += rest; // Add the remainder to the last thread
        }

        threads.emplace_back(CalculateGlobalSum, table, start, end);
    }

    // Join all the threads
    for (int i = 0; i < threads.size(); i++)
    {
        threads[i].join();
    }
}


int main()
{

    /** Initialization */
    srand(time(NULL));
    std::vector<int> table;
    InitializeList(table);

    /** Sequential */
    int sum = CalculateSequentialSum(table);
    std::cout << "\033[33mSequential\033[0m total sum of the table: " << sum << "\n";

    /**  MultiThread */
    sum = MultiThreading(table);
    std::cout << "Multi-Thread total sum of the \033[94msubTables\033[0m: " << sum << "\n";

    /** Mutex */
    Mutex(table);
    std::cout << "Multi-Thread total sum of the table (\033[91mMutex\033[0m): " << sumGlobal << "\n";
}
