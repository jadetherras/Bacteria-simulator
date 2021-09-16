#pragma once
#include <iostream>
#include "Utility/Types.hpp"
#include "CircularBody.hpp"
#include <SFML/Graphics.hpp>
#include "Interface/Drawable.hpp"
#include "Interface/Updatable.hpp"

//dependance circulaire : class prédéclaré avant la class nutriment
class Bacterium;
class SwarmBacterium;
class SimpleBacterium;
class TwitchingBacterium;
class PoisonousBacterium;

class Nutriment : public CircularBody, public Drawable, public Updatable
{
public :
    Nutriment(Quantity quantite, const Vec2d& position);//constructeur
    Nutriment(const Vec2d& position);
    virtual ~Nutriment(); //utilisation polymorphique !
    Quantity takeQuantity(Quantity a_prelever);
    void setQuantity(Quantity quantite);
    bool morte() const; //pour permettre l'élimination des nutriments consommés
    void drawOn(sf::RenderTarget& target) const override; //dessin des nutriments
    virtual j::Value const&getConfig() const =0;
    void update(sf::Time dt) override;

    //définit la quantité d'energie ingérable par les différents type de bactéries
    virtual Quantity eatenBy(Bacterium& bact) = 0;
    virtual Quantity eatenBy(SimpleBacterium& bact) = 0;
    virtual Quantity eatenBy(TwitchingBacterium& bact) = 0;
    virtual Quantity eatenBy(SwarmBacterium& bact) = 0;
    virtual Quantity eatenBy(PoisonousBacterium& bact) = 0;

    virtual bool est_poison() const; //permet de savoir si on à affaire à du poison ou non
    //préféré à un attribut et un getter

protected :
    Quantity quantite;
};
