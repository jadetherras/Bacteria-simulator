#pragma once
#include <iostream>
#include "Interface/Drawable.hpp"
#include "Interface/Updatable.hpp"
#include "CircularBody.hpp"
#include "Utility/Types.hpp"
#include "Utility/Vec2d.hpp"
#include "Utility/MutableColor.hpp"
#include "Utility/MutableNumber.hpp"
#include <string>
#include<vector>
#include "Bacterium.hpp"
#include <SFML/Graphics.hpp>
#include "Utility/DiffEqSolver.hpp"

class Virus : public CircularBody, public Drawable, public Updatable, public DiffEqFunction
{
public :
    Vec2d f(Vec2d position, Vec2d speed) const override;
    Virus(const Vec2d& position); //constructeur de virus
    Virus(const Virus& V); //copie
    ~Virus(); //destructeur de virus
    void drawOn(sf::RenderTarget& target) const override;
    void update(sf::Time dt) override; //mise à jour
    void divide(); //division spécifique du virus (a besoin d'une bactérie)
    void mutate(); //mutation
    bool morte() const;
    j::Value& getConfig() const; //accès aux données de app.json
    void consumeEnergy(Quantity qt);
    Virus* clone();// clonage
    void move(sf::Time dt);
    Vec2d getSpeedVector();
    MutableNumber* getProperty(const std::string& cle);
    int get_generation() const;
    void setEnergie(Quantity qt);

private :
    Vec2d direction;
    Bacterium* cible; //bacterie ciblée
    Quantity energie;
    MutableColor couleur;
    sf::Time propagation_infection; //temps avant la division
    std::map<std::string, MutableNumber> parametre;
    int generation;
};

