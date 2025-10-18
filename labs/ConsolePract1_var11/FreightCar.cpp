#include "FreightCar.h"
#include <iostream>
#include <string>

FreightCar::FreightCar(const std::wstring& cargo, double cap, double load, bool refrigerated) 
	: cargoType(cargo), capacity(cap), currentLoad(load), isRefrigerated(refrigerated) {}

void FreightCar::loadCargo(double weight)
{
	if (currentLoad + weight <= capacity) {
		currentLoad += weight;
		std::wcout << "Loaded " << weight << L" tons of " << cargoType << std::endl;
	}
}

void FreightCar::unloadCargo(double weight)
{
	if (currentLoad - weight >= 0) {
		currentLoad -= weight;
		std::wcout << L"Unloaded " << weight << L" tons of " << cargoType << std::endl;
	}
}

void FreightCar::activateRefrigeration() {
	if (isRefrigerated) {
		std::wcout << L"Refrigeration activated for " << cargoType << L" car\n";
	}
}

void FreightCar::displayInfo() {
	std::wcout << L"Cargo Type: " << cargoType << L", Capacity: " << capacity
		<< L" tons, Current Load: " << currentLoad << L" tons, Refrigerated: "
		<< (isRefrigerated ? L"Yes" : L"No") << L"\n";
}