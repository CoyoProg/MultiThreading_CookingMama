#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

using namespace std;

int listLength = 1001;
int counter = 0;
mutex myMutex;

void DisplayEvenNumbers()
{
	cout << "Even: ";

	for (int i = 0; i <= 1000; i += 2)
	{
		cout << i << " ";
	}

	cout << endl;
}

void DisplayOddNumbers()
{
	cout << "Odd: ";

	for (int i = 1; i <= 1000; i += 2)
	{
		cout << i << " ";
	}

	cout << endl;
}

void DisplayEvenNumbersMutex(const vector<int> listP)
{
	for (counter; counter < listLength - 1;)
	{
		if (listP[counter] % 2 == 0)
		{
			lock_guard<mutex> lock(myMutex);
			cout << listP[counter] << " ";
			counter++;
		}
	}
}

void DisplayOddNumbersMutex(const vector<int> listP)
{
	for (counter; counter < listLength - 1;)
	{
		if (listP[counter] % 2 != 0)
		{
			lock_guard<mutex> lock(myMutex);
			cout << listP[counter] << " ";
			counter++;
		}
	}
}

void Play()
{
	int answer;
	cout << "1: Naive approche \n"
		<< "2: Ordering with Mutex \n";

	bool succeeded{ false };
	while (!succeeded)
	{
		cin >> answer;

		if (answer == 1 || answer == 2)
		{
			succeeded = true;
		}
	}

	system("cls");
	switch (answer)
	{
	case 1:
	{
		/** Not really great to display,
		everything is mixed up in the display,
		we dont know which one is odd or even */
		thread thread1(DisplayEvenNumbers);
		thread thread2(DisplayOddNumbers);

		thread1.join();
		thread2.join();
		break;
	}
	case 2:
	{
		/** We will use a list to make sens for mutex,
		we could use it without a list to achieve the same result,
		just adding: lock_guard<mutex> lock(myMutex);
		in DisplayEvenNumber & DisplayOddNumber */
		vector<int> list;
		list.resize(listLength);

		for (int i = 0; i < listLength; i++)
		{
			list[i] = i;
		}

		thread thread3(DisplayOddNumbersMutex, list);
		thread thread4(DisplayEvenNumbersMutex, list);

		thread3.join();
		thread4.join();

		break;
	}
	}

	cout << "\n";
	cout << "Press anykey to return to menu \n";
	system("pause");
}


int main()
{
	while (true)
	{
		system("cls");
		Play();
	}

}