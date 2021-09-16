#include<iostream>
#include"Application.hpp"
#include"PetriDish.hpp"
#include"Lab/Lab.hpp"
#include<SFML/Graphics.hpp>
#include"PetriDish.hpp"
using namespace std;

Lab::Lab()
    : boite(getApp().getCentre(),0.95*getApp().getLabSize().x/2)
    , Generateur()
{}


void Lab::reset()
{
    boite.reset();
    Generateur.reset();
    resetControls();
}

void Lab::resetControls()
{
    boite.resetTemp();
    boite.resetGrad();
    boite.resetProb_nutriment();
}

void Lab::resetTemp()
{
    boite.resetTemp();
}

void Lab::resetGrad()
{
    boite.resetGrad();
}

void Lab::drawOn (sf::RenderTarget& targetWindow) const
{
    boite.drawOn(targetWindow);
}

void Lab::update (sf::Time dt)
{
    boite.update(dt);
    Generateur.update(dt);
}

bool Lab::contains(const CircularBody& body) const
{
    return boite>body;
}

bool Lab::doesCollideWithDish(CircularBody const& body) const
{
    return (body.isColliding(boite) && !contains(body));
}

Nutriment* Lab::getNutrimentColliding(CircularBody const& body) const
{
    return boite.getNutrimentColliding(body);
}

//BONUS on ne peux pas ajouter sur les obstacle (éviter les incohérences pour les nutriments et les bugs des bactéries
void Lab::addNutriment(Nutriment* nutriments)
{
    if (nutriments != nullptr and contains(*nutriments) and !doesCollideWithObstacle(*nutriments) and !doesCollideWithSeparator(*nutriments)) {
        boite.addNutriment(nutriments);
    }
}

void Lab::addBacterium(Bacterium *bacteries)
{
    if (bacteries != nullptr and contains(*bacteries) and !doesCollideWithObstacle(*bacteries) and !doesCollideWithSeparator(*bacteries)) {
        boite.addBacterium(bacteries);
    }
}

void Lab::addSwarm(Swarm* groupe_bacterie)
{
    if(groupe_bacterie != nullptr) {
        boite.addSwarm(groupe_bacterie);
    }
}

Swarm* Lab::getSwarmWithId(string id) const
{
    return boite.getSwarmWithId(id);
}

//BONUS de meme
void Lab::addBacterium_clone(Bacterium *bacteries)
{
    if (bacteries != nullptr and contains(*bacteries) and !doesCollideWithObstacle(*bacteries) and !doesCollideWithSeparator(*bacteries)) {
        boite.addBacterium_clone(bacteries);
    }
}

double Lab::getTemperature() const
{
    return boite.getTemperature();
}

void Lab::decreaseTemperature()
{
    boite.decreaseTemperature();
}

void Lab::increaseTemperature()
{
    boite.increaseTemperature();
}

double Lab::getGradientExponent() const
{
    return boite.getGradientExponent();
}

void Lab::decreaseGradientExponent()
{
    boite.decreaseGradientExponent();
}

void Lab::increaseGradientExponent()
{
    boite.increaseGradientExponent();
}

double Lab::getPositionScore(const Vec2d& p, bool poisonous) const
{
    return boite.getPositionScore(p, poisonous);
}

std::unordered_map<std::string, double> Lab:: fetchData(const std::string & titre)
{
    return boite.fetchData(titre);
}


//BONUS

void Lab::addObstacle(Obstacle* obs)
{
    if(obs != nullptr and contains(*obs) and !doesCollideWithSeparator(*obs)) {
        boite.addObstacle(obs);
    }
}

bool Lab::doesCollideWithObstacle(CircularBody const& body) const
{
    return boite.doesCollideWithObstacle(body);
}

Bacterium* Lab::getBacteriesColliding(CircularBody const& body) const
{
    return boite.getBacteriesColliding(body);
}

void Lab::addVirus(Virus* V)
{
    if (V != nullptr and contains(*V) and !doesCollideWithObstacle(*V) and !doesCollideWithSeparator(*V)) {
        boite.addVirus(V);
    }
}

void Lab::addVirus_clone(Virus* V)
{
    if (V != nullptr and contains(*V) and !doesCollideWithObstacle(*V) and !doesCollideWithSeparator(*V)) {
        boite.addVirus_clone(V);
    }
}

double Lab::getPositionBact(const Vec2d& p) const
{
    return boite.getPositionBact(p);
}

void Lab::remove_separateur()
{
    boite.remove_separateur();
}

bool Lab::doesCollideWithSeparator(CircularBody const& body) const
{
    return boite.doesCollideWithSeparator(body);
}

bool Lab::global_colliding(CircularBody const& body) const
{
    return doesCollideWithDish(body) or doesCollideWithObstacle(body) or doesCollideWithSeparator(body);
}


double Lab::getProb_nutriment() const
{
    return boite.getProb_nutriment();
}

void Lab::decreaseProb_nutriment()
{
    boite.decreaseProb_nutriment();
}

void Lab::increaseProb_nutriment()
{
    boite.increaseProb_nutriment();
}

void Lab::resetProb_nutriment()
{
    boite.resetProb_nutriment();
}

void Lab::add_at_graph(std::string nom, bool ajout)
{
    boite.add_at_graph(nom, ajout);
}
