#include "Ball.h"
#include "GameManager.h" // avoid cicular dependencies

Ball::Ball(sf::RenderWindow* window, float velocity, GameManager* gameManager)
    : _window(window), _velocity(velocity), _gameManager(gameManager),
    _timeWithPowerupEffect(0.f), _isFireBall(false), _isAlive(true), _direction({1,1})
{
    _sprite.setRadius(RADIUS);
    _sprite.setFillColor(sf::Color::Cyan);
    _sprite.setPosition(0, 300);
}

Ball::~Ball()
{
}

void Ball::update(float dt)
{
    // check for powerup, tick down or correct
    if (_timeWithPowerupEffect > 0.f)
    {
        _timeWithPowerupEffect -= dt;
    }
    else
    {
        if (_velocity != VELOCITY)
            _velocity = VELOCITY;   // reset speed.
        else
        {
            setFireBall(0);    // disable fireball
            _sprite.setFillColor(_ballColour);  // back to normal colour.
        }        
    }

    // Fireball effect
    if (_isFireBall)
    {
        // Flickering effect
        int flicker = rand() % 50 + 205; // Random value between 205 and 255
        _sprite.setFillColor(sf::Color(flicker, flicker / 2, 0)); // Orange flickering color
    }

    // Update position with a subtle floating-point error
    _sprite.move(_direction * _velocity * dt);

    updateTrail(dt);

    // check bounds and bounce
    sf::Vector2f position = _sprite.getPosition();
    sf::Vector2u windowDimensions = _window->getSize();

    // bounce on walls
    if ((position.x >= windowDimensions.x - 2 * RADIUS && _direction.x > 0) || (position.x <= 0 && _direction.x < 0))
    {
        _gameManager->StartShake(0.5f, 15.f, 0.02f);
        _direction.x *= -1;
    }

    // bounce on ceiling
    if (position.y <= 0 && _direction.y < 0)
    {
        _gameManager->StartShake(0.5f, 15.f, 0.02f);
        _direction.y *= -1;
    }

    // lose life bounce
    if (position.y > windowDimensions.y)
    {
        _sprite.setPosition(0, 300);
        _direction = { 1, 1 };
        _gameManager->loseLife();
    }

    // collision with paddle
    if (_sprite.getGlobalBounds().intersects(_gameManager->getPaddle()->getBounds()))
    {
        changeColour();
        _direction.y *= -1; // Bounce vertically

        float paddlePositionProportion = (_sprite.getPosition().x - _gameManager->getPaddle()->getBounds().left) / _gameManager->getPaddle()->getBounds().width;
        _direction.x = paddlePositionProportion * 2.0f - 1.0f;

        // Adjust position to avoid getting stuck inside the paddle
        _sprite.setPosition(_sprite.getPosition().x, _gameManager->getPaddle()->getBounds().top - 2 * RADIUS);
    }

    // collision with bricks
    int collisionResponse = _gameManager->getBrickManager()->checkCollision(_sprite, _direction);
    if (_isFireBall) return; // no collisisons when in fireBall mode.
    if (collisionResponse == 1)
    {
        changeColour();
        _direction.x *= -1; // Bounce horizontally
    }
    else if (collisionResponse == 2)
    {
        changeColour();
        _direction.y *= -1; // Bounce vertically
    }
}

void Ball::render()
{
    // iterate over each point in trailPos
    for (size_t i = 0; i < trailPos.size(); i++) {
        // decrease the alphaFactor as i increases so that earlier points in the trail are more transparent
        float alphaFactor = 1.0f - static_cast<float>(i) / trailPos.size();
        sf::Uint8 alpha = static_cast<sf::Uint8>(alphaFactor * 255);

        sf::CircleShape trailDot(RADIUS * (0.75f + 0.25f * alphaFactor));
        trailDot.setPosition(trailPos[i]);
        // Fireball effect
        if (_isFireBall)
        {
            // Flickering effect
            int flicker = rand() % 50 + 205; // Random value between 205 and 255
            trailDot.setFillColor(sf::Color(flicker, flicker / 2, alpha)); // Orange flickering color
        }
        else {
            trailDot.setFillColor(sf::Color(_ballColour.r,_ballColour.g,_ballColour.b, alpha));
        }
        _window->draw(trailDot);
    }
    _window->draw(_sprite);
}

void Ball::setVelocity(float coeff, float duration)
{
    _velocity = coeff * VELOCITY;
    _timeWithPowerupEffect = duration;
}

void Ball::setFireBall(float duration)
{
    if (duration) 
    {
        _isFireBall = true;
        _timeWithPowerupEffect = duration;        
        return;
    }
    _isFireBall = false;
    _timeWithPowerupEffect = 0.f;    
}

void Ball::changeColour()
{
    switch (rand() % 5)
    {
    case 0:
        _ballColour = sf::Color(234.f, 242.f, 5.f);
        break;
    case 1:
        _ballColour = sf::Color(242.f, 5.f, 5.f);
        break;
    case 2:
        _ballColour = sf::Color(6.f, 3.f, 128.f);
        break;
    case 3:
        _ballColour = sf::Color(0.f, 255.f, 255.f);
        break;
    case 4:
        _ballColour = sf::Color(133.f, 0.f, 153.f);
        break;
    case 5:
        break;
    }
}

void Ball::updateTrail(float dt)
{
    // slow trails removal
    trailTimer += dt;
    if (trailTimer >= trailUpdateInterval) {
        // add current ball position to front of trailPos deque
        trailPos.push_front(_sprite.getPosition());

        // limit number of balls in the trail
        if (trailPos.size() > trailLength) {
            trailPos.pop_back();
        }
        trailTimer = 0.0f;
    }
}
