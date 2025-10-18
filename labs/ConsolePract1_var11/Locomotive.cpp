#include "Locomotive.h"
#include <iostream>
#include <string>

Locomotive::Locomotive(const std::wstring& mdl, int pwr, int speed, const std::wstring& fuel)
	: model(mdl), power(pwr), maxSpeed(speed), fuelType(fuel) {};


void Locomotive::startEngine() {
    std::wcout << L"Locomotive " << model << " engine started\n";
}

void Locomotive::accelerate(int speed) {
    std::wcout << L"Locomotive " << model << " accelerating to " << speed << " km/h\n";
}

void Locomotive::stopEngine() {
    std::wcout << L"Locomotive " << model << L" engine stopped\n";
}

void Locomotive::displayInfo() {
    std::wcout << L"Model: " << model << L", Power: " << power << L" kW, Max Speed: "
        << maxSpeed << L" km/h, Fuel: " << fuelType << "\n";
}