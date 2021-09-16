#include "Bacterium.hpp"
#include "Utility/Utility.hpp"
#include "Application.hpp"
#include "Config.hpp"
#include <iostream>
#include "Nutriment.hpp"
#include "NutrimentA.hpp"
#include "NutrimentB.hpp"

using namespace std;

Bacterium::Bacterium(Quantity energie, const Vec2d& position, const Vec2d& direction, double rayon,  MutableColor couleur)
    : CircularBody(position, rayon),
      direction(direction),
      energie(energie),
      couleur(couleur),
      abstinence(false),
      compteur(sf::Time::Zero),
      score(getAppEnv().getPositionScore(position)),
      rotation(0.0),
      empoisonne(sf::Time::Zero),
      I(0),
      infecte(false)
{}

Bacterium::Bacterium(const Bacterium& source)
    :CircularBody(source),
     direction(source.direction),
     energie(source.energie),
     couleur(source.couleur),
     abstinence(source.abstinence),
     parametre(source.parametre),
     compteur(source.compteur),
     score(source.score),
     rotation(source.rotation),
     empoisonne(source.empoisonne),
     I(source.I),
     infecte(source.infecte)
{}

Bacterium::~Bacterium() {}

void Bacterium::drawOn(sf::RenderTarget& targetWindow) const
{
    auto const circle = buildCircle(position, rayon, couleur.get()); //dessin du cercle
    targetWindow.draw(circle);
    if(isDebugOn()==true)  { //affichage en mode debug
        auto const text = buildText(std::to_string(energie), position, getAppFont(), 15, sf::Color::Black, 0.f);
        targetWindow.draw(text);
    }

    // si empoisonnement de la bactérie via la Poison de l'extension
    if (empoisonne > sf::Time::Zero) {
        auto const P = buildAnnulus(position, rayon, sf::Color::Green, 2);
        targetWindow.draw(P);
    }
    // si infection de la bactérie via le Virus de l'extension
    if(infecte) {
        auto const P = buildAnnulus(position, I, sf::Color::Magenta, 2);
        targetWindow.draw(P);

    }
}

void Bacterium::update(sf::Time dt)
{
    move(dt); //deplacement
    update_compteur(dt);
    //collision boite ou collision obstacle
    if (getAppEnv().global_colliding(*this)) {
        direction = -direction;
    }
    //collision nutriments
    if (getAppEnv().getNutrimentColliding(*this)!=nullptr) {
        if (!abstinence and compteur>=get_tps_attente()) {
            reset_compteur();
            eat(*(getAppEnv().getNutrimentColliding(*this)));
        }
    }
    if(energie >= get_energie_min_div()) {
        divide();
    }
    rotation_ajour(dt);


    if (empoisonne > sf::Time::Zero) { //permet l'affichage de l'empoisonnement un temps précis (décrémente)
        empoisonne -= dt;
    }
    int r(rayon); //I varie avec le temps, permet de changer le rayon du dessin lors de l'infection virale
    I = (I+1)%r;
}

void Bacterium::eat(Nutriment& nutriment)
{
    energie += nutriment.eatenBy(*this); //consommation du nutriment et gain d'energie
}

void Bacterium::divide()
{
    energie = energie/2;
    Bacterium* r1(clone()); //création et modification du clone
    r1->direction =  - r1->direction; //meilleur visibilité
    r1->mutate();
    getAppEnv().addBacterium_clone(r1); //ajout du clone
}

bool Bacterium::morte() const
{
    return energie <= 0.0;
}

Quantity Bacterium::get_energie_min_div() const
{
    return getConfig()["energy"]["division"].toDouble();
}

sf::Time Bacterium::get_tps_attente() const
{
    return sf::seconds(getConfig()["meal"]["delay"].toDouble());
}

Quantity Bacterium::get_energie_deplacement() const
{
    return getConfig()["energy"]["consumption factor"].toDouble();
}

Quantity Bacterium::get_max_meal() const
{
    return getConfig()["meal"]["max"].toDouble();
}


void Bacterium::consumeEnergy(Quantity qt)
{
    energie += -qt;

}

void Bacterium::update_compteur(sf::Time dt)
{
    compteur +=dt;
}

void Bacterium::reset_compteur()
{
    compteur = sf::Time::Zero;
}

void Bacterium::addProperty(const std::string& cle, MutableNumber valeur)
{
    parametre[cle]= valeur;
}

MutableNumber* Bacterium::getProperty(const std::string& cle)
{
    //prevision du cas où aucun MutableNumber ne correspond à la clé
    auto paire = parametre.find(cle);
    if(paire==parametre.end()) { //cle introuvable
        return nullptr;
    } else {
        return &(paire->second);
    }
}

void Bacterium::rotation_ajour(sf::Time dt)
{
    auto const angleDiff = angleDelta(direction.angle(), rotation);
    auto dalpha = M_PI * dt.asSeconds();    // calcule dα
    dalpha = std::min(dalpha, std::abs(angleDiff)); // on ne peut tourner plus que de angleDiff
    dalpha = std::copysign(dalpha, angleDiff); // on tourne dans la direction indiquée par angleDiff
    rotation += dalpha; // angle de rotation mis à jour
}

void Bacterium::setEnergie(Quantity qt)
{
    energie = qt;
}




Quantity Bacterium::get_poison()
{
    if (getConfig()["poison"].toDouble()!=0) {
        empoisonne += sf::seconds(1.0);
    }
    return getConfig()["poison"].toDouble();
}

void Bacterium::infection()
{
    infecte = true;
}

bool Bacterium::est_infecte() const
{
    return infecte;
}
