#include <string>
#pragma once
class RailwayCrane
{
private:
    std::wstring model;
    double liftingCapacity;
    int boomLength;
    bool isMobile;

public:
    RailwayCrane(const std::wstring& mdl, double liftCap, int boom, bool mobile);
    void liftLoad(double weight);
    void moveBoom(int angle);
    void moveToPosition(const std::wstring& position);
    void displayInfo();
};

