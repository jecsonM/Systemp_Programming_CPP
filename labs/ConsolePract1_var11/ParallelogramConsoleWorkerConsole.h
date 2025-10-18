#pragma once
#include "Parallelogram.h"
class ParallelogramConsoleWorkerConsole
{
protected:
	Parallelogram* Parallelograms;
	int SelectedIndex;
	int ParallelogramsAmount;
public:

	ParallelogramConsoleWorkerConsole();

	void StartWork();
	void EndWork();


	int CinParallelogramsAmount();
	void CinAndCoutSelectedParallelogram();
	Parallelogram CinParallelogram();
	bool CinChooseOperation();

	void CoutParallelograms();
	void CoutParallelogram(Parallelogram parallelogramToCout);
	void CinAndCoutParallelogramSides();
	void CoutParallelogramArea();
	void CoutParallelogramPerimeter();
	void CoutParallelogramDiagonals();
	void CoutParallelogramHeights();

};

