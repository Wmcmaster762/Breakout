#include "GameManager.h"
#include "Ball.h"
#include "PowerupManager.h"
#include <iostream>

GameManager::GameManager(sf::RenderWindow* window)
    : _window(window), _paddle(nullptr), _ball(nullptr), _brickManager(nullptr), _powerupManager(nullptr),
    _messagingSystem(nullptr), _ui(nullptr), _pause(false), _time(0.f), _lives(3), _pauseHold(0.f), _levelComplete(false),
    _powerupInEffect({ none,0.f }), _timeLastPowerupSpawned(0.f)
{
    _font.loadFromFile("font/montS.ttf");
    _masterText.setFont(_font);
    _masterText.setPosition(50, 400);
    _masterText.setCharacterSize(48);
    _masterText.setFillColor(sf::Color::Yellow);
}

void GameManager::initialize()
{
    _paddle = new Paddle(_window);
    _brickManager = new BrickManager(_window, this);
    _messagingSystem = new MessagingSystem(_window);
    _ball = new Ball(_window, 400.0f, this); 
    _powerupManager = new PowerupManager(_window, _paddle, _ball);
    _ui = new UI(_window, _lives, this);

    // Create bricks
    _brickManager->createBricks(5, 10, 80.0f, 30.0f, 5.0f);
}

void GameManager::update(float dt)
{
    UpdateScreenShake(dt);
    _powerupInEffect = _powerupManager->getPowerupInEffect();
    _ui->updatePowerupText(_powerupInEffect);
    _powerupInEffect.second -= dt;
    
    //restart game on press R
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
    {
        RestartGame();
    }
    if (_lives <= 0)
    {
        _masterText.setString("Game over. Press R to restart");
        return;
    }
    if (_levelComplete)
    {
        _masterText.setString("Level completed. Press R to restart");
        return;
    }
    // pause and pause handling
    if (_pauseHold > 0.f) _pauseHold -= dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
    {
        if (!_pause && _pauseHold <= 0.f)
        {
            _window->setMouseCursorVisible(true);
            _window->setMouseCursorGrabbed(false);
            _pause = true;
            _masterText.setString("paused.");
            _pauseHold = PAUSE_TIME_BUFFER;
        }
        if (_pause && _pauseHold <= 0.f)
        {
            _window->setMouseCursorVisible(false);
            _window->setMouseCursorGrabbed(true);
            _pause = false;
            _masterText.setString("");
            _pauseHold = PAUSE_TIME_BUFFER;
        }
    }
    if (_pause)
    {
        return;
    }

    //quick close for convenience
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        _window->close();
    }

    // timer.
    _time += dt;


    if (_time > _timeLastPowerupSpawned + POWERUP_FREQUENCY && rand()%700 == 0)      // TODO parameterise
    {
        _powerupManager->spawnPowerup();
        _timeLastPowerupSpawned = _time;
    }

    // move paddle
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) _paddle->moveRight(dt);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) _paddle->moveLeft(dt);
    
    // set mousePos to the psotition of the mouse within the window
    mousePos = sf::Mouse::getPosition(*_window);
    // move paddle if mouse is in the window
    if ((mousePos.x >= _paddle->getWidth() / 2 && mousePos.x < (_window->getSize().x - _paddle->getWidth()/2)) && (mousePos.y >= 0 && mousePos.y < _window->getSize().y))
    {
        _window->setMouseCursorVisible(false);
        _window->setMouseCursorGrabbed(true);
        _paddle->mouseMovement(dt);
    }

    // update everything 
    _paddle->update(dt);
    _ball->update(dt);
    _powerupManager->update(dt);
}

void GameManager::loseLife()
{
    _lives--;
    _ui->lifeLost(_lives);

    // trigger screen shake.
    StartShake(0.5f,15.f,0.08f);
}

void GameManager::StartShake(float duration, float intensity, float fade)
{
    // save the center of the view
    if (!isShaking) {
        originalCenter = _window->getView().getCenter();
    }
    shakeDuration = duration;
    shakeIntensity = intensity;
    shakeFade = fade;
    isShaking = true;
}

void GameManager::UpdateScreenShake(float dt)
{
    if (isShaking) {
        sf::View view = _window->getView();

        if (shakeDuration > 0) {
            // set screens view to original center plus the offset
            offset = GetRandomOffset() * shakeIntensity;

            // apply fading
            shakeIntensity *= (1.0f - shakeFade);

            view.setCenter(originalCenter + offset);
            _window->setView(view);
        }
        else {
            isShaking = false;
            view.setCenter(originalCenter); // reset view to original center
            _window->setView(view);
        }
        // decrease shake duration
        shakeDuration -= dt;
    }
}

void GameManager::RestartGame()
{
    // delete objects
    delete _paddle;
    delete _brickManager;
    delete _messagingSystem;
    delete _ball;
    delete _powerupManager;
    delete _ui;

    // reset variables
    _lives = 3;
    _pause = false;
    _time = 0;
    _pauseHold = 0;
    _levelComplete = false;
    _powerupInEffect = {none,0.0f};
    _timeLastPowerupSpawned = 0.0f;
    _masterText.setString("");

    // re-initialize deleted objects
    initialize();
}

sf::Vector2f GameManager::GetRandomOffset()
{
    // ensures the offset is between -1 and 1
    sf::Vector2f off = sf::Vector2f(((rand() % 100) / 100 * 2 - 1), ((rand() % 100) / 100 * 2 - 1));
    return off;
}


void GameManager::ChangeBackground()
{
    switch (rand() % 5)
    {
    case 0:
        _backgroundColour = sf::Color(100.f, 150.f, 200.f);
        break;
    case 1:
        _backgroundColour = sf::Color(0.f, 0.f, 0.f);
        break;
    case 2:
        _backgroundColour = sf::Color(5.f, 242.f, 68.f);        
        break;
    case 3:
        _backgroundColour = sf::Color(242.f, 5.f, 238.f);
        break;
    case 4:
        _backgroundColour = sf::Color(242.f, 171.f, 5.f);
        break;
    case 5:
        break;
    }
}

sf::Color GameManager::GetBackground()
{
    return _backgroundColour;
}

void GameManager::render()
{
    _window->clear(_backgroundColour);

    _paddle->render();
    _ball->render();
    _brickManager->render();
    _powerupManager->render();
    _window->draw(_masterText);
    _ui->render();
}

void GameManager::levelComplete()
{
    _levelComplete = true;
}

sf::RenderWindow* GameManager::getWindow() const { return _window; }
UI* GameManager::getUI() const { return _ui; }
Paddle* GameManager::getPaddle() const { return _paddle; }
BrickManager* GameManager::getBrickManager() const { return _brickManager; }
PowerupManager* GameManager::getPowerupManager() const { return _powerupManager; }
