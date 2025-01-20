#ifndef FOOD_H
#define FOOD_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Entity.h"

class Food : public Entity {
private:
    sf::RectangleShape food;
public:
    Food();
    void regenerate(const std::vector<sf::RectangleShape>& snakeBody);
    bool isFoodOnSnakeBody(int x, int y, const std::vector<sf::RectangleShape>& snakeBody);
    void draw(sf::RenderWindow& window) override;
    sf::Vector2f getPosition() override;
};

#endif // FOOD_H