#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SFML/Graphics.hpp>

class GameObject {
public:
    // virtuali funkcija, skirta objektui piešti
    virtual void draw(sf::RenderWindow& window) = 0;

    // Virtualus destruktorius, užtikrinantis teisingą išvestinių klasių objektų išvalymą
    virtual ~GameObject() = default;
};

#endif // GAMEOBJECT_H