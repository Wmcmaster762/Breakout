#include "PowerupSticky.h"

PowerupSticky::PowerupSticky(sf::RenderWindow* window, Paddle* paddle, Ball* ball)
	: PowerupBase(window, paddle, ball)
{
	_sprite.setFillColor(stickyBallEffectsColour);
}

PowerupSticky::~PowerupSticky()
{
}

std::pair<POWERUPS, float> PowerupSticky::applyEffect()
{
	_ball->setSticky(true,10.0f);
	return { stickyBall, 10.0f };
}
