#include "ParallelogramConsoleWorkerConsole.h"
#include <iostream>

ParallelogramConsoleWorkerConsole::ParallelogramConsoleWorkerConsole()
{
}

void ParallelogramConsoleWorkerConsole::StartWork()
{
	std::wcout << L"----------Working with Parallelograms----------\n";
	ParallelogramsAmount = CinParallelogramsAmount();
    Parallelograms = new Parallelogram[ParallelogramsAmount];


    for (size_t i = 0; i < ParallelogramsAmount; i++)
    {
        std::wcout << i << L": ";
        Parallelograms[i] = CinParallelogram();
    }


    bool toContinue = true;

    while (toContinue)
    {
        CoutParallelograms();
        CinAndCoutSelectedParallelogram();
        toContinue = CinChooseOperation();
    }


}

void ParallelogramConsoleWorkerConsole::EndWork()
{
}

void ParallelogramConsoleWorkerConsole::CinAndCoutSelectedParallelogram()
{
    int parallelogramSelectedIndex;

    bool isInputCorrect = false;
    while (!isInputCorrect) {
        std::wcout << L"Choose the parallelogram to work with (0 to  " << ParallelogramsAmount-1 << L"): ";
        std::wcin >> parallelogramSelectedIndex;

        if (std::wcin.fail() || parallelogramSelectedIndex < 0 || parallelogramSelectedIndex >= ParallelogramsAmount) {
            std::wcin.clear();
            std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), L'\n');
            std::wcout << L"Error! Please, input positive integer (0 to  " << ParallelogramsAmount - 1 << L"): ";
        }
        else {
            isInputCorrect = true;
        }
    }
    SelectedIndex = parallelogramSelectedIndex;
    
    CoutParallelogram(Parallelograms[SelectedIndex]);
    

}

int ParallelogramConsoleWorkerConsole::CinParallelogramsAmount()
{
	int parallelogramAmount;
	
    bool isInputCorrect = false;
    while (!isInputCorrect) {
        std::wcout << L"Input the amount of parallelograms to work with (positive): ";
        std::wcin >> parallelogramAmount;

        if (std::wcin.fail() || parallelogramAmount <= 0) {
            std::wcin.clear(); 
            std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), L'\n');
            std::wcout << L"Error! Please, input positive integer.\n";
        }
        else {
            isInputCorrect = true;
        }
    }

	return parallelogramAmount;
}

Parallelogram ParallelogramConsoleWorkerConsole::CinParallelogram()
{
    double sideA, sideB, angle;
    bool isInputCorrect = false;

    while (!isInputCorrect)
    {
        std::wcout << L"Input sides A, B and angle in degrees (space between): ";
        std::wcin >> sideA >> sideB >> angle;

        if (std::wcin.fail()) {
            std::wcin.clear();
            std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::wcout << L"Error! Input 3 numbers with space between.\n";
        }
        else {
            if (sideA <= 0 || sideB <= 0) {
                std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::wcout << L"Error! Sides must be > 0\n";
            }
            else {
                std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                isInputCorrect = true;
            }
        }
    }
    return Parallelogram(sideA, sideB, angle);
}

bool ParallelogramConsoleWorkerConsole::CinChooseOperation()
{

    int selectedOperationIndex;

    bool isInputCorrect = false;
    while (!isInputCorrect) {
        std::wcout << L"Actions:" << std::endl
            << L"-1: Quit" << std::endl
            << L"0: Cancel" << std::endl
            << L"1: Change sides by percent" << std::endl
            << L"2: See area" << std::endl
            << L"3: See perimeter" << std::endl
            << L"4: See diagonals" << std::endl
            << L"5: See heights" << std::endl
            << L"Choose action: ";
        std::wcin >> selectedOperationIndex;

        if (std::wcin.fail() || selectedOperationIndex < -1 || selectedOperationIndex > 5) {
            std::wcin.clear();
            std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), L'\n');
            std::wcout << L"Error! Please, input positive integer (0 to  " << ParallelogramsAmount - 1 << L"): ";
        }
        else {
            isInputCorrect = true;
        }
    }

    switch (selectedOperationIndex)
    {
        case -1:
            return false;
        case 0:
            break;
        case 1:
            CinAndCoutParallelogramSides();
            break;
        case 2:
            CoutParallelogramArea();
            break;
        case 3:
            CoutParallelogramPerimeter();
            break;
        case 4:
            CoutParallelogramDiagonals();
            break;
        case 5:
            CoutParallelogramHeights();
            break;
    default:
        std::wcout << L"ERROR! UNKOWN operation" << std::endl;
        break;
    }

	return true;
}


void ParallelogramConsoleWorkerConsole::CoutParallelograms()
{
    std::wcout << L"---------- inputed parallelograms ----------\n\n";
    for (size_t i = 0; i < ParallelogramsAmount; i++)
    {
        std::wcout << L"[" << i << L"]:";
        CoutParallelogram(Parallelograms[i]);
    }

    std::wcout << L"-----------------------------------------\n\n";
}
void ParallelogramConsoleWorkerConsole::CoutParallelogram(Parallelogram parallelogramToCout)
{
    std::wcout 
        << " sideA " << parallelogramToCout.getSideA()
        << " sideB " << parallelogramToCout.getSideB()
        << " angle " << parallelogramToCout.getAngle() << std::endl;
}

void ParallelogramConsoleWorkerConsole::CinAndCoutParallelogramSides()
{

    double sideChangePercent;
    int choosenSides;
    bool isInputCorrect = false;

    while (!isInputCorrect)
    {
        std::wcout << L"\nSides to affect:\n0: None\n1: sideA\n2: sideB\n3: bothSides\n";
        std::wcout << L"Input sides change percent (>-100) and which side it will affect (0 to 3): ";
        std::wcin >> sideChangePercent >> choosenSides;

        if (std::wcin.fail()) {
            std::wcin.clear();
            std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::wcout << L"Error! Input 3 numbers with space between.\n";
        }
        else {
            if (sideChangePercent < -100 || choosenSides < 0 || choosenSides > 3) {
                std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::wcout << L"Error! Choosen sides must be from 0 to 3 and side change percent > -100\n";
            }
            else {
                std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                isInputCorrect = true;
            }
        }
    }

    Parallelogram choosenPar = Parallelograms[SelectedIndex];
    choosenPar.changeSideLengths(sideChangePercent, choosenSides);
    
    std::wcout << L"Parrallelogram sides: A:" << choosenPar.getSideA() << L" B: " << choosenPar.getSideB() << L"\n\n";
}

void ParallelogramConsoleWorkerConsole::CoutParallelogramArea()
{
    std::wcout << L"Parrallelogram area: " << Parallelograms[SelectedIndex].getArea() << L"\n\n";
}

void ParallelogramConsoleWorkerConsole::CoutParallelogramPerimeter()
{
    std::wcout << L"Parrallelogram perimeter: " << Parallelograms[SelectedIndex].getPerimeter() << L"\n\n";
}

void ParallelogramConsoleWorkerConsole::CoutParallelogramDiagonals()
{
    double* diagonals = Parallelograms[SelectedIndex].getDiagonals();
    std::wcout << L"Parrallelogram diagonals: [0]:" << diagonals[0] << L" [1]: " << diagonals[1] << L"\n\n";
}

void ParallelogramConsoleWorkerConsole::CoutParallelogramHeights()
{
    double* heights = Parallelograms[SelectedIndex].getHeights();
    std::wcout << L"Parrallelogram heights: [0]:" << heights[0] << L" [1]: " << heights[1] << L"\n\n";
}
