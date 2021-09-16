#pragma once
#include <SFML/Graphics.hpp>
#include "Interface/Updatable.hpp"

class NutrimentGenerator : public Updatable
{
public:
    NutrimentGenerator(); //constructeur
    void update(sf::Time dt) override;
    void reset(); //ramène le compteur à 0
private :
    sf::Time compteur;
};

