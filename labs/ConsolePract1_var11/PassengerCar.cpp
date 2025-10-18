#include "PassengerCar.h"
#include <iostream>
#include <string>

PassengerCar::PassengerCar(const std::wstring& type, int cap, int comfort, bool ac) 
	: carType(type), capacity(cap), comfortLevel(comfort), hasAC(ac) {};

void PassengerCar::openDoors() {
    std::wcout << L"Passenger car " << carType << L" doors opened\n";
}

void PassengerCar::closeDoors() {
    std::wcout << L"Passenger car " << carType << L" doors closed\n";
}

void PassengerCar::adjustTemperature(int temp) {
    if (hasAC) {
        std::wcout << L"Passenger car " << carType << L" temperature set to " << temp << std::endl;
    }
}

void PassengerCar::displayInfo() {
    std::wcout << L"Type: " << carType << L", Capacity: " << capacity
        << L", Comfort Level: " << comfortLevel << L", AC: " << (hasAC ? L"Yes" : L"No") << std::endl;
}