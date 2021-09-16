#pragma once
#include <iostream>
#include "Bacterium.hpp"
#include"Utility/DiffEqSolver.hpp"
#include <SFML/Graphics.hpp>
#include <string>


class PoisonousBacterium: public DiffEqFunction, public Bacterium

{
public:
    PoisonousBacterium(const Vec2d& position);//constructeur
    PoisonousBacterium(const PoisonousBacterium& source);//permet de créer une copie
    ~PoisonousBacterium() override; //destructeur
    Bacterium* clone() override;//permet de créer une copie profonde, clone + mutation
    Vec2d f(Vec2d position, Vec2d speed) const override;  //comme bacterie simple
    Vec2d getSpeedVector(); //comme bacterie simple, on ne peux pas mettre const car "speed" est un paramettre mutable
    virtual void mutate() override;   //permet à la bactérie de muter
    j::Value& getConfig() const override; //Raccourci pour les paramètres

    void drawOn(sf::RenderTarget& targetWindow) const override;

    void update(sf::Time dt) override;//met à jour l'etat de la bacterie
    void move(sf::Time dt) override;

    void ajout_poison(Quantity Q);//la bactérie emmagasine du poison avant de le lacher (en fonction de ce qu'elle mange)


    Quantity eatableQuantity(NutrimentA& nutriment) override;
    Quantity eatableQuantity(NutrimentB& nutriment) override;
    Quantity eatableQuantity(Poison& nutriment) override;


private:
    Quantity poison; //quantité en stock
    sf::Time generation; //temps avant la génération du poison
    sf::Time rebond; //temps pendant lequel est voit sa vitesse doublé après avoir laché du poison
    double t; //pour le dessin des tentacules
};
