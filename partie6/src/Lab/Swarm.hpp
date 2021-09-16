#pragma once
#include <vector>
#include "Utility/Vec2d.hpp"
#include "SwarmBacterium.hpp"
#include "Interface/Updatable.hpp"
#include <SFML/Graphics.hpp>
#include <string>

class SwarmBacterium;

class Swarm : public Updatable //pas dessinable (mais les bacteries oui
{
public:
    Swarm(const std::string& id); //construction groupe
    ~Swarm(); //destruction groupe
    void add_SwarmBacterium(SwarmBacterium* bacterie); //ajoute une bactérie au groupe
    void sup_SwarmBacterium(SwarmBacterium* bacterie); //supprime une bactérie du groupe
    Vec2d position_leader() const; //renvoie la position du leader
    bool is_leader(const SwarmBacterium &bac) const; // retourne true sur la bacterie est le leader
    MutableColor couleur_depart() const; //renvoie la couleur liée à l'identificateur
    std::string get_ident() const; //renvoie l'identificateur du groupe
    void update(sf::Time dt) override;//met à jour l'état du groupe de bactéries
private :
    std::string identificateur;
    std::vector<SwarmBacterium*> groupe; //le groupe
    SwarmBacterium* leader;// pointeur sur la bactérie qui sera suivie par les autres du groupe
};

