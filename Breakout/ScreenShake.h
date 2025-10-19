#pragma once
#include <SFML/Graphics.hpp>
#include "CONSTANTS.h"
#include <vector>
#include <random>

class ScreenShake
{
public:
	ScreenShake();
	~ScreenShake();

	void StartShake(float duration, float intensity, float fade, const sf::View& view);
	void Update(float dt, sf::View& view);

private:
	bool isShaking;
	float shakeDuration;
	float shakeIntensity;
	float shakeFade;
	sf::Vector2f originalCenter;
	sf::Vector2f offset;

	std::default_random_engine rng;
	std::uniform_real_distribution<float> dist;

	sf::Vector2f GetRandomOffset();
};

