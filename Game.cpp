#include "Game.h"
#include <iostream>

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;

Game::Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Snake Game"), delay(0.2f), score(0), highScore(0), gameOver(false) {
    srand(static_cast<unsigned int>(time(0)));

   // Load font
    if (!font.loadFromFile("../resources/arial.ttf")) {
        std::cerr << "Could not load font!" << std::endl;
    }

    // Initialize score text
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);

    // Initialize high score text
    highScoreText.setFont(font);
    highScoreText.setCharacterSize(24);
    highScoreText.setFillColor(sf::Color::White);
    highScoreText.setPosition(10, 40);
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        if (!gameOver) {
            update();
        }
        render();
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        snake.changeDirection(UP);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        snake.changeDirection(DOWN);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        snake.changeDirection(LEFT);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        snake.changeDirection(RIGHT);
    }

    if (gameOver) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            restartGame();
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            window.close();
        }
    }
}

void Game::update() {
    if (clock.getElapsedTime().asSeconds() > delay) {
        snake.move();
        if (snake.getHeadPosition() == food.getPosition()) {
            snake.grow();
            food.regenerate(snake.getBody());
            score += 10;
             if (score > highScore) {
                highScore = score;
            }
        } else {
            snake.shrink();
        }
        if (snake.checkCollision()) {
            std::cout << "Game Over! Your score: " << score << std::endl;
            gameOver = true;
        }
        clock.restart();
    }
}

void Game::render() {
    window.clear();
    snake.draw(window);
    food.draw(window);

    // Update score text
    scoreText.setString("Score: " + std::to_string(score));
    window.draw(scoreText);

    // Update high score text
    highScoreText.setString("High Score: " + std::to_string(highScore));
    window.draw(highScoreText);

    if (gameOver) {
        gameOverScreen();
    }

    window.display();
}

void Game::gameOverScreen() {
    sf::Font font;
    if (!font.loadFromFile("../resources/arial.ttf")) {
        std::cerr << "Could not load font!" << std::endl;
        return;
    }

    sf::Text text;
    text.setFont(font);
    text.setString("Game Over! Press R to Restart or Q to Quit");
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(50, WINDOW_HEIGHT / 2);

    window.draw(text);
}

void Game::restartGame() {
    score = 0;
    snake = Snake();
    food.regenerate(snake.getBody());
    gameOver = false;
}