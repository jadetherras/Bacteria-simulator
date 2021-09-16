#include "SwarmBacterium.hpp"
#include"Utility/Vec2d.hpp"
#include "Application.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Config.hpp"
#include "Utility/Utility.hpp"
#include "Swarm.hpp"
#include "NutrimentA.hpp"
#include "NutrimentB.hpp"

using namespace std;

SwarmBacterium::SwarmBacterium(const Vec2d& position, Swarm* groupe)
    :Bacterium(uniform(getConfig()["energy"]["min"].toDouble(), getConfig()["energy"]["max"].toDouble()),
               position,
               Vec2d::fromRandomAngle(),
               uniform(getConfig()["radius"]["min"].toDouble(), getConfig()["radius"]["max"].toDouble()),
               groupe->couleur_depart()),
     appartenance(groupe)
{
    appartenance->add_SwarmBacterium(this); //mise dans le groupe
    getAppEnv().add_at_graph(s::SWARM_BACTERIA, true);
}

SwarmBacterium::SwarmBacterium(const SwarmBacterium& source)
    :Bacterium(source),
     appartenance(source.appartenance)
{
    appartenance->add_SwarmBacterium(this); //mise dans le groupe
    getAppEnv().add_at_graph(s::SWARM_BACTERIA, true);
}

SwarmBacterium::~SwarmBacterium()
{
    appartenance->sup_SwarmBacterium(this); //suppression du groupe
    getAppEnv().add_at_graph(s::SWARM_BACTERIA, false);
}

j::Value& SwarmBacterium::getConfig() const
{
    return getAppConfig()["swarm bacterium"];
}

Bacterium* SwarmBacterium::clone()
{
    return new SwarmBacterium(*(this));
}

Vec2d SwarmBacterium::f(Vec2d position, Vec2d speed) const
{
    Vec2d force(getAppConfig()["swarms"][appartenance->get_ident()]["force factor"].toDouble()*(appartenance->position_leader()-position));
    return force;
}

void SwarmBacterium::mutate()
{
    couleur.mutate();
}

void SwarmBacterium::drawOn(sf::RenderTarget& target) const
{
    Bacterium::drawOn(target);

    if ( appartenance->is_leader(*(this)) and isDebugOn()) { //si c'est la bacterie leader et que le debug est enclenché
        auto anneaux = buildAnnulus(position,rayon+10, sf::Color::Red, 2);
        target.draw(anneaux);
    }
}

void SwarmBacterium::move(sf::Time dt)
{
    Vec2d pos = position;
    Vec2d vitesse = getSpeedVector();
    vitesse = stepDiffEq(position, vitesse, dt, *this).speed; // se dirige vers le leader


    if ((stepDiffEq(position, vitesse, dt, *this).position - position).lengthSquared()>0.001) {

        //test des deplacements, car trop grand par rapport aux obstacles. Permet de prendre en compte ce dernier.
        position = stepDiffEq(position, vitesse, dt, *this).position;// déplacement

        if(!getAppEnv().global_colliding(*this)) {
            consumeEnergy((position-pos).length()*get_energie_deplacement());  //consommation d'energie
        } else {
            position = pos;
        } // si la bacterie ne rencontre pas d'obstacle, elle bouge et consomme l'energie, sinon elle reste en place.
    }

    if (appartenance->is_leader(*(this))) {
        Vec2d best(0.0, 0.0);
        for(size_t i(0); i<20; ++i) {
            Vec2d dir(Vec2d::fromRandomAngle());
            if(getAppEnv().getPositionScore(dir + position)> getAppEnv().getPositionScore(best + position) ) {
                best = dir;
            }
        }
        if(getAppEnv().getNutrimentColliding(*this) == nullptr) {
            direction=best;
            vitesse=getSpeedVector();
        }
    }
}

Vec2d SwarmBacterium::getSpeedVector() const
{
    return getConfig()["speed"]["initial"].toDouble()*direction;
}

Quantity SwarmBacterium::eatableQuantity(NutrimentA& nutriment)
{
    return nutriment.eatenBy(*this);
}

Quantity SwarmBacterium::eatableQuantity(NutrimentB& nutriment)
{
    return nutriment.eatenBy(*this);
}

Quantity SwarmBacterium::eatableQuantity(Poison& nutriment)
{
    return nutriment.eatenBy(*this);
}

