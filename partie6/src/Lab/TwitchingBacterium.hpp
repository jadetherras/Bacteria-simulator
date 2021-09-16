#pragma once
#include <iostream>
#include "Bacterium.hpp"
#include"Utility/DiffEqSolver.hpp"
#include <SFML/Graphics.hpp>
#include"Grip.hpp"
#include <string>

enum etat {EAT, RETRACT, ATTRACT, DEPLOY, WAIT_TO_DEPLOY, IDLE}; //ensemble des états

class TwitchingBacterium : public Bacterium, public DiffEqFunction
{
public :

    TwitchingBacterium(const Vec2d& position); //constructeur bacterie à tentacule
    TwitchingBacterium(const TwitchingBacterium& source); //copie
    ~TwitchingBacterium() override; //destructeur bactérie à tentacule

    void mutate() override; //mutation
    Bacterium* clone() override; //clonage
    void drawOn(sf::RenderTarget& target) const override;
    Vec2d f(Vec2d position, Vec2d speed) const override;//Force pour obtenir un MRU
    void update(sf::Time dt) override;
    void move(sf::Time dt) override;
    j::Value& getConfig() const override;//facilite l'accès aux valeur de TwitchingBacterium dans app.json
    Quantity get_cons_move() const; //perte d'energie due au mouvement
    Quantity get_cons_tent() const; //perte d'energie due au tentacule
    void draw_tent_grapin(sf::RenderTarget& target) const;
    void moveGrip(const Vec2d& delta); //déplacement du tentacule

    Quantity eatableQuantity(NutrimentA& nutriment) override;
    Quantity eatableQuantity(NutrimentB& nutriment) override;
    Quantity eatableQuantity(Poison& nutriment) override;

private :
    Grip grapin;
    etat phase;
};
