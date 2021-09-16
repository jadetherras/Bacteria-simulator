#include"Grip.hpp"
#include"Utility/Vec2d.hpp"

using namespace std;

Grip::Grip(const Vec2d &position, double rayon)
    :CircularBody(position,rayon)
{}

void Grip::move(const Vec2d &delta)
{
    CircularBody::move(delta); //methode de la super-classe
}
