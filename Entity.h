#ifndef ENTITY_H
#define ENTITY_H

#include "GameObject.h"

// Abstrakti klasė, apibrėžianti bendrą sąsają visiems žaidimo objektams
class Entity : public GameObject {
public:
    // virtuali funkcija, skirta gauti objekto poziciją
    virtual sf::Vector2f getPosition() = 0;

    // Virtualus destruktorius, užtikrinantis teisingą išvestinių klasių objektų išvalymą
    virtual ~Entity() = default;
};

#endif // ENTITY_H