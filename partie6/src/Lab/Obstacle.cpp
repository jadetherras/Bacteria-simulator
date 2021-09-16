#include "Obstacle.hpp"
#include <iostream>
#include "Application.hpp"
#include "Config.hpp"
#include "Random/Random.hpp"
#include <SFML/Graphics.hpp>
#include "Utility/Utility.hpp"

using namespace std;

Obstacle::Obstacle(const Vec2d &position)
    :CircularBody(position, uniform(getConfig()["taille"]["min"].toDouble(), getConfig()["taille"]["max"].toDouble())),
     collision(0)
{}

j::Value& Obstacle::getConfig() const
{
    return getAppConfig()["obstacles"];
}

void Obstacle::drawOn(sf::RenderTarget& target) const
{
    //dessin juste comme un cercle

    /* auto const circle = buildCircle(position, rayon, sf::Color::White);
     target.draw(circle);
     if(isDebugOn()==true)  {
         auto const text = buildText(std::to_string(collision), position, getAppFont(), 15, sf::Color::Black, 0.f);
         target.draw(text);
     }*/

    //dessin avec texture (libre de droit)

    auto const& texture = getAppTexture(getConfig()["texture"].toString());
    auto obstacleSprite = buildSprite(position, 6, texture);
    // adapte la taille du Sprite au rayon du nutriment:
    obstacleSprite.setScale(2 * rayon / texture.getSize().x, 2 * rayon / texture.getSize().y);
    target.draw(obstacleSprite);

    if (isDebugOn()) {
        auto const text = buildText(std::to_string(collision), position, getAppFont(), 15, sf::Color::Black, 0.f);
        target.draw(text);
    }
}

bool Obstacle::en_collision(const CircularBody &body)
{
    if(isColliding(body)) {
        ++collision;
        return true;
    }
    return false;
}
