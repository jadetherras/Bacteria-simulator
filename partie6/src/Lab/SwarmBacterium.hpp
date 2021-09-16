#pragma once
#include "Bacterium.hpp"
#include"Utility/DiffEqSolver.hpp"
#include <SFML/Graphics.hpp>
#include "Swarm.hpp"
#include <string>

//on prédéclare la classe (pas de dépendance circulaire)
class Swarm;

class SwarmBacterium : public Bacterium, public DiffEqFunction
{
public:
    SwarmBacterium(const Vec2d& position, Swarm* groupe); //constructeur d'une bactérie de groupe
    SwarmBacterium(const SwarmBacterium& source); //constructeur de copie
    ~SwarmBacterium() override; //destructeur d'une bactérie de groupe

    Vec2d f(Vec2d position,Vec2d speed) const override; //redefinition de la force pour que la bactérie soit attirée par le leader
    void move(sf::Time dt) override;
    Bacterium* clone() override; //clonage
    void mutate() override; //mutation
    j::Value& getConfig() const override;
    void drawOn(sf::RenderTarget& target) const override; //facilite l'accès aux valeur de SwarmBacterium dans app.json
    Vec2d getSpeedVector() const;

    Quantity eatableQuantity(NutrimentA& nutriment) override;
    Quantity eatableQuantity(NutrimentB& nutriment) override;
    Quantity eatableQuantity(Poison& nutriment) override;

private :
    Swarm* appartenance;
};
