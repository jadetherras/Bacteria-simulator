#include "Poison.hpp"
#include <iostream>
#include "Config.hpp"
#include "Application.hpp"
#include "Bacterium.hpp"
#include "SimpleBacterium.hpp"
#include "SwarmBacterium.hpp"
#include "TwitchingBacterium.hpp"
#include "PoisonousBacterium.hpp"
#include <SFML/Graphics.hpp>
#include "Utility/Utility.hpp"
#include "Utility/Constants.hpp"

using namespace std;

Poison::Poison(Quantity quantite, const Vec2d& position, double puissance)
    :Nutriment(quantite,position),
     puissance(puissance)
{}

j::Value const&Poison::getConfig() const
{
    return getAppConfig()["nutriments"]["poison"];
}

void Poison::update(sf::Time dt)
{
    rayon = quantite;
}

void Poison::drawOn(sf::RenderTarget& targetWindow) const
{
    Nutriment::drawOn(targetWindow);
    if(isDebugOn()==true)  {
        //affichage de la puissance en plus de la quantite
        Vec2d decalage{0,15}; //decalage du texte par rapport à la quantite
        auto const text = buildText(std::to_string(puissance), position+decalage, getAppFont(), 15, sf::Color::Red, 0.f);
        targetWindow.draw(text);
    }
}

Quantity Poison::eatenBy(Bacterium& bact)
{
    return bact.eatableQuantity(*this);
}

Quantity Poison::eatenBy(SimpleBacterium& bact)
{
    Quantity factor(-puissance);
    return takeQuantity(bact.get_poison())*factor;
}

Quantity Poison::eatenBy(TwitchingBacterium& bact)
{
    Quantity factor(-puissance);
    return takeQuantity(bact.get_poison())*factor;
}

Quantity Poison::eatenBy(SwarmBacterium& bact)
{
    Quantity factor(-puissance);
    return takeQuantity(bact.get_poison())*factor;
}

Quantity Poison::eatenBy(PoisonousBacterium& bact)
{
    Quantity factor(puissance);
    return takeQuantity(bact.get_poison())*factor;
}

//surcharge
bool Poison::est_poison() const
{
    return true;
}
