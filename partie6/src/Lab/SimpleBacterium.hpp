#pragma once
#include <iostream>
#include "Bacterium.hpp"
#include"Utility/DiffEqSolver.hpp"
#include <SFML/Graphics.hpp>
#include <string>


class SimpleBacterium: public DiffEqFunction, public Bacterium

{
public:
    SimpleBacterium(const Vec2d& position);//constructeur
    SimpleBacterium(const SimpleBacterium& source); //copie
    ~SimpleBacterium() override; //destructeur
    Bacterium* clone() override;//permet de créer une copie profonde
    void drawOn(sf::RenderTarget& targetWindow) const override;//dessine avec la flagelle
    void update(sf::Time dt) override;//met à jour l'etat de la bacterie
    void move(sf::Time dt) override; //permet le déplacent en MRU avec des basculements
    Vec2d f(Vec2d position, Vec2d speed) const override; //calule la force qui régit le déplacement qui est nulle dans cette classe
    Vec2d getSpeedVector();  //retourne la vitesse courante
    virtual void mutate() override;   //permet à la bactérie de muter
    j::Value& getConfig() const override; //Raccourci pour les paramètres associés à SimpleBacterium

    Quantity eatableQuantity(NutrimentA& nutriment) override;
    Quantity eatableQuantity(NutrimentB& nutriment) override;
    Quantity eatableQuantity(Poison& nutriment) override;

private:
    double t;
    double ancien_score = 0.0;
    double t_bascule = 0.0;
};
