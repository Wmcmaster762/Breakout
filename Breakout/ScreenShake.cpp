#include "ScreenShake.h"

ScreenShake::ScreenShake() 
	: rng(std::random_device{}()),dist(-1.f,1.f)
{

}

ScreenShake::~ScreenShake()
{
}

void ScreenShake::StartShake(float duration, float intensity, float fade, const sf::View& view)
{
	shakeDuration = duration;
	shakeIntensity = intensity;
	shakeFade = fade;
	isShaking = true;
	// save the center of the view
	originalCenter = view.getCenter();
}

void ScreenShake::Update(float dt, sf::View& view)
{
	if (isShaking) {
		if (shakeDuration > 0) {
			// decrease shake duration
			shakeDuration -= dt;

			// set screens view to original center plus the offset
			offset = GetRandomOffset() * shakeIntensity;
			view.setCenter(originalCenter + offset);

            // apply fading
            shakeIntensity *= (1.0f - shakeFade);
        }
        else {
            isShaking = false;
			view.setCenter(originalCenter); // reset view to original center
		}
	}
}

// return two random numbers between -1 and 1
sf::Vector2f ScreenShake::GetRandomOffset()
{
	return sf::Vector2f(dist(rng),dist(rng));
}


