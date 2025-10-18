#include "MaintenanceVehicle.h"
#include <iostream>
#include <string>

MaintenanceVehicle::MaintenanceVehicle(const std::wstring& type, const std::wstring& purp, int speed, bool equipment)
	: vehicleType(type), purpose(purp), workSpeed(speed), hasSpecialEquipment(equipment) {};


void MaintenanceVehicle::startWork() {
    std::wcout << L"Maintenance vehicle " << vehicleType << L" starting work\n";
}

void MaintenanceVehicle::performMaintenance() {
    std::wcout << L"Performing maintenance: " << purpose << std::endl;
}

void MaintenanceVehicle::activateEquipment() {
    if (hasSpecialEquipment) {
        std::wcout << L"Special equipment activated on " << vehicleType << std::endl;
    }
}

void MaintenanceVehicle::displayInfo() {
    std::wcout << L"Type: " << vehicleType << L", Purpose: " << purpose
        << L", Work Speed: " << workSpeed << L" km/h, Special Equipment: "
        << (hasSpecialEquipment ? L"Yes" : L"No") << std::endl;
}