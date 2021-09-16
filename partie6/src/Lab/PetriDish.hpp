#pragma once
#include<SFML/Graphics.hpp>
#include "Nutriment.hpp"
#include "Bacterium.hpp"
#include "CircularBody.hpp"
#include "Utility/Vec2d.hpp"
#include <iostream>
#include <vector>
#include "Interface/Drawable.hpp"
#include "Interface/Updatable.hpp"
#include "Swarm.hpp"
#include <string>
#include "Obstacle.hpp"
#include "Virus.hpp"

class PetriDish : public CircularBody, public Drawable, public Updatable
{
public :

    PetriDish (const Vec2d& position, double rayon); //Constructeur
    PetriDish (const PetriDish& Petri)=delete; //Interdit la copie
    ~PetriDish ();//Destructeur

    PetriDish& operator=(const PetriDish& Petri)=delete; //Interdit l'affectation

    bool addBacterium(Bacterium* bacteries); // Pour savoir si l'ajout est un succès
    bool addBacterium_clone(Bacterium* bacteries);
    bool addNutriment(Nutriment* nutriments);
    bool addSwarm(Swarm* groupe_b);

    Swarm* getSwarmWithId(std::string id) const;

    void update (sf::Time dt) override; //Faire évoluer les bactéries en fonction du temps
    void drawOn (sf::RenderTarget& targetWindow) const override; //Dessine la boite avec les nutriments et les bactéries
    void reset(); //supprime les bactéries et nutriments
    Nutriment* getNutrimentColliding(CircularBody const& body) const;
    double getPositionScore(const Vec2d& p, bool poisonous = false) const;

    //gestion du gradient
    double getGradientExponent() const;
    void decreaseGradientExponent();
    void increaseGradientExponent();
    void resetGrad();

    //gestion de la température
    double getTemperature() const;
    void decreaseTemperature();
    void increaseTemperature();
    void resetTemp();

    std::unordered_map<std::string, double> fetchData(const std::string & titre); //renvoie l'unordered_map contenant le titre de l'entité et le nombre qui correspond

    bool addObstacle(Obstacle* obs);//ajoute obstacle
    bool doesCollideWithObstacle(CircularBody const& body) const; //permet de savoir

    Bacterium* getBacteriesColliding(CircularBody const& body) const;
    bool addVirus(Virus* V);//ajoute virus
    bool addVirus_clone(Virus* V);//ajoute les virus issues de la reproduction
    double getPositionBact(const Vec2d& p) const;//renvoie la position d'une bactérie

    void remove_separateur(); //ajoute/enlève le séparateur
    bool doesCollideWithSeparator(const CircularBody& body) const; //permet de savoir s'il y a collision avec le mur séparateur

    //gestion de la probabilité des nutriments
    void increaseProb_nutriment();
    void resetProb_nutriment();
    void decreaseProb_nutriment();
    double getProb_nutriment() const;

    //ajout des bacteries au graphe general
    void add_at_graph(std::string nom, bool ajout);

private :

    std::vector<Bacterium*> bacteries; // tableau des adresses des bacteries dans la boîtes
    std::vector<Bacterium*> bacteries_sup;
    std::vector<Swarm*> groupe_bacteries;
    std::vector<Nutriment*> nutriments; //Pour prévoir le cas de la boîte vide on utilise des tableaux dynamiques de pointeurs
    std::vector<Obstacle*> obstacles;//tableau des adresses des obstacles dans la boîte
    std::vector<Virus*> virus; // tableau des adresses des virus dans la boîtes
    std::vector<Virus*> virus_sup; //tableau des adresses des virus issues de la reproduction

    double temperature;
    double puissance;
    double prob_nutriment;
    bool separateur;//mur qui coupe la boîte en 2 : est-il là ou non?

    std::unordered_map<std::string, double> nb; //pour le graphe général, les bacteries
};
