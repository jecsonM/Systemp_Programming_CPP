#pragma once
class Parallelogram
{
protected:
	double SideA;
	double SideB;
	double Angle;
public:
	const static int PARALLELOGRAM_FLAG_SIDE_A = 1;
	const static int PARALLELOGRAM_FLAG_SIDE_B = 2;

	Parallelogram();
	Parallelogram(double sideA, double sideB, double angle);
	
	bool setSideA(double sideA);
	double getSideA();
	bool setSideB(double sideB);
	double getSideB();
	bool setAngle(double angle);
	double getAngle();

	void changeSideLengths(double percent, int sidesFlags = 3);
	double getPerimeter();
	double getArea();
	double* getDiagonals();
	double* getHeights();


};

