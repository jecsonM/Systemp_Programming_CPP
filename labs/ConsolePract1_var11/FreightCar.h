#include <string>
#pragma once
class FreightCar
{
private:
    std::wstring cargoType;
    double capacity;
    double currentLoad;
    bool isRefrigerated;

public:
    FreightCar(const std::wstring& cargo, double cap, double load, bool refrigerated);

    void loadCargo(double weight);
    void unloadCargo(double weight);
    void activateRefrigeration();
    void displayInfo();
};

