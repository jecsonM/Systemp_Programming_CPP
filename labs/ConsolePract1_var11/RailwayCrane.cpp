#include "RailwayCrane.h"
#include <iostream>
#include <string>

RailwayCrane::RailwayCrane(const std::wstring& mdl, double liftCap, int boom, bool mobile)
	: model(mdl), liftingCapacity(liftCap), boomLength(boom), isMobile(mobile) {};

void RailwayCrane::liftLoad(double weight) {
    if (weight <= liftingCapacity) {
        std::wcout << L"Crane " << model << L" lifting " << weight << L" tons\n";
    }
}

void RailwayCrane::moveBoom(int angle) {
    std::wcout << L"Crane " << model << L" moving boom to " << angle << L" degrees\n";
}

void RailwayCrane::moveToPosition(const std::wstring& position) {
    if (isMobile) {
        std::wcout << L"Crane " << model << L" moving to position " << position << std::endl;
    }
}

void RailwayCrane::displayInfo() {
    std::wcout << L"Model: " << model << L", Lifting Capacity: " << liftingCapacity
        << L" tons, Boom Length: " << boomLength << L" m, Mobile: "
        << (isMobile ? L"Yes" : L"No") << std::endl;
}