#pragma once
#include "CircularBody.hpp"
#include "Interface/Drawable.hpp"
#include <SFML/Graphics.hpp>

class Obstacle : public CircularBody, public Drawable
{
public:
    Obstacle(const Vec2d& position); //constructeur
    j::Value& getConfig() const; //facilite l'accès aux valeur de obstacle ajoutées dans app.json
    void drawOn(sf::RenderTarget& target) const override; //défini la manière de dessiner un obstacle
    bool en_collision(const CircularBody& body); //fait savoir si l'obstacle entre en collision avec un autre corps
private :
    int collision; //compteur de collision
};
