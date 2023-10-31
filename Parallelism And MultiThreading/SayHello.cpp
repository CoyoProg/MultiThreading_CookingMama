// Parallelism And MultiThreading.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <thread>


using std::cout;
using std::endl;

class MatrixMultiplactorTask
{
public:
	void operator()() const
	{
		sayHello();
	}

private:
	void sayHello() const
	{
		cout << "Hello world from operator\n";
	}
};

void sayHelloFunction() 
{ 
	cout << "Hello world from function" << endl; 
}

int main()
{
	// Thread with a class that implements () operator (uses rvalue)
	std::thread t0{ MatrixMultiplactorTask() };

	std::cout << std::endl;

	// Thread with lambda
	std::thread t1([]
		{
			sayHelloFunction();
		});

	// The order is not deterministic
	t0.join();
	t1.join();
	return 0;
}