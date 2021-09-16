#include "Virus.hpp"
#include "Utility/Utility.hpp"
#include "Application.hpp"
#include "Config.hpp"
#include <iostream>

using namespace std;

Virus::Virus(const Vec2d& position)
    :CircularBody(position,uniform(getConfig()["radius"]["min"].toDouble(),
                                   getConfig()["radius"]["max"].toDouble())),
     direction(Vec2d::fromRandomAngle()),
     cible(nullptr),
     energie(uniform(getConfig()["energy"]["min"].toDouble(),
                     getConfig()["energy"]["max"].toDouble())),
     couleur(getConfig()["color"]),
     generation(1)
{
    parametre["speed"] = MutableNumber::positive(getConfig()["speed"]["initial"].toDouble(), getConfig()["speed"]["rate"].toDouble(), getConfig()["speed"]["sigma"].toDouble());
}

Virus::Virus(const Virus& V)
    :CircularBody(V.position,V.rayon),
     direction(V.direction),
     cible(nullptr),
     energie(V.energie),
     couleur(V.couleur),
     parametre(V.parametre)
{}

Virus::~Virus()
{
    //libération de la mémoire
    cible = nullptr;
    delete cible;
}

void Virus::drawOn(sf::RenderTarget& target) const
{
    if(cible==nullptr) {
        auto const octagon = buildOctagon(position, rayon, couleur.get());
        auto const anneau = buildAnnulus(position, rayon/2, sf::Color::Green, rayon/4);
        auto const anneau1 = buildAnnulus(position, rayon*1.4, couleur.get(), rayon/4);
        target.draw(octagon);
        target.draw(anneau);
        target.draw(anneau1);
        if(isDebugOn()==true)  {
            auto const text = buildText(std::to_string(energie), position, getAppFont(), 15, sf::Color::Black, 0.f);
            target.draw(text);
        }
    } else {}
}

void Virus::update(sf::Time dt)
{

    if(cible==nullptr) {
        move(dt);
        //colission boite/obstacle
        if (getAppEnv().global_colliding(*this)) {
            direction = -direction;
        }
        //collision bacterie
        if (getAppEnv().getBacteriesColliding(*this)!=nullptr) {
            cible = getAppEnv().getBacteriesColliding(*this);
            cible->infection();
            propagation_infection = sf::seconds(3);
        }
    } else {
        propagation_infection -= dt;
        position = cible->getPosition();

        if(propagation_infection<=sf::Time::Zero) {
            divide();
            cible->setEnergie(0.0);
            cible = nullptr;
            energie = 0.0;
        }
    }

}

void Virus::divide()
{
    energie = uniform(getConfig()["energy"]["min"].toDouble(),
                      getConfig()["energy"]["max"].toDouble());
    //4 clones
    for(unsigned int t(0); t <4; ++t ) {
        Virus* r1(clone());
        r1->direction = {cos(t*PI/2),sin(t*PI/2)}; //4 directions différentes en croix
        r1->generation = generation+1;
        r1->mutate();
        getAppEnv().addVirus_clone(r1);
    }
}

void Virus::mutate()
{
    couleur.mutate();
    getProperty("speed")->mutate();
}

bool Virus::morte() const
{
    return energie <= 0.0;
}

j::Value& Virus::getConfig() const
{
    return getAppConfig()["virus"];
}

void Virus::consumeEnergy(Quantity qt)
{
    energie -= qt;
}

Virus* Virus::clone()
{
    return new Virus(*this);
}

void Virus::move(sf::Time dt)
{

    Vec2d best(0.0, 0.0);
    for(size_t i(0); i<20; ++i) {
        Vec2d dir(Vec2d::fromRandomAngle());
        if(getAppEnv().getPositionBact(dir + position)> getAppEnv().getPositionBact(best + position) ) {
            best = dir;
        }
    }

    //On applique une loi de bernoulli car les virus ne doivent pas trop basculer, sinon les bacteries sont decimés trop vite
    //Et la disposition après division n'est pas observable
    if(((best[0]!=0.0) or (best[1]!=0.0)) and bernoulli(0.02)) {
        direction=best;
    }

    Vec2d pos(position);
    Vec2d vitesse(getSpeedVector());
    vitesse = stepDiffEq(position, vitesse, dt, *this).speed;

    if ((stepDiffEq(position, vitesse, dt, *this).position - position).lengthSquared()>0.001) {
        position = stepDiffEq(position, vitesse, dt, *this).position;
        consumeEnergy((position-pos).length()*0.2);
    }
}

Vec2d Virus::getSpeedVector()
{
    return direction * getProperty("speed")->get();
}

MutableNumber* Virus::getProperty(const std::string& cle)
{
    //prevision du cas où aucun MutableNumber ne correspond à la clé
    auto paire = parametre.find(cle);
    if(paire==parametre.end()) {
        return nullptr;
    } else {
        return &(paire->second);
    }
}

Vec2d Virus::f(Vec2d position,Vec2d speed) const
{
    Vec2d force(0,0);
    return force;
}

int Virus::get_generation() const
{
    return generation;
}

void Virus::setEnergie(Quantity qt)
{
    energie = qt;
}
