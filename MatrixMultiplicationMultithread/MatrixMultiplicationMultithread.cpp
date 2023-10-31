#include <iostream>
#include <vector>
#include <thread>
#include "MatrixMultiplicationMultithread.h"

using namespace std;

int matrixSize = 3;

class MatrixMultiplactorTask
{
public:
	/** Constructor */
	MatrixMultiplactorTask(const vector<vector<int>>& matrixA, const vector<vector<int>>& matrixB, vector<vector<int>>& matrixResults, int rowP, int colP)
		: A(matrixA), B(matrixB), P(matrixResults), row(rowP), col(colP) {}

	/** operator ()() */
	void operator()() const
	{
		int sum = 0;
		for (int k = 0; k < matrixSize; k++) 
		{
			sum += A[row][k] * B[k][col];
		}
		P[row][col] = sum;
	}

private:
	/** Task Properties */
	const vector<vector<int>>& A;
	const vector<vector<int>>& B;
	vector<vector<int>>& P;
	int row, col;
};

void MaxtrixSingleElementMultiplication(const vector<vector<int>>& matrixA, const vector<vector<int>>& matrixB, vector<vector<int>>& resultMatrix, int row, int col) {
	int sum = 0;
	for (int k = 0; k < matrixSize; k++) 
	{
		sum += matrixA[row][k] * matrixB[k][col];
	}

	resultMatrix[row][col] = sum;
}

void MatrixSequencialMultiplication(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& P)
{
	for (int i = 0; i < matrixSize; i++)
	{
		for (int j = 0; j < matrixSize; j++)
		{
			P[i][j] = 0;
			for (int k = 0; k < matrixSize; k++)
			{
				P[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

void TaskMultiThreading(std::vector<std::vector<int>>& A, std::vector<std::vector<int>>& B, std::vector<std::vector<int>>& Result)
{
	// Create an array of threads
	vector<thread> threads;

	// Start computing P_ij using threads
	for (int i = 0; i < matrixSize; i++)
	{
		for (int j = 0; j < matrixSize; j++)
		{
			threads.emplace_back(MatrixMultiplactorTask(A, B, Result, i, j));
		}
	}

	// Join all the threads
	for (int i = 0; i < matrixSize * matrixSize; i++)
	{
		threads[i].join();
	}
}

void MultiThreadings(const vector<vector<int>>& matrixA, const vector<vector<int>>& matrixB, vector<vector<int>>& resultMatrix)
{
	// Create an array of threads
	vector<thread> threads;

	// Start computing P_ij using threads
	for (int i = 0; i < matrixSize; i++) 
	{
		for (int j = 0; j < matrixSize; j++) 
		{
			threads.emplace_back(MaxtrixSingleElementMultiplication, ref(matrixA), ref(matrixB), ref(resultMatrix), i, j);
		}
	}

	// Join all the threads
	for (int i = 0; i < threads.size(); i++) 
	{
		threads[i].join();
	}
}

void DisplayMatrix(std::vector<std::vector<int>>& Result)
{
	for (int i = 0; i < matrixSize; i++)
	{
		for (int j = 0; j < matrixSize; j++)
		{
			cout << Result[i][j] << " ";
		}
		cout << endl;
	}
}

int main() 
{
	// Define and initialize matrices A and B as 3x3 matrices
	vector<vector<int>> A = 
	{	{9, 3, 45}, 
		{4, 7, 90}, 
		{2, 8, 73} 
	};
	vector<vector<int>> B = 
	{	{1, 49, 1}, 
		{3, 73, 8}, 
		{5, 22, 7} 
	};

	vector<vector<int>> Result(3, vector<int>(3));

	/** Perform matrix sequential multiplication */
	MatrixSequencialMultiplication(A, B, Result);
	cout << "Matrix \033[33mSequencial\033[0m Result" << endl;
	DisplayMatrix(Result);

	std::cout << "\n";

	/** Perform matrix task multithread multiplication */
	cout << "Matrix \033[91mtask multi-thread\033[0m Result" << endl;
	TaskMultiThreading(A, B, Result);
	DisplayMatrix(Result);

	std::cout << "\n";

	/** Perform matrix multithread multiplication */
	cout << "Matrix \033[94mmulti-thread\033[0m Result" << endl;
	MultiThreadings(A, B, Result);
	DisplayMatrix(Result);

	return 0;
}
