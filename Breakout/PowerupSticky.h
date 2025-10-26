#pragma once
#include "PowerupBase.h"
class PowerupSticky : public PowerupBase
{
public:
    PowerupSticky(sf::RenderWindow* window, Paddle* paddle, Ball* ball);
    ~PowerupSticky();

    std::pair<POWERUPS, float> applyEffect() override; // Method to apply the power-up effect

};


