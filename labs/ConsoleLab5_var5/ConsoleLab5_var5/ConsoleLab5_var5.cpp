#include <Windows.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <barrier>
#include <vector>
#include <future>
#include <queue>

int calculateParticialSum(int* mas, int size, int* retSum, std::barrier<> &bar)
{
	int sum = 0;
	for (size_t i = 0; i < size; i++)
	{
		sum += mas[i];
	}
	retSum[0] += sum;
	bar.arrive_and_wait();
	return sum;
	
}

int masterMasSumFunc(int *mas, int size)
{
	int *sum = new int[1];
	sum[0] = 0;
	
	if (size > 4)
	{
		int numOfthreads = 5;
		std::barrier barrier(numOfthreads);
		
		std::vector<std::thread> threads;
		for (size_t i = 0; i < 4; i++)
		{
			int* startMas = &(mas[(size / 4)*i]);
			int particialSize = i < 3 ? size / 4 : size - 3*(size / 4);
			
			threads.emplace_back(calculateParticialSum, startMas, particialSize, sum, std::ref(barrier));
		}
		
		
		
		barrier.arrive_and_wait();
		for (std::thread &t : threads)
			t.join();
		
	}
	else
	{
		std::barrier barrier(1);
		sum[0] = calculateParticialSum(mas, size, sum, std::ref(barrier));
	}
	return sum[0];
}

void firstTask()
{
	int size = 100;
	int* mas = new int[size];

	for (size_t i = 0; i < size; i++)
		mas[i] = rand() % 100;
	for (size_t i = 0; i < size; i++)
		std::cout << mas[i] << " ";
	std::cout << std::endl;

	int justSum = 0;
	for (size_t i = 0; i < size; i++)
		justSum += mas[i];
	
	int parallelSum = masterMasSumFunc(mas, size);


	std::cout << "The parallel sum is " << parallelSum << " and regular sum is " << justSum << std::endl;
}

void secondTask()
{
	std::queue<std::function<void()>> q;
	int amountOfTasks = 30;
	int amountOfThreads = 4;

	for (size_t i = 0; i < amountOfTasks; i++)
	{
		q.push(firstTask);
	}

	std::vector<std::thread> threads;
	for (size_t i = 0; i < amountOfThreads; i++)
	{
		threads.emplace_back(q.front());
		q.pop();
	}

	HANDLE sem = CreateSemaphore(
		NULL,
		0,
		amountOfTasks,
		NULL
	);

	bool allQuerried = true;
	do
	{
		allQuerried = true;
		int i = 0;
		while (i < threads.size())
		{
			if (!threads.empty() && threads[i].joinable())
			{
				threads[i].join();
				ReleaseSemaphore(sem, 1, NULL);
				if (q.empty())
				{
					threads.erase(threads.begin() + i);
					i--;
				}
				else
				{
					threads[i].~thread();
					threads[i] = std::thread(q.front());
					q.pop();
					allQuerried = false;
				}
			}
			else
				allQuerried = false;
			i++;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(500));

	} while (!allQuerried);
	WaitForSingleObject(sem, INFINITE);
	
}


void absurdSumAAndB(int a, int& b, std::mutex &mutexForResourceB)
{
	mutexForResourceB.lock();
	b += 10;
	std::cout << "A is " << a << "\nB += 10\nB is " << b << "\nA+B is " << a + b << std::endl;
	mutexForResourceB.unlock();
};

void thirdTask()
{
	
	std::mutex mtxForResourceB;

	int b = 20;
	int amountOfThreads = 20;

	std::vector<std::thread> threads;
	for (size_t i = 0; i < amountOfThreads; i++)
	{
		threads.emplace_back(absurdSumAAndB, i, std::ref(b), std::ref(mtxForResourceB));
		
	}
	for (size_t i = 0; i < amountOfThreads; i++)
	{
		threads[i].join();
	}


	
}

int main()
{
	

	firstTask();
	//secondTask();
	thirdTask();

}

