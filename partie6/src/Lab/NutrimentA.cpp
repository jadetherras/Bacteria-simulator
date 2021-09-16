#include "NutrimentA.hpp"
#include <iostream>
#include "Config.hpp"
#include "Application.hpp"
#include "Bacterium.hpp"
#include "SimpleBacterium.hpp"
#include "SwarmBacterium.hpp"
#include "TwitchingBacterium.hpp"
#include "PoisonousBacterium.hpp"

using namespace std;

NutrimentA::NutrimentA(Quantity quantite, const Vec2d& position)
    : Nutriment(quantite, position) //appel au constructeur super-classe
{}

j::Value const&NutrimentA::getConfig() const
{
    return getAppConfig()["nutriments"]["A"];
}

Quantity NutrimentA::eatenBy(Bacterium& bact)
{
    return bact.eatableQuantity(*this);
}

Quantity NutrimentA::eatenBy(SimpleBacterium& bact)
{
    return takeQuantity(bact.get_max_meal());
}

Quantity NutrimentA::eatenBy(TwitchingBacterium& bact)
{
    return takeQuantity(bact.get_max_meal());
}

Quantity NutrimentA::eatenBy(SwarmBacterium& bact)
{
    return takeQuantity(bact.get_max_meal());
}

Quantity NutrimentA::eatenBy(PoisonousBacterium& bact)
{
    return takeQuantity(bact.get_max_meal());
}
