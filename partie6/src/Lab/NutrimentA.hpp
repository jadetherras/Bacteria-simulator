#pragma once
#include "Nutriment.hpp"

class NutrimentA : public Nutriment
{
public:
    NutrimentA(Quantity quantite, const Vec2d& position);
    j::Value const&getConfig() const override;

    //redéfinition dans les sous-classes
    Quantity eatenBy(Bacterium& bact) override;
    Quantity eatenBy(SimpleBacterium& bact) override;
    Quantity eatenBy(TwitchingBacterium& bact) override;
    Quantity eatenBy(SwarmBacterium& bact) override;
    Quantity eatenBy(PoisonousBacterium& bact) override;
};
