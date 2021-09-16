#pragma once
#include "Utility/Vec2d.hpp"
#include "CircularBody.hpp"

class Grip : public CircularBody
{
public :
    Grip (const Vec2d& position, double rayon); //constructeur
    void move(const Vec2d& delta); //mouvement du tentacule
};
