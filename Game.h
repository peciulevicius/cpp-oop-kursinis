#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Snake.h"
#include "Food.h"
#include "Container.h"

// Žaidimo klasė
class Game {
private:
    sf::RenderWindow window; // Žaidimo langas
    Snake snake; // Gyvatė
    Food food; // Maistas
    sf::Clock clock; // Laikrodis
    float delay;
    int score;
    int highScore;
    bool gameOver;
    sf::Font font;
    sf::Text scoreText;
    sf::Text highScoreText;
    void handleEvents();
    void update();
    void render();
    void gameOverScreen();
    void restartGame();

    Container<Snake> snakeContainer;
    Container<Food> foodContainer;
public:
    Game();
    void run();
};

#endif // GAME_H