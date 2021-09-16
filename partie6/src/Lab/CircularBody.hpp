#pragma once
#include "Utility/Vec2d.hpp"
#include <iostream>

class CircularBody //super-classe
{
public :
    CircularBody(const Vec2d& position, double rayon);
    CircularBody(const CircularBody& other);
    const Vec2d getPosition() const;
    double getRadius() const;

    //surcharge des operateurs
    CircularBody& operator=(const CircularBody& other);
    bool operator>(const CircularBody& body2) const;
    bool operator&(const CircularBody& body2) const;

    //deplacement
    void move(const Vec2d& mouvement);

    bool contains(const CircularBody& other) const;
    bool contains(const Vec2d& point) const;

    bool isColliding(const CircularBody& other) const;

protected :
    Vec2d position;
    double rayon;

    void setPosition(const Vec2d& position);
    void setRadius(double rayon);

};

//surcharge externe pour les operateurs d'affichage et de comparaison
bool operator>(CircularBody body, const Vec2d& point);
std::ostream& operator<<(std::ostream& out, const CircularBody& other);
