#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Snake.h"
#include "Food.h"

class Game {
private:
    sf::RenderWindow window;
    Snake snake;
    Food food;
    sf::Clock clock;
    float delay;
    int score;
    int highScore;
    bool gameOver;
    sf::Font font;
    sf::Text scoreText;
    sf::Text highScoreText;
public:
    Game();
    void run();
    void handleEvents();
    void update();
    void render();
    void gameOverScreen();
    void restartGame();
};

#endif // GAME_H