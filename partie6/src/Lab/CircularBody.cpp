#include "CircularBody.hpp"
#include "Utility/Vec2d.hpp"
#include <cmath>
using namespace std;

CircularBody::CircularBody(const Vec2d& position, double rayon)
    : position(position)
    , rayon(rayon)
{}

CircularBody::CircularBody(const CircularBody& other)
    : position(other.position)
    , rayon(other.rayon)
{}

const Vec2d CircularBody::getPosition() const
{
    return position;
}
void CircularBody::setPosition(const Vec2d& position)
{
    this->position=position;
}

double CircularBody::getRadius() const
{
    return rayon;
}
void CircularBody::setRadius(double rayon)
{
    this->rayon=rayon;
}

CircularBody& CircularBody::operator=(const CircularBody& other)
{
    rayon = other.rayon;
    position = other.position;
    return (*this);
}

bool CircularBody::operator>(const CircularBody& body2) const
{
    return contains(body2);
}

bool CircularBody::operator&(const CircularBody& body2) const
{
    return isColliding(body2);
}

void CircularBody::move(const Vec2d& mouvement)
{
    position += mouvement;
}

bool CircularBody::contains(const CircularBody& other) const
{
    return (rayon >= other.rayon and distance(position, other.position)<= abs(other.rayon-rayon));
}

bool CircularBody::contains(const Vec2d& p) const
{
    return distance(p, position) <= rayon;
}

bool CircularBody::isColliding(const CircularBody& other) const
{
    return (distance(position, other.position) <= (rayon+other.rayon));
}

bool operator>(CircularBody body, const Vec2d& point)
{
    return body.CircularBody::contains(point);
}

ostream& operator<<(ostream& out, const CircularBody& other)
{
    out << "CircularBody: position = " << other.CircularBody::getPosition();
    out << " radius = " << other.CircularBody::getRadius();
    return out;
}

