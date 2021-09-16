#include "NutrimentB.hpp"
#include <iostream>
#include "Config.hpp"
#include "Application.hpp"
#include "Bacterium.hpp"
#include "SimpleBacterium.hpp"
#include "SwarmBacterium.hpp"
#include "TwitchingBacterium.hpp"
#include "PoisonousBacterium.hpp"

using namespace std;

NutrimentB::NutrimentB(Quantity quantite, const Vec2d& position)
    : Nutriment(quantite, position)
{}

j::Value const&NutrimentB::getConfig() const
{
    return getAppConfig()["nutriments"]["B"];
}

Quantity NutrimentB::eatenBy(Bacterium& bact)
{
    return bact.eatableQuantity(*this);
}

Quantity NutrimentB::eatenBy(SimpleBacterium& bact)
{
    Quantity factor = getConfig()["resistance factor"].toDouble();
    return takeQuantity(bact.get_max_meal()/factor);
}

Quantity NutrimentB:: eatenBy(TwitchingBacterium& bact)
{
    Quantity factor = getConfig()["nutritive factor"].toDouble();
    return takeQuantity(bact.get_max_meal())*factor;
}

Quantity NutrimentB::eatenBy(SwarmBacterium& bact)
{
    Quantity factor = getConfig()["poison factor"].toDouble();
    return -takeQuantity(bact.get_max_meal())*factor;
}

Quantity NutrimentB::eatenBy(PoisonousBacterium& bact)
{
    Quantity Q(takeQuantity(bact.get_max_meal()));
    bact.ajout_poison(Q*getAppConfig()["poisonous bacterium"]["production factor"].toDouble());
    return Q;
}
