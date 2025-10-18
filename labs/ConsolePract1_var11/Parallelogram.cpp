#include <stdexcept>
#include <cmath>
#include "Parallelogram.h"
#define M_PI 3.14159265358979323846


Parallelogram::Parallelogram()
{
}


Parallelogram::Parallelogram(double sideA, double sideB, double angle)
{
	setSideA(sideA);
	setSideB(sideB);
	setAngle(angle);
}

bool Parallelogram::setSideA(double sideA)
{
	if (sideA > 0)
		SideA = sideA;
	else
		return false;
	//throw std::invalid_argument("Side length must be positive");
	return true;
};
double Parallelogram::getSideA() { return SideB; };


bool Parallelogram::setSideB(double sideB)
{
	if (sideB > 0)
		SideB = sideB;
	else
		return false;
		//throw std::invalid_argument("Side length must be positive");
	return true;
};
double Parallelogram::getSideB() { return SideB; };


bool Parallelogram::setAngle(double angle)
{
	if (angle < 0)
	{
		angle = std::abs(angle);
	}
	angle = std::fmod(angle, 180.0);
	if (angle > 90.0)
	{
		angle = 180.0 - angle;
	}
	Angle = angle;
	return true;
};
double Parallelogram::getAngle() { return Angle; };


void Parallelogram::changeSideLengths(double percent, int sidesFlags)
{
	if (sidesFlags & PARALLELOGRAM_FLAG_SIDE_A == PARALLELOGRAM_FLAG_SIDE_A)
		SideA *= 1 + percent / 100.0;

	if (sidesFlags & PARALLELOGRAM_FLAG_SIDE_B == PARALLELOGRAM_FLAG_SIDE_B)
		SideB *= 1 + percent / 100.0;
};

double Parallelogram::getPerimeter() { return (getSideA() + getSideB()) * 2; };
double Parallelogram::getArea() { 
	return std::sin(getAngle() * M_PI / 180) * getSideA() * getSideB(); 
};

double* Parallelogram::getDiagonals() 
{
	double* diagonals = new double[2];

	double a = getSideA(), 
		b = getSideB(), 
		ang = getAngle() * M_PI / 180;
	

	diagonals[0] = std::sqrt( a*a + b*b + 2*a*b*std::cos(ang));
	diagonals[1] = std::sqrt(a * a + b * b - 2 * a * b * std::cos(ang));

	return diagonals;
};
double* Parallelogram::getHeights()
{
	double* heights = new double[2];

	double 
		a = getSideA(),
		b = getSideB(),
		area = getArea();

	heights[0] = area / a;
	heights[1] = area / b;
	return heights;
};