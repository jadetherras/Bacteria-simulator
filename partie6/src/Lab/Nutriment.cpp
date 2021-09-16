#include "Nutriment.hpp"
#include "Application.hpp"
#include "Utility/Utility.hpp"
#include "Config.hpp"
#include "Random/Random.hpp"
#include "cmath"
#include "Bacterium.hpp"
#include "SimpleBacterium.hpp"
#include "SwarmBacterium.hpp"
#include "TwitchingBacterium.hpp"
#include "PoisonousBacterium.hpp"

using namespace std;

Nutriment::Nutriment(Quantity quantite, const Vec2d& position)
    : CircularBody(position, quantite)
    , quantite(quantite)
{}

Nutriment::~Nutriment() {}

Quantity Nutriment::takeQuantity(Quantity a_prelever)
{
    if (quantite>=a_prelever) {
        quantite -= a_prelever;
    } else {
        a_prelever = quantite;
        quantite = 0;
    }
    rayon = quantite;
    return a_prelever; //prélève le maximum de nutriment possible
}

void Nutriment::setQuantity(Quantity quantite)
{
    if (quantite < 0) {
        quantite = 0;
    }
    this->quantite = quantite;
    rayon = quantite;
}

void Nutriment::drawOn(sf::RenderTarget& target) const
{
    auto const& texture = getAppTexture(getConfig()["texture"].toString());
    auto nutrimentSprite = buildSprite(position, 6, texture);
    // adapte la taille du Sprite au rayon du nutriment:
    nutrimentSprite.setScale(2 * rayon / texture.getSize().x, 2 * rayon / texture.getSize().y);
    target.draw(nutrimentSprite);

    if (isDebugOn()) {
        auto const text = buildText(to_string(quantite), position, getAppFont(), 15,
                                    sf::Color::Black);
        target.draw(text);
    }
}

void Nutriment::update(sf::Time dt)
{
    double minTemp(getConfig()["growth"]["min temperature"].toDouble());
    double maxTemp(getConfig()["growth"]["max temperature"].toDouble());
    double temp(getAppEnv().getTemperature());
    Quantity max(2*getConfig()["quantity"]["max"].toDouble());
    Quantity init(quantite);

    //gestion du développement en fonction de la température de la boite
    if (temp>=minTemp and temp<=maxTemp) {
        double speed = getConfig()["growth"]["speed"].toDouble();
        auto growth = speed * dt.asSeconds();
        if (quantite+growth <= max) {
            quantite +=growth;
        } else {
            quantite = max;
        }
        //si il rentre en contact avec un bord, le nutriment ne grandit pas
        if(getAppEnv().global_colliding(*this)) {
            quantite = init;
        }
    }
    rayon = quantite;
}

bool Nutriment::morte() const
{
    return quantite == 0;
}

bool Nutriment::est_poison() const
{
    return false; //par définition faux, redéfinit pour les poisons
}
