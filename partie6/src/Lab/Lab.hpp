#pragma once
#include"Application.hpp"
#include"PetriDish.hpp"
#include "NutrimentGenerator.hpp"
#include "Interface/Drawable.hpp"
#include "Interface/Updatable.hpp"
#include <string>

class Lab : public Updatable, public Drawable
{
public :

    Lab();
    void update (sf::Time dt) override; // met à jour l'état de la boite
    void reset(); //réinitialise l'état de la boite
    void resetControls(); // réinitialise la température et le gradient
    void drawOn (sf::RenderTarget& targetWindow) const override; //dessine
    bool contains(const CircularBody& body) const;//vérifie que la boîte puisse contenir
    bool doesCollideWithDish(CircularBody const& body) const; // permet de savoir si un corps percute
    Nutriment* getNutrimentColliding(CircularBody const& body) const; //si collision avec un nutriment
    void addNutriment(Nutriment* nutriments); //ajoute les nutriments
    void addBacterium(Bacterium* bacteries); //ajoute bactéries
    void addBacterium_clone(Bacterium* bacteries); // ajoute les bactéries obtenues par mitose
    void addSwarm(Swarm* groupe_bacterie); //ajoute un groupe de bactéries
    Swarm* getSwarmWithId(std::string id) const; //renvoie un pointeur sur un groupe précis de bactéries

    double getTemperature() const;
    void decreaseTemperature();
    void increaseTemperature();
    double getGradientExponent() const;
    void decreaseGradientExponent();
    void increaseGradientExponent();
    void resetTemp();
    void resetGrad();

    //score en fonction du gradient, permet un aperçu de la disposition des nutriments
    double getPositionScore(const Vec2d& p, bool poisonous = false) const; //bool pour l'extention : les poisonous ne prennent pas en compte les poisons
    std::unordered_map<std::string, double> fetchData(const std::string & titre); //pour la mise a jour des données des graphes de la boite
    void add_at_graph(std::string nom, bool ajout); //pour le graphe general, appel dans le constructeur/destructeur des bacteries pour "compter" les instances

    //acces a la probabilité de génération des nutriments (bleu/vert)
    double getProb_nutriment() const;
    void decreaseProb_nutriment();
    void increaseProb_nutriment();
    void resetProb_nutriment();

    void addObstacle(Obstacle* obs); //empeche de bouger
    bool doesCollideWithObstacle(CircularBody const& body) const; //collsion avec un obstacle
    Bacterium* getBacteriesColliding(CircularBody const& body) const; //si collision avec une bacterie (virus)
    void addVirus(Virus* V); //ajoute virus
    void addVirus_clone(Virus* V); //ajoute les nouveaux virus créées via les infections (clonage)
    double getPositionBact(const Vec2d& p) const; //donne un gradient pour les bacteries (utile aux virus)
    void remove_separateur(); //ajouter/enlever la separation
    bool doesCollideWithSeparator(CircularBody const& body) const; // gère la collision avec le mur qui coupe la boite en 2
    bool global_colliding(CircularBody const& body) const; //permet un appel à toute les fonction de collision usuel d'un coup (boite, separateur, obstacle)

private :
    PetriDish boite;
    NutrimentGenerator Generateur; //automatise la génération
};
