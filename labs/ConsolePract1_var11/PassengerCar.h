#include <string>;
#pragma once
class PassengerCar
{
private:
    std::wstring carType;
    int capacity;
    int comfortLevel;
    bool hasAC;

public:
    PassengerCar(const std::wstring& type, int cap, int comfort, bool ac);
    void openDoors();
    void closeDoors();
    void adjustTemperature(int temp);
    void displayInfo();
};

