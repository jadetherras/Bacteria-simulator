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
#include <SFML/Graphics.hpp>
#include "Poison.hpp"

class Nutriment;
class NutrimentA;
class NutrimentB;

class Bacterium : public CircularBody, public Drawable, public Updatable
{

public:

    Bacterium(Quantity energy, const Vec2d &position, const Vec2d &direction, double rayon,  MutableColor couleur);//constructeur
    Bacterium(const Bacterium& source); // constructeur de copie
    virtual ~Bacterium();//destructeur VIRTUEL de la bacterie car utilisation polymorphique dans la sous-classe
    void drawOn(sf::RenderTarget& target) const override;//dessin des bacteries
    void update(sf::Time dt) override;//actualise l'etat de la bactérie
    virtual void divide();//général la mutation d'une bactérie
    virtual void mutate() =0;//la mutation d'une bactérie est spécifique à son type
    bool morte() const;//permet de savoir si la bactérie est morte
    virtual j::Value& getConfig() const =0;//servira aux sous-classes de faciliter la récupération des données dans app.json
    Quantity get_energie_min_div() const;//récupère la quantité minimale d'energie avant de se diviser
    sf::Time get_tps_attente() const;//récupère le temps que met la bactérie à manger
    Quantity get_energie_deplacement() const;// récupère l'energie necessaire au déplacement
    Quantity get_max_meal() const;//récupère l'energie que la bactérie prend par "bouchée"
    void setEnergie(Quantity qt); //initialise l'énergie (permet par exemple de tuer la bacterie)
    virtual void consumeEnergy(Quantity qt);//fait consommer de l'energie
    void update_compteur(sf::Time dt);//met a jour le compteur de temps
    void reset_compteur();//met à zero le compteur de temps
    void addProperty(const std::string& cle, MutableNumber valeur);//dans map cette fonction fait correspondre un MutableNumber à une chaine de caractères
    MutableNumber* getProperty(const std::string& cle);//dans map permet de retourner l'adresse mémoire du mutable number correspondant au string en parametre
    void rotation_ajour (sf::Time dt);//permet de mettre à jour la direction de la flagelle
    void eat(Nutriment& nutriment);//incorporation de l'energie du nutriment mangé

    virtual Quantity eatableQuantity(NutrimentA& nutriment) = 0; //définit la vitesse de consommation du Nutriment A par la bactérie (quantite
    virtual Quantity eatableQuantity(NutrimentB& nutriment) = 0; //définit la vitesse de consommation du Nutriment B par la bactérie


    Quantity get_poison(); //getter en cas d'empoisonnement
    virtual Quantity eatableQuantity(Poison& nutriment) = 0; //quantite de poison consommable par la bacterie
    void infection(); //si la bactérie n'était pas infectée, le booleen infecte devient vrai
    bool est_infecte() const; //obtient la valeur du booléen infecte


protected:

    virtual void move(sf::Time dt) =0; //dit que la sous-classe devra définir une manière spécifique de bouger
    virtual Bacterium* clone() =0;//dit que la sous-classe devra définir une manière spécifique de se cloner

    Vec2d direction;
    Quantity energie;
    MutableColor couleur;
    bool abstinence; //consomme ou pas des nutriments
    std::map<std::string, MutableNumber> parametre; //map avec les paramètres mutables
    sf::Time compteur; // compte le temps de vie de la bactérie
    double score; //perseption du gradient
    double rotation;

    sf::Time empoisonne; //0 si saine, si empoisonnée temps depuis l'empoisonnement (pour l'affichage)
    int I; //pour l'effet lors de l'infection viral
    bool infecte; //état d'infection ou non de la bactérie

};

