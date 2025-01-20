#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>

// Abstrakti klasė, apibrėžianti bendrą sąsają visiems žaidimo objektams
class Entity {
public:
    // Grynoji virtuali funkcija, skirta objektui piešti
    virtual void draw(sf::RenderWindow& window) = 0;

    // Grynoji virtuali funkcija, skirta gauti objekto poziciją
    virtual sf::Vector2f getPosition() = 0;

    // Virtualus destruktorius, užtikrinantis teisingą išvestinių klasių objektų išvalymą
    virtual ~Entity() = default;
};

#endif // ENTITY_H