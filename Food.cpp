#include "Food.h"
#include <cstdlib>
#include <ctime>

const int CELL_SIZE = 20;
const int NUM_CELLS = 30;

Food::Food() {
    food.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    food.setFillColor(sf::Color::Red);
    regenerate(std::vector<sf::RectangleShape>());
}

void Food::regenerate(const std::vector<sf::RectangleShape>& snakeBody) {
    int x, y;
    do {
        x = (rand() % NUM_CELLS) * CELL_SIZE;
        y = (rand() % NUM_CELLS) * CELL_SIZE;
    } while (isFoodOnSnakeBody(x, y, snakeBody));

    food.setPosition(x, y);
}

bool Food::isFoodOnSnakeBody(int x, int y, const std::vector<sf::RectangleShape>& snakeBody) {
    for (const auto& segment : snakeBody) {
        if (segment.getPosition() == sf::Vector2f(x, y)) {
            return true;
        }
    }
    return false;
}

void Food::draw(sf::RenderWindow& window) {
    window.draw(food);
}

sf::Vector2f Food::getPosition() {
    return food.getPosition();
}