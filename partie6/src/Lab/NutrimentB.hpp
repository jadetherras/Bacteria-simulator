#pragma once
#include "Nutriment.hpp"


class NutrimentB : public Nutriment
{
public:
    NutrimentB(Quantity quantite, const Vec2d& position);
    j::Value const&getConfig() const override;

    //redéfinition dans les sous-classes.
    //pour les nutriments B, les effets sont différent.
    Quantity eatenBy(Bacterium& bact) override;
    Quantity eatenBy(SimpleBacterium& bact) override;
    Quantity eatenBy(TwitchingBacterium& bact) override;
    Quantity eatenBy(SwarmBacterium& bact) override;
    Quantity eatenBy(PoisonousBacterium& bact) override;
};

