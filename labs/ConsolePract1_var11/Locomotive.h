#include <string>
#pragma once
class Locomotive {
private:
    std::wstring model;
    int power;
    int maxSpeed;
    std::wstring fuelType;

public:
    Locomotive(const std::wstring& mdl, int pwr, int speed, const std::wstring& fuel);
    void startEngine();
    void accelerate(int speed);
    void stopEngine();
    void displayInfo();
};

