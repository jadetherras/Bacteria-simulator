#pragma once
#include "Nutriment.hpp"

class Poison : public Nutriment
{
public:
    Poison(Quantity quantite, const Vec2d& position, double puissance); //constructeur
    j::Value const&getConfig() const override; //facilite l'accès aux valeur de poison ajoutées dans app.json
    void update(sf::Time dt) override;//mise à jour
    void drawOn(sf::RenderTarget& targetWindow) const override; //représente le poison sur l'interface
    bool est_poison() const override; //redéfinition de la fonction est poison pour signifier sa nature


    //définit les dégats du poison pour chaque type de bactérie
    Quantity eatenBy(Bacterium& bact) override;
    Quantity eatenBy(SimpleBacterium& bact) override;
    Quantity eatenBy(TwitchingBacterium& bact) override;
    Quantity eatenBy(SwarmBacterium& bact) override;
    Quantity eatenBy(PoisonousBacterium& bact) override;

private :
    double const puissance; //definit la puissance du poison
};
