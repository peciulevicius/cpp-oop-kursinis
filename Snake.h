#ifndef SNAKE_H
#define SNAKE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Entity.h"

enum Direction { UP, DOWN, LEFT, RIGHT };

class Snake : public Entity {
private:
    std::vector<sf::RectangleShape> body;
    Direction direction;
public:
    Snake();
    void changeDirection(Direction newDirection);
    void move();
    void shrink();
    void grow();
    void draw(sf::RenderWindow& window) override;
    bool checkCollision();
    sf::Vector2f getHeadPosition();
    sf::Vector2f getPosition() override;
    const std::vector<sf::RectangleShape>& getBody();
};

#endif // SNAKE_H