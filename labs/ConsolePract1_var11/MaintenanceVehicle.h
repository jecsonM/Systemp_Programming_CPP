#include <string>
#pragma once
class MaintenanceVehicle
{
private:
    std::wstring vehicleType;
    std::wstring purpose;
    int workSpeed;
    bool hasSpecialEquipment;

public:
    MaintenanceVehicle(const std::wstring& type, const std::wstring& purp, int speed, bool equipment);
    void startWork();
    void performMaintenance();
    void activateEquipment();
    void displayInfo();
};

