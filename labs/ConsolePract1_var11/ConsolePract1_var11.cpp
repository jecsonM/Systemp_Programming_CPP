#include "Parallelogram.h"
#include "ParallelogramConsoleWorkerConsole.h"
#include <string>

#include "FreightCar.h"
#include "Locomotive.h"
#include "MaintenanceVehicle.h"
#include "PassengerCar.h"
#include "RailwayCrane.h"

#include <vector>
#include <iostream>

int main()
{
    
    ParallelogramConsoleWorkerConsole consoleWorker = ParallelogramConsoleWorkerConsole();
    consoleWorker.StartWork();
    
    //Второе задание

    std::vector<Locomotive> locomotives = {
        Locomotive(L"VL80", 6000, 110, L"Electric"),
        Locomotive(L"2TE116", 4500, 100, L"Diesel"),
        Locomotive(L"ChS4", 5200, 120, L"Electric"),
        Locomotive(L"TEP70", 4000, 160, L"Diesel"),
        Locomotive(L"EP1", 5800, 140, L"Electric"),
        Locomotive(L"2ES6", 8800, 120, L"Electric"),
        Locomotive(L"TEM2", 1200, 95, L"Diesel")
    };

    std::vector<PassengerCar> passengerCars = {
        PassengerCar(L"Compartment", 36, 4, true),
        PassengerCar(L"Platzkart", 54, 2, true),
        PassengerCar(L"Luxury", 18, 5, true),
        PassengerCar(L"Sitting", 80, 1, false),
        PassengerCar(L"Restaurant", 48, 3, true),
        PassengerCar(L"Sleeping", 32, 4, true),
        PassengerCar(L"Business", 24, 5, true)
    };

    std::vector<FreightCar> freightCars = {
        FreightCar(L"Coal", 70, 0, false),
        FreightCar(L"Grain", 60, 0, false),
        FreightCar(L"Refrigerated", 50, 0, true),
        FreightCar(L"Timber", 65, 0, false),
        FreightCar(L"Tank", 75, 0, false),
        FreightCar(L"Container", 68, 0, false),
        FreightCar(L"Automobile", 55, 0, false)
    };

    std::vector<RailwayCrane> cranes = {
        RailwayCrane(L"KDE-251", 25, 15, true),
        RailwayCrane(L"EK-1", 100, 25, false),
        RailwayCrane(L"KZh-50", 50, 20, true),
        RailwayCrane(L"EDK-200", 200, 30, false),
        RailwayCrane(L"KK-20", 20, 12, true),
        RailwayCrane(L"K-100", 100, 22, true),
        RailwayCrane(L"EDK-500", 500, 40, false)
    };

    std::vector<MaintenanceVehicle> maintenanceVehicles = {
        MaintenanceVehicle(L"Track layer", L"Rail laying", 5, true),
        MaintenanceVehicle(L"Snow plow", L"Snow removal", 20, true),
        MaintenanceVehicle(L"Rail grinder", L"Rail polishing", 10, true),
        MaintenanceVehicle(L"Ballast cleaner", L"Track maintenance", 8, true),
        MaintenanceVehicle(L"Inspection car", L"Track inspection", 40, false),
        MaintenanceVehicle(L"Weed sprayer", L"Vegetation control", 15, true),
        MaintenanceVehicle(L"Ditch cleaner", L"Drainage maintenance", 12, true)
    };

    std::wcout << L"=== Railway Equipment Demo ===\n\n";

    std::wcout << L"LOCOMOTIVES:\n";
    for (auto& loco : locomotives) {
        loco.displayInfo();
        loco.startEngine();
        loco.accelerate(80);
        loco.stopEngine();
        std::wcout << L"---\n";
    }

    std::wcout << L"\nPASSENGER CARS:\n";
    for (auto& car : passengerCars) {
        car.displayInfo();
        car.openDoors();
        car.closeDoors();
        car.adjustTemperature(22);
        std::wcout << L"---\n";
    }

    std::wcout << L"\nFREIGHT CARS:\n";
    for (auto& car : freightCars) {
        car.displayInfo();
        car.loadCargo(30);
        car.unloadCargo(15);
        car.activateRefrigeration();
        std::wcout << L"---\n";
    }

    std::wcout << L"\nRAILWAY CRANES:\n";
    for (auto& crane : cranes) {
        crane.displayInfo();
        crane.liftLoad(10);
        crane.moveBoom(45);
        crane.moveToPosition(L"Station A");
        std::wcout << L"---\n";
    }

    std::wcout << L"\nMAINTENANCE VEHICLES:\n";
    for (auto& vehicle : maintenanceVehicles) {
        vehicle.displayInfo();
        vehicle.startWork();
        vehicle.performMaintenance();
        vehicle.activateEquipment();
        std::wcout << L"---\n";
    }

    return 0;


}
