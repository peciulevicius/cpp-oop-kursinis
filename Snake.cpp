#include "Snake.h"

const int CELL_SIZE = 20;
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;

Snake::Snake() {
    sf::RectangleShape segment(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    segment.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    segment.setFillColor(sf::Color::Green);
    body.push_back(segment);
    direction = RIGHT;
}

void Snake::changeDirection(Direction newDirection) {
    if ((direction == UP && newDirection != DOWN) ||
        (direction == DOWN && newDirection != UP) ||
        (direction == LEFT && newDirection != RIGHT) ||
        (direction == RIGHT && newDirection != LEFT)) {
        direction = newDirection;
    }
}

void Snake::move() {
    sf::Vector2f newHeadPos = body[0].getPosition();
    switch (direction) {
        case UP: newHeadPos.y -= CELL_SIZE; break;
        case DOWN: newHeadPos.y += CELL_SIZE; break;
        case LEFT: newHeadPos.x -= CELL_SIZE; break;
        case RIGHT: newHeadPos.x += CELL_SIZE; break;
    }

    sf::RectangleShape newHead(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    newHead.setPosition(newHeadPos);
    newHead.setFillColor(sf::Color::Green);
    body.insert(body.begin(), newHead);
}

void Snake::shrink() {
    body.pop_back();
}

void Snake::grow() {
    sf::RectangleShape newSegment(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    newSegment.setPosition(body.back().getPosition());
    newSegment.setFillColor(sf::Color::Green);
    body.push_back(newSegment);
}

void Snake::draw(sf::RenderWindow& window) {
    for (auto& segment : body) {
        window.draw(segment);
    }
}

bool Snake::checkCollision() {
    sf::Vector2f headPos = body[0].getPosition();
    if (headPos.x < 0 || headPos.y < 0 || headPos.x >= WINDOW_WIDTH || headPos.y >= WINDOW_HEIGHT) {
        return true;
    }
    for (size_t i = 1; i < body.size(); ++i) {
        if (body[i].getPosition() == headPos) {
            return true;
        }
    }
    return false;
}

sf::Vector2f Snake::getHeadPosition() {
    return body[0].getPosition();
}

sf::Vector2f Snake::getPosition() {
    return getHeadPosition();
}

const std::vector<sf::RectangleShape>& Snake::getBody() {
    return body;
}