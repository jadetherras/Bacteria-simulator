#pragma once
#include "Graph.hpp"
#include "Interface/Drawable.hpp"
#include "Interface/Updatable.hpp"
#include <vector>
#include <iostream>
#include <string>

struct Graphique { //structure représentant le graphique et son nom
    std::unique_ptr<Graph> graphe; //pointeur unique, un graphe ne doit être représenté qu'une fois
    std::string nom;
};

class Stats : public Drawable, public Updatable
{
public :
    Stats(); //Constructeur
    void drawOn(sf::RenderTarget& target) const override; //affichage des stats (graphe actif)
    void update(sf::Time dt) override;//mise à jour des stats
    std::string getCurrentTitle() const; //titre du graphe actif
    void next(); //permet d'obtenir le graphe suivant lors du parcours du tableau de données
    void previous();//permet d'obtenir le graphe précédent lors du parcours du tableau de données
    void reset(); //réinitialise les données
    void addGraph(int identifiant, const std::string& titre, const std::vector<std::string>& titres, double min, double max, const Vec2d& size); //ajout d'un graphe
    void setActive(int identifiant); //fait considérer le paramètre comme l'identifiant

private :
    int identifiant_actif; //graphique à représenter
    std::vector<Graphique*> ensemble; //pour regrouper ensemble le titre et le graphique, tout en ayant un identifiant commun assurément. Les identifiants ce suivant, on utilise la position dans le vecteur.
    sf::Time compteur;
};

